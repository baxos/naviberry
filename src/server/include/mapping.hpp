#ifndef MAPPING__H
#define MAPPING__H

#include <map>
#include <cstdint>

struct Point
{
  int32_t x;
  int32_t y;
};


static bool operator< (Point a, Point b) { return std::make_pair(a.x, a.y) < std::make_pair(b.x,b.y) ; }

enum class TileType { Unknown, Empty, Blocked, None };
enum class MachineState { Standby, Forward, Backward, TurnRight, TurnLeft, ReadSensor };


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
  uint8_t* getByteArray();
};



#endif
