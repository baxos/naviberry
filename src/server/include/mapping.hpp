#include <map>

struct Point
{
  int x;
  int y;
};


bool operator< (Point a, Point b) { return std::make_pair(a.x, a.y) < std::make_pair(b.x,b.y) ; }

enum class TileType { Unknown, Empty, Blocked };

class MapHandler
{
private:
  std::map<Point, TileType> Map;
  int MapSize;
  Point CurrentPosition;

public:
  void setTile(Point _pt, TileType _type);
  void setPosition(Point _newpt);
  MapHandler(int _size);
  Point getPosition() { return CurrentPosition; };
  TileType getTile(Point _pt);
  uint8_t* getByteArray();
};
