#!/bin/bash
# Shell script for compiling robot source code to binary
# Written 2015

COMPILEFLAGS="-ggdb -Werror -Wall"

echo "Compiling everything with debug flag"
echo "Compiling netpacket"
gcc $COMPILEFLAGS -c netpacket.c -o netpacket.o
echo "Compiling network"
gcc $COMPILEFLAGS -c network.c -o network.o
echo "Compiling llist"
gcc $COMPILEFLAGS -c llist.c -o llist.o
echo "Compiling argparse"
gcc $COMPILEFLAGS -c argparse.c -o argparse.o
echo "Compiling objects to final binary"
gcc $COMPILEFLAGS  network.o netpacket.o llist.o argparse.o -lm -o robot
echo "Done"
