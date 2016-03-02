#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <random>
#include <ctime>
#include "./mapping.hpp"

using namespace std;

extern bool verboseView;

void Map::GenerateEmptyMap()
{
  vector<uint8_t> v (xsize);

  // GEnerate an empty row of 0's mathing the X length
  std::fill(v.begin(), v.end(), 0);


  for (auto i = 0; i < ysize; i++)
  {
    map.push_back(v);
  }

}

void Map::GenerateMazeMap()
{
  printf("[debug] generating %d x %d maze \n", xsize, ysize);
  std::srand(std::time(0));


  for (auto i = 0; i < ysize; i++)
    {
      // |
      // V

      auto doorOpen = std::rand() % (xsize-1);
      auto doorOpenTwo = std::rand() % (xsize-1);
      
      if (i % 2 == 0)
	{
	  for (auto j=0; j < xsize; j++)
	    {
	      if (j == doorOpen || j == doorOpenTwo)
		{
		  this->setTile(j,i, 0);
		}
	      else
		{
		  this->setTile(j,i,1);
		}
	    }
	}
    }
}

void Map::setTile(int x, int y, uint8_t val)
{

  map[y][x] = val;
}

vector< vector<uint8_t> > Map::getMap()
{
  return map;
}

Map::Map(int x, int y)
{
  ysize = y;
  xsize = x;
  map.resize(y);
  
  if (verboseView)
    {
      printf("Resizing map vector to %d x %d \n", x, y);
    }

  for ( auto& m : map)
    {
      m.resize(x);
    }
}





