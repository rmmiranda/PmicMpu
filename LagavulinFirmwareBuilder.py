###############################################################################
# RocketFirmwareBuilder.py
#
# Generates a header file containing a Rocket Boot and App image to be stored
# in Nessus to allow firware updates. Requires 2 ASCII files as input which
# are generated from the TI Hex tool
#
# Also, injects CRC of image into image info struct at the start of each 
#
###############################################################################

import sys
import getopt
import re
import time

# Image size is fixed to 0xE000 bytes
IMG_SIZEOF = 0xE000

# Image info struct size is 18 bytes
IMG_INFO_SIZEOF = 18

# Image info struct is located at the very end of the image
IMG_INFO_ENTRYPOINT		= -22
IMG_INFO_COOKIE     	= -20
IMG_INFO_VERSION_MAJ    = -18
IMG_INFO_VERSION_MIN    = -16
IMG_INFO_LOADADDR   	= -14
IMG_INFO_ENDADDR    	= -12
IMG_INFO_TYPE       	= -10
IMG_INFO_PAIR_CRC   	= -8
IMG_INFO_CRC        	= -6


# extract from IAR hex file the boot and app images
def ExtractImagesFromIarHex( hex, boot, app ):
	
	lineno = 0
	bank = 0

	# read and process all lines from the hex file
	for line in open( hex, "rb").readlines():
		
		check = 0
		lineno += 1
		
		# each line should start with ':''
		if line[0] != ':':
			continue
		
		# get the number of data to store 
		databytes = int( line[1:3], 16 )
		check += databytes
		
		# get the low address
		address = (int( line[3:5], 16 ) << 8) + int( line[5:7], 16 )
		check += (address >> 8)
		check += address & 0xFF
		
		# proceed depending on the record type
		rectype = int( line[7:9], 16 )
		check += rectype
		if rectype == 4:
			# take note of the bank (high address)
			bank = int( line[9:13], 16 ) << 16
			continue
		if rectype == 1:
			break
		if rectype != 0:
			continue
		
		# valid data follows
		index = bank + address
		for data in range( 9, 2*databytes+9, 2 ):
			
			if index >= 0x12000:
				# store to app image
				app[index-0x12000] = line[data:data+2]
			else:
				if index >= 0x4000:
					# store to boot image
					boot[index-0x4000] = line[data:data+2]
					
			check += int( line[data:data+2], 16 )
			index += 1

		# read checksum
		sum = int( line[data+2:data+4], 16 )
		
		# validate checksum
		if sum != ((0x100 - (check & 0xFF)) & 0xFF):
			print "Bad checksum for line %d: expected=0x%04X actual=0x%04X" % (lineno, sum, ((0x100 - (check & 0xFF)) & 0xFF))

	return


# generate CRC-16-IBM compliant value for input data 
def GenerateImageCrc16rb( crc, poly, image ):
    
    for i in range( len(image)-8 ):

        data = (int( image[i], 16 ) ^ crc) & 0xFF
        crc = (crc >> 8) & 0xFF

    	parity = data ^ (data >> 4)
    	parity = parity ^ (parity >> 2)
    	parity = parity ^ (parity >> 1)
    	
    	if parity&1 == 1:
    		crc = crc ^ poly
    	
    	data = data << 6
    	crc = crc ^ data
    	data = data << 1
    	crc = crc ^ data

    return crc&0xFFFF


def usage():
	print "LagavulinFirmwareBuilder.py <image.hex> <output.h> <internal.h>"

def parseVersionFile( file ):

    found = 0
    binfo = {}

    header = open( file, "rb" )

    for line in header:
        regex = re.compile(r'#define[\s]LAGAVULIN_VERSION_MAJOR[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['major'] = int(str.group(1))
            found += 1
            
        regex = re.compile(r'#define[\s]LAGAVULIN_VERSION_MINOR[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['minor'] = int(str.group(1))
            found += 2

        regex = re.compile(r'#define[\s]LAGAVULIN_BUILD[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['build'] = int(str.group(1))
            found += 4
    
        regex = re.compile(r'#define[\s]LAGAVULIN_RELEASE[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['release'] = int(str.group(1))
            found += 8

    if found != 15:
        print "error parsing version header"
        sys.exit(-1);

    return binfo


def generatePadding( byteCount ):
    formatted = ""

    for index in range( byteCount ):
        if (index+1)%8 == 0:
            formatted += "0xFF,\n    "

        else:
            formatted += "0xFF, "

    return formatted

def formatConstData( data, pad ):
    formatted = ""

    #data elements are already in a hex string format, but are missing 0x##
    for index, item in enumerate( data ):

        if index+1 == len(data):
            if pad == 0:
                formatted += "0x"+item+"\n"
            else:
                if (index+1)%8 == 0:
                    formatted += "0x"+item+",\n    "
                else:
                    formatted += "0x"+item+", "
                    for x in range(7-((index+1)%8)):
                        formatted += "0xFF, "

                    formatted += "0xFF,\n    "

        elif (index+1)%8 == 0:
            formatted += "0x"+item+",\n    "

        else:
            formatted += "0x"+item+", "

    return formatted

if len(sys.argv) != 4:
    usage()
    sys.exit(-1)


scriptName = re.search( r"[\\/]*(\w+\.\w+)\s*",sys.argv[0]).group(1)
hexImageFile = sys.argv[1]
releaseHeaderName = sys.argv[2]
internalHeaderName = sys.argv[3]

# Create/get each name for the release header file and its contents
releaseHeaderName = releaseHeaderName.lower()
relVarName = re.search( r"[\\/]*(\w+)\.\w+\s*", releaseHeaderName ).group(1)
# format the variable and define strings
relVarName = relVarName.lower()
relDefName = relVarName.upper()

#create/get each name for the internal image header file and its contents
internalHeaderName = internalHeaderName.lower()
intVarName = re.search( r"[\\/]*(\w+)\.\w+\s*", internalHeaderName ).group(1)
# format the variable and define strings
intVarName = intVarName.lower()
intDefName = intVarName.upper()

# Extract entire image from the hex file
bootImage = ["FF"] * 0xE000
appImage = ["FF"] * 0xE000
ExtractImagesFromIarHex( hexImageFile, bootImage, appImage )


# Get build version number from version.h
buildVersion = parseVersionFile( "include\\lagavulin_version.h" )

# Generate CRC and insert it into the image (generate CRC from entrypoing and beyond 
# in ImageInfo struct at beginning of image)
bootCRC = GenerateImageCrc16rb( 0, 0xC001, bootImage[0:] )
bootImage[IMG_INFO_CRC] = "%02X" % (bootCRC & 0xFF)
bootImage[IMG_INFO_CRC+1] = "%02X" % (bootCRC >> 8)
print "    BootImage CRC   0x%X" % bootCRC

# Generate CRC and insert it into the image
appCRC = GenerateImageCrc16rb( 0, 0xC001, appImage[0:] )
appImage[IMG_INFO_CRC] = "%02X" % (appCRC & 0xFF)
appImage[IMG_INFO_CRC+1] = "%02X" % (appCRC >> 8)
print "    AppImage CRC    0x%X" % appCRC

# Put the 'pair CRC' into both images
# Put APP CRC into BOOT image
bootImage[IMG_INFO_PAIR_CRC] = "%02X" % (appCRC & 0xFF)
bootImage[IMG_INFO_PAIR_CRC+1] = "%02X" % (appCRC >> 8)
# Put BOOT CRC into APP image
appImage[IMG_INFO_PAIR_CRC] = "%02X" % (bootCRC & 0xFF)
appImage[IMG_INFO_PAIR_CRC+1] = "%02X" % (bootCRC >> 8)

bootVersion = (int( bootImage[IMG_INFO_VERSION_MAJ+1] + bootImage[IMG_INFO_VERSION_MAJ], 16 )) << 16 \
		+ int( bootImage[IMG_INFO_VERSION_MIN+1] + bootImage[IMG_INFO_VERSION_MIN], 16 )
appVersion = (int( appImage[IMG_INFO_VERSION_MAJ+1] + appImage[IMG_INFO_VERSION_MAJ], 16 )) << 16 \
		+ int( appImage[IMG_INFO_VERSION_MIN+1] + appImage[IMG_INFO_VERSION_MIN], 16 )

bootSize = (int( bootImage[IMG_INFO_ENDADDR+1] + bootImage[IMG_INFO_ENDADDR], 16 ) \
		- int( bootImage[IMG_INFO_LOADADDR+1] + bootImage[IMG_INFO_LOADADDR], 16 ) + 1) << 1
bootPercent = (bootSize*100 / 0xE000)
print "    BootImage Size  0x%X   ( %d%% )" % (bootSize, bootPercent)

appSize = (int( appImage[IMG_INFO_ENDADDR+1] + appImage[IMG_INFO_ENDADDR], 16 ) \
		- int( appImage[IMG_INFO_LOADADDR+1] + appImage[IMG_INFO_LOADADDR], 16 ) + 1) << 1
appPercent = (appSize*100 / 0xE000)
print "    AppImage Size   0x%X   ( %d%% )" % (appSize, appPercent )

# Do some sanity checks on the raw data and image info embedded in binary
if int( bootImage[IMG_INFO_TYPE], 16 ) != 1:
    print "Error: boot image info isn't of type IMAGE_TYPE_BOOT"
#    sys.exit(-1)

if int( appImage[IMG_INFO_TYPE], 16 ) != 2:
    print "Error: app image info isn't of type IMAGE_TYPE_APP"
#    sys.exit(-1)

if bootSize != len(bootImage):
    print "Error: Boot image info mismatch (bootSize)"
    print "bootSize = %d bootImage = %d" % (bootSize, len(bootImage))
#    sys.exit(-1)

if appSize != len(appImage):
    print "Error: App image info mismatch (appSize)"
    print "appSize = %d appImage = %d" % (appSize, len(appImage))
#    sys.exit(-1)

if bootVersion != appVersion:
    print "Error: Image version mismatch between Boot and App versions"
#    sys.exit(-1)

# Start generating release header file (for Nessus)
releaseHeader = open( releaseHeaderName, "w")

releaseHeader.write( "/*******************************************************************************\n" )
releaseHeader.write( " * Generated by %s - DO NOT MANUALLY EDIT\n" % scriptName )
#releaseHeader.write( " * %s\n" % time.strftime("%d.%m.%Y %I:%M:%S %p") )
releaseHeader.write( " ******************************************************************************/\n" )
releaseHeader.write( "\n" )
releaseHeader.write( "#ifndef __%s_H__\n" % relDefName )
releaseHeader.write( "#define __%s_H__\n\n\n" % relDefName )
releaseHeader.write( "#define %s_BUILD_VERSION     0x%02X%02X%02X\n" % (relDefName, buildVersion['major'], buildVersion['minor'], buildVersion['build']) )
releaseHeader.write( "#define %s_BUILD_VERSION_REL %d\n" % (relDefName, buildVersion['release'] ) )
releaseHeader.write( "\n" )
releaseHeader.write( "#define %s_BOOT_SIZE         0x%04X\n" % (relDefName, bootSize ) )
releaseHeader.write( "#define %s_BOOT_CRC          0x%04X\n" % (relDefName, bootCRC ) )
releaseHeader.write( "\n" )
releaseHeader.write( "#define %s_APP_SIZE          0x%04X\n" % (relDefName, appSize ) )
releaseHeader.write( "#define %s_APP_CRC           0x%04X\n" % (relDefName, appCRC ) )
releaseHeader.write( "\n\n" )

releaseHeader.write( "static const unsigned char %s_boot[ %d ] = { \n    " %(relVarName, len(bootImage)) )
releaseHeader.write( formatConstData(bootImage, 0 ) )
releaseHeader.write( "};\n\n" )

releaseHeader.write( "static const unsigned char %s_app[ %d ] = { \n    " %(relVarName, len(appImage)) )
releaseHeader.write( formatConstData(appImage, 0 ) )
releaseHeader.write( "};\n\n" )

releaseHeader.write( "#endif /* __%s_H__ */\n" % relDefName )

releaseHeader.close()

# Start generating internal header for loader (full, contiguous, image of app and boot)

imageGap = int( appImage[IMG_INFO_LOADADDR+1] + appImage[IMG_INFO_LOADADDR], 16 ) - \
            int( bootImage[IMG_INFO_ENDADDR+1] + bootImage[IMG_INFO_ENDADDR], 16 )

# used below when specifying number of 8 byte lines to pad between
# BOOT image end and APP image start
requiredPadding = imageGap - imageGap%8;

internalHeader = open( internalHeaderName, "w")

internalHeader.write( "/*******************************************************************************\n" )
internalHeader.write( " * Generated by %s - DO NOT MANUALLY EDIT\n" % scriptName )
#internalHeader.write( " * %s\n" % time.strftime("%d.%m.%Y %I:M%:%S %p") )
internalHeader.write( " ******************************************************************************/\n" )
internalHeader.write( "\n" )
internalHeader.write( "#ifndef __%s_H__\n" % intDefName )
internalHeader.write( "#define __%s_H__\n\n\n" % intDefName )

internalHeader.write( "const unsigned char %s[ %d ] = { \n    " %(intVarName, len(bootImage)+len(appImage)+imageGap) )
internalHeader.write( formatConstData( bootImage, 1 ) )
internalHeader.write( generatePadding( requiredPadding ) )
internalHeader.write( formatConstData( appImage, 0 ) )
internalHeader.write( "};\n\n" )

internalHeader.write( "#endif /* __%s_H__ */\n" % intDefName )

internalHeader.close()

sys.exit()
