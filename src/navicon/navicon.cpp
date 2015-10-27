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
      ip_str = *argv[1];
      port_str = *argv[2];
      if (argc > 2)
	{
	  for (int i = 2; i < argc; i++)
	    {
	      if (strcmp(argv[i], "-controller")==0)
		{
		  cout << "Controller opt in" << endl;
		  controller_opt = true;
		}
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


  // If window & controller are opt-in, initialize everything
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

 
  // If user asked for window, create it
  if (window_opt == true)
    {
      win = SDL_CreateWindow("Navicon", 300, 300, 640, 480, SDL_WINDOW_SHOWN);
      if (win == nullptr)
	print_error("Failed to create window");
    }

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
	  return -1;
	}
    }


  // start main loop
  while (quit != true)
    {
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
		  else if (e.caxis.value < -30000)
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

	  // Delay after each event

	}

    }


  SDL_DestroyWindow(win);
  SDL_Quit();
 

  // just exit
  return EXIT_SUCCESS;
}
