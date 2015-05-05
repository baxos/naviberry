#!/bin/bash
# A small compile script
# written by j.e.b anno 2014

OUTPUTFILE="../img_process"
GCCOBJS="image_process.o img_process.o stack.o"
GCCFLAGS="-ggdb -lSDL2"
HRLINE="################################################################"
echo ""
echo "$HRLINE"
echo ""
echo "Removing old exec : $OUTPUTFILE"
rm $OUTPUTFILE
# Test
echo "Compiling with flags : $GCCFLAGS"
gcc -c img_process.c -o img_process.o $GCCFLAGS
gcc -c stack.c  -o stack.o $GCCFLAGS
gcc -c image_process.c -o image_process.o $GCCFLAGS

echo "Object files being compiled : $GCCOBJS"

gcc $GCCOBJS -o $OUTPUTFILE $GCCFLAGS

echo "Output file saved as : $OUTPUTFILE"
echo "Script done"
echo ""
echo "$HRLINE"
