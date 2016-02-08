#include "./node.hpp"

void GraphNode::setH(int val)
{
  this->h = val;
}

void GraphNode::setG(int val)
{
  this->g = val;

  this->setF(g + h);
}

void GraphNode::setF(int val)
{
  this->f = val;
}

int GraphNode::getCost()
{
  return this->f;
}

void GraphNode::setPosition(int x, int y)
{
  Point p;
  p.x = x;
  p.y = y;

  pos = p;
}

void GraphNode::setParent(GraphNode *_par)
{
  this->parent = _par;
}

Point GraphNode::getPosition()
{
  return pos;
}


int GraphNode::getG()
{
  return g;
}
