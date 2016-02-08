#ifndef NODE__HPP
#define NODE__HPP

#include <cstdint>
#include <vector>
#include "./graphics.hpp"


using namespace std;



class GraphNode
{
private:
  Point pos;
  int g,h,f;
public:
  GraphNode *parent;
  int getCost();
  int getG();
  void setParent(GraphNode* _par);
  void setH(int val);
  void setG(int val);
  void setF(int val);
  void setPixel(Pixel _p);
  void setPosition(int x, int y);
  Pixel getPixel();
  Point getPosition();
  

  bool operator==(const GraphNode& other)
  {
    if (this->pos.x == other.pos.x && this->pos.y == other.pos.y)
      {
	return true;
      }

    return false;
  }


};


#endif
