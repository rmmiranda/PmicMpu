# -------------------------------------------------------------------------
# Command line compiler define script
#
# Generates a list of --define=DEFINE=1 defines which can be used by the
# MSP430 linker as an input to allow pre-processing of the linker.cmd file 
#
# Essentially a copy of the h_to_d.pl file that generates a differnt 
# syntax of -D defines
#
# -------------------------------------------------------------------------

if( $#ARGV > 1 ) {
    die "Usage: <script> <header file> [variable]\n";
}

open( INFILE, $ARGV[0] ) or die "Can't open input file.";

if( $#ARGV > 0 ) {
    print $ARGV[1]." = ";
}

while($line = <INFILE>) {
    if( $line =~ /^\s*\#\s*define\s+(?:(\w+)\s+(\w+)|(\w+))\s*(\/\/.*)?$/ ) {
        if( $1 ) {
            print " --define=".$1."=".$2;
        } else {
            print " --define=".$3;
        }
    }
}


