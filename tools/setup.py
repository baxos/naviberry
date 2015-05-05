#!/bin/python3
# Setup script written in Python 3
# Goal is to ask user various questions regarding setup
# and then do the setup..
# Written 2015

def isNumber(nr):
    try:
        float(nr)
        return True
    except ValueError:
        return False


isError = False
portNumber = 0
ipStr = ''
robotNameStr = ''
rootDirectory = ''

print('Welcome to [NAME] Setup script')
portNumber   = input('Please select port number : ')
if isNumber(portNumber) == False:
    isError = True

ipStr        = input('Please write IP of server : ')
robotNameStr = input('Please write robot name   : ')
rootDirectory= input('Please write root directory to the robot package')

if isError:
    print('Error happened, please restart..')
    exit

# Else save settings
