#ifndef ASTAR__HPP
#define ASTAR__HPP


#include <vector>
#include <list>
#include <map>
#include <atomic>
#include "./graphics.hpp"
#include "./node.hpp"


class AStar
{
 private:
  GraphNode** graph;
  vector<GraphNode*> open;
  vector<GraphNode*> closed;
  vector<GraphNode> checked;

  vector<Point> checkedBoxes;

  bool goalReached, cantReach;
  int ymax, xmax;
  Point targetPt;
  Point startPt;
public:
  int ManhattenCost(Point target, Point start);
  vector<GraphNode> getNeighbours(GraphNode node);
  vector<GraphNode> getGoalRoute(GraphNode* goalNode);
  GraphNode FindLowestNode();
  int FindLowestIndex();
  AStar(vector <vector<uint8_t> > map, int maxX, int maxY);
  ~AStar();
  int getTotalGraphCount();
  void Start();
  void setTarget(Point _pt);
  void setStart(Point _pt);
  void AddToOpen(GraphNode g);
  void AddToClosed(GraphNode g);
  bool ClosedContains(GraphNode g);
  bool OpenContains(GraphNode g);
  vector<Point> getCheckedBoxes();

  void PrintOpen();
};


#endif

