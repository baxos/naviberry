#ifndef MAPPING__H
#define MAPPING__H

#include <map>
#include <cstdint>
#include "./point.hpp"

enum class TileType { Unknown, Free, Block, None };
enum class MachineState { Standby, Forward, Backward, TurnRight, TurnLeft, ReadSensor };
enum class Direction { Up=0, Right=1, Down=2, Left=3 };

Direction UpdateDirectionByTurn(Direction _currentDirection, Direction _turn);
Point IncreasePointByDirection(Point _pt, Direction _dir);

static bool operator< (Point a, Point b) { return std::make_pair(a.x, a.y) < std::make_pair(b.x, b.y);}

class MapHandler
{
private:
  std::map<Point, TileType> Map;
  int32_t MapSize;
  Point CurrentPosition;

public:
  void setTile(Point _pt, TileType _type);
  void setPosition(Point _newpt);
  MapHandler(int32_t _size);
  Point getPosition() { return CurrentPosition; };
  TileType getTile(Point _pt);
  uint32_t getDataSize() { return (MapSize*MapSize) + 3; };
  uint8_t* getByteArray();
};



#endif
