#include <map>
#include <iostream>
#include <thread>
#include "./mapping.hpp"
#include "./naviberryio.hpp"

Direction UpdateDirectionByTurn(Direction _currentDirection, Direction _turn)
{
  switch (_turn)
    {
    case Direction::Right:
      switch(_currentDirection)
	{
	case Direction::Up:
	  _currentDirection = Direction::Right;
	  break;
	case Direction::Right:
	  _currentDirection = Direction::Down;
	  break;
	case Direction::Down:
	  _currentDirection = Direction::Left;
	  break;
	case Direction::Left:
	  _currentDirection = Direction::Up;
	  break;
	}
      break;
    case Direction::Left:
      switch(_currentDirection)
	{
	case Direction::Up:
	  _currentDirection = Direction::Left;
	  break;
	case Direction::Left:
	  _currentDirection = Direction::Down;
	  break;
	case Direction::Down:
	  _currentDirection = Direction::Right;
	  break;
	case Direction::Right:
	  _currentDirection = Direction::Up;
	    break;
	}
      break;
    }

  return _currentDirection;
}

Point IncreasePointByDirection(Point _pt, Direction _dir)
{
  switch(_dir)
    {
    case Direction::Up:
      _pt.y--;
      break;
    case Direction::Down:
      _pt.y++;
      break;
    case Direction::Right:
      _pt.x++;
      break;
    case Direction::Left:
      _pt.x--;
      break;
    }

  return _pt;
}

// Sets the specific point to the given type
/**
  * @name setTile
  * @brief Sets a tile to new value
  * @param _pt Coordinate to the tile
  * @param _type The tile type.
  * @retval None
  *
  *
  * This function simply sets a tile specified by a coordinate
  * to a new TileType
  **/
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

/**
 * @name MapeModeStart
 * @brief Sets the robot mode to MapMode
 * @param MotorController* _controller : The steering module
 * @param SonicSensor* _sensor : The distance reader module
 **/
void MapHandler::MapModeStart(MotorController* _controller, SonicSensor* _sensor)
{
  // Spawn thread with mapmodeFunc

  std::thread mapmodeThread (&MapHandler::mapmodeFunc, this, _controller, _sensor);
}

/**
 * @name MapModeStop
 * @brief Stops the robot MapMode
 **/
void MapHandler::MapModeStop()
{
  mapmodeThreadRun = false;
}

/**
 * @name mapmodeFunc
 * @brief Internal function used for threading
 **/
void MapHandler::mapmodeFunc(MotorController* _controller, SonicSensor* _sensor)
{
  // setup, initialize
  mapmodeThreadRun = true;
  const uint16_t freq = 5000;
  MachineState state = MachineState::Standby;
  Direction direction = Direction::Up;

  print_msg("Map mode thread, started and running");


  while (mapmodeThreadRun)
    {
      print_msg("Map mode..");
      
      switch (direction)
	{
	case Direction::Up:
	  print_msg("Direction facing up");
	  break;
	case Direction::Down:
	  print_msg("Direction facing down");
	  break;
	case Direction::Right:
	  print_msg("Direction facing right");
	  break;
	case Direction::Left:
	  print_msg("Direction facing left");
	  break;
	}

      Point p = this->getPosition();
      std::cout << "Position : [ " << p.x << " , " << p.y << " ]"<< std::endl;
      
      // Check distance:
      // if distance greater than 20
      //     Drive forward
      //     goto Check distance
      // else
      //     update map
      //     turn left OR turn right
      //     goto Check distance

      // Do the logic for setting the current state
      auto distance = 15 ; //current_distance;
      state = MachineState::Standby;

      if (distance > 20)
	{
	  // update position
	  Point pos = this->getPosition();
	  std::cout << pos.x << "\t" << pos.y << std::endl;
	  pos = IncreasePointByDirection(pos, direction);
	  std::cout << pos.x << "\t" << pos.y << std::endl;
	  this->setPosition(pos);
	  
	  // set mapp as free
	  this->setTile(this->getPosition(), TileType::Free);

	  // Set state
	  state = MachineState::Forward;
	  
	}
      else if(distance < 20 && distance > 3)
	{
	  // Set map+1 as block
	  Point pos = this->getPosition();
	  std::cout << pos.x << "\t" << pos.y << std::endl;
	  pos = IncreasePointByDirection(pos, direction);
	  std::cout << pos.x << "\t" << pos.y << std::endl;

	  // dont update position to handler
	  this->setTile(pos, TileType::Block);
	  
	  // Set state
	  state = MachineState::TurnRight;
	}
      else
	{
	  // Abort, failure has happened halt everything
	}


      // Handle the current state
      switch (state)
	{
	case MachineState::Standby:
	  // Do nothing
	  print_msg("Standing by..");
	  break;
	case MachineState::Forward:
	  // Drive forward
	  print_msg("Driving forward");
	  break;
	case MachineState::TurnRight:
	  // Turn right
	  direction = UpdateDirectionByTurn(direction, Direction::Right);
	  print_msg("Turning right");
	  break;

	}


      //      rob_sleep(freq);
   }
}


// Sets the current position of the robot on the map
/**  
  * @name setPosition
  * @brief Set the position of the robot on the map.
  * @param _newpt The new coordinate of the robot.
  * @retval None
  * 
  * 
  * Sets the new coordinate of the robot to the map class.
 **/
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
/**
 * @name MapHandler
 * @brief Constructs Maphandler
 * @param _size The size of the current map
 * @retval None
 *
 *
 * Constructs and initializes the MapHandler class.
 * It will iterate through all the coordinates and set the tiles to "unknown" 
 **/
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
/**
  * @name getTile
  * @brief Returns the TileType of the coordinate.
  * @param _pt The target tile coordinate
  * @retval Returns a TileType value
  *
  *
  * Returns a value of TileType fetched from a specific coordinate.
 **/
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
/**
  * @name getByteArray
  * @brief Returns a pointer to a byte array of the map
  * @retval uint8_t* pointing to a byte array
  *
  *
  * The function constructs a bytearray depeinding on the size of the array. 
  * The array will contain some few information bytes in the top.
  * uint8_t mapsize
  * uint8_t pos_x
  * uint8_t pos_y
  * uint8_t[mapsize*mapsize] data
 **/
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
