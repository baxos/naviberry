#!/bin/bash
# Clean BASH script for server part of project
# Written by Jan Emil Bacher


echo "Cleaning script.."

# Main binary
if [ -e "./server" ]
then
    echo "Removing server file"
    rm "./server"
fi


# Comm.o
if [ -e "./include/comm.o" ]
then
    echo "Removing comm.o"
    rm "./include/comm.o"
fi


# Status.o
if [ -e "./include/status.o" ]
then
    echo "Removing status.o"
    rm "./include/status.o"
fi


# Util.h
if [ -e "./include/util.o" ]
then
    echo "Removing util.h"
    rm "./include/util.h"
fi

echo "Done cleaning.."
