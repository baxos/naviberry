#include "./node.hpp"

GraphNode::GraphNode()
{
  this->h = 0;
  this->f = 0;
  this->g = 0;

  this->parent = END_PT;
}


void GraphNode::setH(int val)
{
  this->h = val;

  this->UpdateCost();
}

void GraphNode::setG(int val)
{
  this->g = val;

  this->UpdateCost();
}

void GraphNode::setF(int val)
{
  this->f = val;

  this->UpdateCost();
}

void GraphNode::UpdateCost()
{
  this->f = this->g + this->h;
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

  this->pos = p;
}

void GraphNode::setParent(Point _par)
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
