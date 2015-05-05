#!/bin/bash
# Compile BASH script for server part of project
# Written by Jan Emil Bacher

output="./server"

echo "Compiling script.."
if [ -e "$output" ]
then
    echo "Removing old file.."
    rm "$output"
fi


echo "Starting compilation with debug flag.."
echo "Compiling utils library.."
gcc -ggdb -c ./include/utils.c -o ./include/utils.o
echo "Compiling comm library.."
gcc -ggdb -c ./include/comm.c -o ./include/comm.o
echo "Compiling status library.."
gcc -ggdb -c ./include/status.c -o ./include/status.o

echo "Compiling main file.."
gcc  -ggdb server.c ./include/utils.o ./include/comm.o ./include/status.o  -o "$output"

if [ -e "$output" ]
then
    echo "Done compiling.."
else
    echo "Failed compiling.."
fi
