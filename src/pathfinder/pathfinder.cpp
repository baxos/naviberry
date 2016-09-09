/* 
   Author      : Jan Emil Bacher
   Name        : naviberry pathfinder module
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
#include "./include/navi-utils.hpp"



#define SCREEN_HEIGHT 1100
#define SCREEN_WIDTH 1200

#define GAME_X 200
#define GAME_Y 100


bool verboseView = false;

using namespace std;


int main(int argc, char* argv[])
{
  // Time the map creation
  Naviberry::StopWatch sw;


  sw.Start();
  

  Map m(GAME_X, GAME_Y);

  m.GenerateEmptyMap();

  m.GenerateMazeMap();

  
  sw.Stop();
  printf("[verbose] Time creation of the map %d miliseconds \n", sw.getTimeElapsed());

  
  // Make target
  Point target;
  target.x = 35;
  target.y = 35;

  // Make start point
  Point start;
  start.x = 1;
  start.y = 1;

  auto currentMap = m.getMap();
  AStar astar(&currentMap, GAME_X, GAME_Y, &target, &start);



  sw.Start();
  astar.Start();
  sw.Stop();

  printf("[verbose] completed route in %d ms \n", sw.getTimeElapsed());

 

  // just exit
  return EXIT_SUCCESS;
}
