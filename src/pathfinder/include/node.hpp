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
  void UpdateCost();
  bool canWalk;
public:
  Point parent;
  GraphNode();
  int getCost();
  int getG();
  void setParent(Point _par);
  void setH(int val);
  void setG(int val);
  void setF(int val);
  void setPosition(int x, int y);
  void setWalkable(bool _val);
  bool isWalkable();
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
