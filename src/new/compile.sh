#!/bin/bash
#Compile script
# written jeb 2015

echo "Compiling"
echo "Compiling motordriver.c"
gcc -o motordriver.o -c motordriver.c -lbcm2835

echo "Compiling bcm wrapper"
gcc -o bcmwrapper.o -c bcmwrapper.c -lbcm2835

echo "Compiling main \t output : robot"
gcc -o robot main.c bcmwrapper.o motordriver.o -lbcm2835
