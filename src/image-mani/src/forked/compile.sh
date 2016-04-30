#!/bin/bash
# compile script

echo "Compiling img_process.o"
g++ -ggdb -std=c++11 -c img_process.cpp -o img_process.o
echo "Compiling main.o"
g++ -ggdb -std=c++11 -c main.cpp -o main.o
echo "Compiling executeable"
g++ -ggdb -std=c++11 main.o img_process.o
