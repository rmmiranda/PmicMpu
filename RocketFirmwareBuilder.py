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

# ImageInfo struct byte index info for specific elements
IMG_INFO_CRC        = 0
IMG_INFO_PAIR_CRC   = 2
IMG_INFO_ENTRYPOINT = 4
IMG_INFO_COOKIE     = 6
IMG_INFO_VERSION    = 8
IMG_INFO_LOADADDR   = 10
IMG_INFO_ENDADDR    = 12
IMG_INFO_TYPE       = 14
IMG_INFO_RESERVED   = 15
IMG_INFO_SIZEOF     = 16    # size, in bytes, of struct


def bitReflect( data, width ):

    result = 0
    for i in range(width):
        if ( data & 0x1 ):
            result = result | (0x1<<(width-1-i))

        data = data >> 1

    return result


# generate CRC-CCITT-BR complient value for input data 
# this will match the output of the CRC module within the MSP430
def GenerateImageCrc16rb( crc, poly, data ):
    
    for i in range( len(data) ):

        num = bitReflect( int( data[i], 16), 8)
        msg = num<<8

        for j in range(8):
            if ((msg ^ crc)>>15 & 1):
                crc = (crc<<1) ^ poly
            else:
                crc <<= 1
            
            msg<<=1
            # only keep track of a 16bit CRC
            crc &= 0xFFFF

    return crc


def usage():
    print "RocketFirmwareBuilder.py <image.boot> <image.app> <output.h> <internal.h>"

def parseVersionFile( file ):

    found = 0
    binfo = {}

    header = open( file, "rb" )

    for line in header:
        regex = re.compile(r'#define[\s]ROCKET_VERSION_MAJOR[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['major'] = int(str.group(1))
            found += 1
            
        regex = re.compile(r'#define[\s]ROCKET_VERSION_MINOR[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['minor'] = int(str.group(1))
            found += 2

        regex = re.compile(r'#define[\s]ROCKET_BUILD[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['build'] = int(str.group(1))
            found += 4
    
        regex = re.compile(r'#define[\s]ROCKET_RELEASE[\s]+([0-9]+)[\s]*')
        str = regex.match( line )
        if str != None:
            binfo['release'] = int(str.group(1))
            found += 8

    if found != 15:
        print "error parsing version header"
        sys.exit(-1);

    return binfo


def parseInputImage( image ):
    # Crop off the first and last characters now that we know the file
    # is probably a valid hex image
    image = image[1:-1]

    # Strip out all of the CR+LFs
    image = image.replace( "\r\n", "")
    # Clear out any trailing/leading whitespace so we don't split on it
    image = image.strip()
    # Chop up the bytes using the space separater
    image = image.split( " " );
    return image

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

if len(sys.argv) != 5:
    usage()
    sys.exit(-1)


scriptName = re.search( r"[\\/]*(\w+\.\w+)\s*",sys.argv[0]).group(1)
bootImageFile = sys.argv[1]
appImageFile = sys.argv[2]
releaseHeaderName = sys.argv[3]
internalHeaderName = sys.argv[4]

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

bootImage = open( bootImageFile, "rb").read()
if ord(bootImage[0]) != 2 and ord(bootImage[-1] != 3):
    print "Error: invalid boot image file specified"
    sys.exit(-1)

appImage = open( appImageFile, "rb").read()
if ord(appImage[0]) != 2 and ord(appImage[-1] != 3):
    print "Error: invalid app image file specified"
    sys.exit(-1)


# Get build version number from version.h
buildVersion = parseVersionFile( "include\\version.h" )

# Clean up boot image input
bootImage = parseInputImage( bootImage )

# Clean up app image input
appImage = parseInputImage( appImage )

# Generate CRC and insert it into the image (generate CRC from entrypoing and beyond 
# in ImageInfo struct at beginning of image)
bootCRC = GenerateImageCrc16rb( 0xFFFF, 0x1021, bootImage[IMG_INFO_ENTRYPOINT:] )


bootImage[IMG_INFO_CRC] = "%02X" % (bootCRC & 0xFF)
bootImage[IMG_INFO_CRC+1] = "%02X" % (bootCRC >> 8)


print "    BootImage CRC   0x%X" % bootCRC

# Generate CRC and insert it into the image
appCRC = GenerateImageCrc16rb( 0xFFFF, 0x1021, appImage[IMG_INFO_ENTRYPOINT:] )
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

bootVersion = int( bootImage[IMG_INFO_VERSION+1] + bootImage[IMG_INFO_VERSION], 16 )
appVersion = int( appImage[IMG_INFO_VERSION+1] + appImage[IMG_INFO_VERSION], 16 )

bootSize = int( bootImage[IMG_INFO_ENDADDR+1] + bootImage[IMG_INFO_ENDADDR], 16 ) \
    - int( bootImage[IMG_INFO_LOADADDR+1] + bootImage[IMG_INFO_LOADADDR], 16 )
bootPercent = (bootSize*100 / 0x3c00)
print "    BootImage Size  0x%X   ( %d%% )" % (bootSize, bootPercent)

appSize = int( appImage[IMG_INFO_ENDADDR+1] + appImage[IMG_INFO_ENDADDR], 16 ) \
    - int( appImage[IMG_INFO_LOADADDR+1] + appImage[IMG_INFO_LOADADDR], 16 )
appPercent = (appSize*100 / 0x3c00)
print "    AppImage Size   0x%X   ( %d%% )" % (appSize, appPercent )

# Do some sanity checks on the raw data and image info embedded in binary
if int( bootImage[IMG_INFO_TYPE], 16 ) != 1:
    print "Error: boot image info isn't of type IMAGE_TYPE_BOOT"
    sys.exit(-1)

if int( appImage[IMG_INFO_TYPE], 16 ) != 2:
    print "Error: app image info isn't of type IMAGE_TYPE_APP"
    sys.exit(-1)

if bootSize != len(bootImage):
    print "Error: Boot image info mismatch (bootSize)"
    print "bootSize = %d bootImage = %d" % (bootSize, len(bootImage))
    sys.exit(-1)

if appSize != len(appImage):
    print "Error: App image info mismatch (appSize)"
    sys.exit(-1)

if bootVersion != appVersion:
    print "Error: Image version mismatch between Boot and App versions"
    sys.exit(-1)

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
