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
  vector<Point> openPoints;
  vector<Point> checkedBoxes;

  bool goalReached, cantReach;
  bool isReady, isError;
  int ymax, xmax;
  Point targetPt;
  Point startPt;
public:
  int ManhattenCost(Point target, Point start);
  vector<GraphNode> getNeighbours(GraphNode* node);
  vector<GraphNode> getGoalRoute(GraphNode* goalNode);
  GraphNode *FindLowestNode();
  int FindLowestIndex();
  AStar(vector <vector<uint8_t> > map, int maxX, int maxY, Point* _target, Point* _start);
  ~AStar();
  int getTotalGraphCount();
  void Start();
  bool ClosedContains(GraphNode g);
  bool OpenContains(GraphNode g);
  vector<Point> getCheckedBoxes();

  void PrintOpen();
};


#endif

