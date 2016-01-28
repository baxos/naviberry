/* 
   Author      : Jan Emil Bacher
   Name        : navicon.cpp
   Description : Linux client application written for "Naviberry Project". 
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
#include <SDL2/SDL.h>
#include "./include/comm.hpp"
#include "./include/naviberryio.hpp"

// for verbose console view
#define __DEBUG__ true


using namespace std;


void print_usage()
{
  cout << "Wrong usage! Please see below" << endl;
  cout << "./navicon [ip] [port]" << endl;
  cout << "Optional : -controller -window" << endl;
}

void show_help()
{
  cout << "Help"<< std::endl << "===============" << std::endl;
  cout << "showgui \t Shows graphical interface, if program started with -window" << std::endl;
  cout << "connect \t Will try to connect to the given port and IP address." << std::endl;
  cout << "help \t Shows this menu" << std::endl;
  cout << "quit \t Quits the programs" << std::endl;
}

void show_window(SDL_Window* _win)
{
  _win = SDL_CreateWindow("Navicon GUI", 300,400, 640, 480, SDL_WINDOW_SHOWN);
  if (_win == NULL)
    {
      // Error
      
    }
}

int main(int argc, char* argv[])
{
  string ip_str = "";
  string port_str = "";
  bool controller_opt = false;
  bool window_opt     = false;


  // Handle input arguments
  // if less than 2 arguments exit
  if (argc < 2)
    {
      print_usage();
      return EXIT_FAILURE;
    }
  else
    {
      // Set local variables, to the argument passed
      ip_str = *argv[1];
      port_str = *argv[2];

      // Try parse, rest of the non-require arguments
      if (argc > 2)
	{
	  for (int i = 2; i < argc; i++)
	    {
	      // for xbox, ps3 controller usage
	      if (strcmp(argv[i], "-controller")==0)
		{
		  cout << "Controller opt in" << endl;
		  controller_opt = true;
		}
	      // graphical usage
	      else if (strcmp(argv[i], "-window")==0)
		{
		  cout << "Window opt in" << endl;
		  window_opt = true;
		}	      
	    }
	  
	}
    }

  // Initialize networking
  Network net(ip_str, 1000);
  // Sdl
  SDL_Window* win = NULL;
  SDL_Event e;
  bool quit = false;


  // If window or controller are opt-in, initialize everything
  if (window_opt == true || controller_opt == true)
    {
      if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
	  print_error("Failed to SDL_INIT_EVERYTHING");
	}
    }
  else
    {

    }
  

  // Ok sdl is initialized now

  /* ************** MOVED
  // If user asked for window, create it
  if (window_opt == true)
    {
      win = SDL_CreateWindow("Navicon", 300, 300, 640, 480, SDL_WINDOW_SHOWN);
      if (win == nullptr)
	print_error("Failed to create window");
    }
  */


  // If user asked for controller, set it up
  if (controller_opt == true)
    {
      // Checking controllers/joystsiks
      auto controllers = SDL_NumJoysticks();
      cout << "Currently connected joystick = " << controllers << endl;
      SDL_GameController* control = nullptr;
      
      // if there is no controllers connected, quit
      if (controllers > 0)
	{
	  control = SDL_GameControllerOpen(0);
	}
      else
	{
	  cout << "No controllers connected, closing.." << endl;
	  return EXIT_FAILURE;
	}
    }


  std::cout << "Starting main loop" << std::endl;

  // start main loop
  while (quit != true)
    {
      if ((window_opt) || (controller_opt))
	{
	  std::cout<<"Fetching controller input" << std::endl;
	  while (SDL_PollEvent(&e))
	    {
	      switch (e.type)
		{
		case SDL_CONTROLLERDEVICEADDED:
		  cout << "A controller was found" << endl;
		  break;
		case SDL_CONTROLLERAXISMOTION:
		  switch  (e.caxis.axis)
		    {
		    case SDL_CONTROLLER_AXIS_LEFTX:
		      if (__DEBUG__)
			cout <<"Axis left X : " << (int) e.caxis.value << endl;
		      if (e.caxis.value > 30000)
			{
			  cout << "Right" << endl;
			}		 
		      else if (e.caxis.value < -30000)
			{
			  cout << "Left" << endl;
			}		 
		      break;
		    case SDL_CONTROLLER_AXIS_LEFTY:
		      if (__DEBUG__)
			cout << "Axis left Y : " << (int) e.caxis.value << endl;
		      if (e.caxis.value > 30000)
			{
			  cout << "Down" << endl;
			}
		      else if  (e.caxis.value < -30000)
			{
			  cout << "Up" << endl;
			}
		      break;
		    }
		  break;
		case SDL_CONTROLLERBUTTONDOWN:
		  switch(e.cbutton.button)
		    {
		    case SDL_CONTROLLER_BUTTON_A:
		      cout << "A pressed" << endl;
		      break;
		    case SDL_CONTROLLER_BUTTON_B:
		      cout << "B pressed" << endl;
		      break;
		    case SDL_CONTROLLER_BUTTON_BACK:
		      cout << "Back pressed, closing.." << endl;
		      quit = true;
		      break;
		    case SDL_CONTROLLER_BUTTON_DPAD_UP:
		      cout << "Up pressed" << endl;
		      break;
		    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		      cout << "Down pressed" << endl;
		      break;
		    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		      cout << "Right pressed" << endl;
		      break;
		    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		      cout << "Left pressed" << endl;
		      break;
		    }	   
		  break;
		case SDL_QUIT:
		  quit = true;
		  cout << "Exiting.." << endl;
		  break;
		}
	    }
	  
	  if (controller_opt == false)
	    {
	      std::cout << "Input command " ;
	      string input_str = "";
	      std::cin >> input_str;

	    }

	  
	  // Delay after each event
	  
	}
      

      if (controller_opt == false)
	{
	  // Get data from user
	  std::cout <<"[Input] ";
	  string input_str = "";
	  std::cin >> input_str;

	  // Parse it
	  if (input_str == "showgui")
	    {
	      // Only avaible with -window flag
	      if (window_opt)
		show_window(win);
	      else {
		show_help();
	      }
	    }
	  else if(input_str == "quit")
	    {
	      // Finish program
	      std::cout << "Exiting.." << std::endl;
	      quit = true;
	    }
	  else if(input_str == "help")
	    {
	      show_help();
	    }
	  else if(input_str == "connect")
	    {
	      // Tries to connect to the given IP and port
	    }
	}

    }
  
  
  SDL_DestroyWindow(win);
  SDL_Quit();
 

  // just exit
  return EXIT_SUCCESS;
}
