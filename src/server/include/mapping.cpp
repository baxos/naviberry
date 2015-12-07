#include <map>
#include "./mapping.hpp"



// Sets the specific point to the given type
void MapHandler::setTile(Point _pt, TileType _type)
{
  Map.insert(std::make_pair(_pt, _type));
}

// Sets the current position of the robot on the map
void MapHandler::setPosition(Point _newpt)
{
  CurrentPosition = _newpt;
}

// Class initializer
// set local scope variables
// initialize data
MapHandler::MapHandler(int _size)
{
  // Initialize the class
  // Set local scope variables
  MapSize = _size;
  
  Point startpos;
  startpos.x = MapSize / 2;
  startpos.y = startpos.x;
  setPosition(startpos);

  for (auto ity = 0; ity<MapSize; ity++)
    {
      for (auto itx = 0; itx<MapSize; itx++)
	{ 
	  // x x1 x2 x3
 	  // y         ->
	  // y1      itx
	  // y2
	  // |  ity
	  // V
	  // Set tile itx, ity type = unknown
	  Point pos = { itx, ity };
	  setTile(pos, TileType::Unknown);
	}
    }
}

TileType MapHandler::getTile(Point _pt)
{
  // If map contains the point return the value of tiletype
  if (Map.count(_pt))
    {
      return Map[_pt];
    }
}
