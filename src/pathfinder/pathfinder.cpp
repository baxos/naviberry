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
#include <thread>
#include <SDL2/SDL.h>
#include "./include/graphics.hpp"
#include "./include/mapping.hpp"
#include "./include/astar.hpp"
#include "./include/node.hpp"


#define SCREEN_HEIGHT 1100
#define SCREEN_WIDTH 1200

#define GAME_X 100
#define GAME_Y 100

using namespace std;


int main(int argc, char* argv[])
{
  // Sdl
  SDL_Window* win = NULL;
  SDL_Event e;
  SDL_Renderer *renderer;
  SDL_Surface *surface;

  bool quit = false;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      return EXIT_FAILURE;
    }
  
  
  
  // If user asked for window, create it
  win = SDL_CreateWindow("Naviberry pathfinder", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr)
    return EXIT_FAILURE;

  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
    return EXIT_FAILURE;


  Graphics g(renderer);
  Map m(GAME_X, GAME_Y);

  m.GenerateEmptyMap();

  m.GenerateMazeMap();


  // Make target
  Point target;
  target.x = 35;
  target.y = 35;

  // Make start point
  Point start;
  start.x = 1;
  start.y = 1;

  AStar astar(m.getMap(), GAME_X, GAME_Y, &target, &start);


  g.ConstructImage(m.getMap());



  g.setTarget(target.x, target.y);
  g.setStart(start.x, start.y);




  

  // Prepare graphics

  // 
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.h = 50;
  rect.w = 50;




  //  SDL_RenderCopy (renderer, texture, NULL , NULL);
  SDL_RenderPresent(renderer);



  //  astar.Start();
  
  


  // start main loop
  while (quit != true)
    {
      
      g.ClearImage();
      g.DrawTarget();
      g.DrawStart();
      g.DrawGrid();
    
      
      auto cboxes = astar.getCheckedBoxes();
      if (cboxes.size() > 0)
	{
	  g.DrawVisited(cboxes, Color::Gray);
	}
      
  
      // if we have goal route
      if (astar.isGoalReached())
	{
	  g.DrawVisited(astar.getGoalRoute(), Color::DarkGreen);
	}
      else
	{
	  astar.Step();
	}
      

      SDL_RenderPresent(renderer);



      //      std::this_thread::sleep_for(std::chrono::milliseconds(10));

      // sleep?

      while (SDL_PollEvent(&e))
	{
	  if (e.type == SDL_QUIT)
	    {
	      quit = true;
	    }
	}
    }
  


  
  SDL_DestroyWindow(win);
  SDL_Quit();
 

  // just exit
  return EXIT_SUCCESS;
}
