#include <map>
#include <iostream>
#include "./mapping.hpp"
#include "./naviberryio.hpp"



// Sets the specific point to the given type
void MapHandler::setTile(Point _pt, TileType _type)
{
  if (Map.count(_pt))
    {
      print_warning("Changing map tile");
      Map[_pt] = _type;
    }
  else
    {
      Map.insert(std::make_pair(_pt, _type));
    }
}

// Sets the current position of the robot on the map
void MapHandler::setPosition(Point _newpt)
{
  std::cout << "X" << _newpt.x << "\t Y" << _newpt.y << std::endl;
  CurrentPosition = _newpt;
  print_warning("New position set");
  std::cout << "X " << CurrentPosition.x << "\t Y " << CurrentPosition.y << std::endl;
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
  auto iterator_count = 0;

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
	  iterator_count++;
	}
    }

  std::cout << "Total count : " << std::to_string(iterator_count) << std::endl;
}

// Returns the given tilen bound to the point
TileType MapHandler::getTile(Point _pt)
{
  // If map contains the point return the value of tiletype
  if (Map.count(_pt))
    {
      return Map[_pt];
    }
  else
    {
      return TileType::None;
    }
}
 

// Return the byte array of the data structure
// uint8 mapsize     [ eg : 50       ]
// uint8 pos_x       [ eg : [0..50]  ]
// uint8 pos_y       [ eg : [0..50]  ]
// uint8[mapsize^2]  [ eg : [b,b,b..b]
// Convert each datatype to raw bytes
// and create a sequential array of the bytes
// ready to send..
uint8_t* MapHandler::getByteArray()
{
  auto offset = 0;
  uint8_t* arr = new uint8_t[(50*50) + 3];
  
  // Check data
  if (arr == nullptr)
    print_error("Memory error");
  
  auto pos =  getPosition();

  // Set mapsize
  arr[offset] = 50;
  offset++;
  // set position x
  // dangerous typecast
  arr[offset] = (uint8_t) pos.x;
  offset++;
  // set position y
  // dangerous typecast
  arr[offset] = (uint8_t) pos.y;
  offset++;

  auto iterator_count  = 0;
  for (auto ity=0; ity<50; ity++)
    {
      for (auto itx=0; itx<50; itx++)
	{
	  int pos = (ity * 50) + itx;
	  Point pt { itx, ity };
	  arr[pos+offset] = uint8_t (getTile(pt)); 


	  std::cout << std::to_string( arr[pos]) << ",\t";
	  iterator_count++;
	}
    }


  std::cout << std::endl << "Counts : " << std::to_string(iterator_count) << std::endl;
  return arr;
}
