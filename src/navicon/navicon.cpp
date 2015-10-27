/* 
   Author      : Jan Emil Bacher
   Name        : server.cpp
   Description : Server application used to recieve data from client and pass it further
                 on to system. See different list of commands in documentation/commands.lst
		 Furthermore documentation can be found in report in section regarding server app.
*/
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <string>
#include <list>
#include <cstring>
#include <thread>
#include <atomic>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include "./include/bcm2835.h"
}
#include "./include/comm.hpp"





int main()
{
  // Initialize networking
  Network net("localhost", 1000);




  // just exit
  return EXIT_SUCCESS;
}
