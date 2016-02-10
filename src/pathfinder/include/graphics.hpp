#ifndef GRAPHICS__HPP
#define GRAPHICS__HPP

#include <cstdint>
#include <vector>
#include <list>
#include <SDL2/SDL.h>

using namespace std;



struct Point
{
  int x, y;

  Point operator=(const Point& other)
  {
    this->x = other.x;
    this->y = other.y;
  }

  bool operator==(const Point& p1)
  {
    if (this->x == p1.x && this->y == p1.y)
      return true;
    else
      return false;
  }

  bool operator!=(const Point& p1)
  {
    if (this->x == p1.x && this->y == p1.y)
      return false;
    else
      return true;
  }

  
};

const Point END_PT = { -999, -999 };

struct Pixel
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
};

enum class  Color { Black, Red, Green, DarkGreen,  White, Blue, Gray,  };

class Graphics
{
private:
  vector< vector<uint8_t> > map;
  SDL_Renderer *renderer;
  int screenX, screenY;
  int targetX, targetY;
  int startX, startY;
public:
  void ClearImage();
  void setTarget(int x, int y);
  void setStart(int x, int y);
  void DrawGrid();
  void DrawTarget();
  void DrawVisited(list<Point> boxes, Color c);
  void DrawStart();
  void setColor(Color c);
  void ConstructImage(vector<vector<uint8_t>> v);
  Graphics(SDL_Renderer *_render);
};


#endif
