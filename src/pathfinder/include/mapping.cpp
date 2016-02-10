#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <random>
#include <ctime>
#include "./mapping.hpp"

using namespace std;

void Map::GenerateEmptyMap()
{
  for (auto i = 0; i < ysize; i++)
    {
      vector<uint8_t> v;
      for (auto j = 0; j < xsize; j++)
	{
	  v.push_back(0);
	}

      map.push_back(v);
    }
}

void Map::GenerateMazeMap()
{
  std::srand(std::time(0));
  for (auto i = 0; i < ysize; i++)
    {
      // |
      // V

      auto doorOpen = std::rand() % (xsize-1);
      auto doorOpenTwo = std::rand() % (xsize-1);
      auto randNumber = std::rand() % (xsize-1);
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
  int cx,cy;
  cx=0;
  cy=0;
  for (auto it = map.begin(); it != map.end(); it++)
    {
      for (auto jt = it->begin(); jt != it->end(); jt++)
	{
	  if (cy == y && cx == x)
	    {
	      *jt = val;
	      break;
	    }
	  cx++;
	}

      cy++;
      cx = 0;
    }
}

vector< vector<uint8_t> > Map::getMap()
{
  return map;
}

Map::Map(int x, int y)
{
  ysize = y;
  xsize = x;
}





