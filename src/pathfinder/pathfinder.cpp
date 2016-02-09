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
  win = SDL_CreateWindow("Navicon", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == nullptr)
    return EXIT_FAILURE;

  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
    return EXIT_FAILURE;


  Graphics g(renderer);
  Map m(10, 10);

  m.GenerateEmptyMap();
  for (auto i=0; i < 7; i++)
    {
      m.setTile(i, 3, 1);
    }



  // Make target
  Point target;
  target.x = 9;
  target.y = 9;

  // Make start point
  Point start;
  start.x = 2;
  start.y = 2;

  AStar astar(m.getMap(), 10, 10, &target, &start);


  g.ConstructImage(m.getMap());



  g.setTarget(target.x, target.y);
  g.setStart(start.x, start.y);




  // spawn thread for solve the pathfinding
  std::thread astarSolver (&AStar::Start, astar);




  // Prepare graphics

  // 
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.h = 50;
  rect.w = 50;




  //  SDL_RenderCopy (renderer, texture, NULL , NULL);
  SDL_RenderPresent(renderer);



  // start main loop
  while (quit != true)
    {
      
      g.ClearImage();
      g.DrawTarget();
      g.DrawStart();
      g.DrawGrid();

      // if we have checked boxes, draw them
      auto checkedboxes = astar.getCheckedBoxes();

      if(checkedboxes.size() > 1)
	{
	  g.DrawVisited(astar.getCheckedBoxes());
	}
      
	SDL_RenderPresent(renderer);
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
