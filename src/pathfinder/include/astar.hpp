#ifndef ASTAR__HPP
#define ASTAR__HPP


#include <vector>
#include <list>
#include <map>
#include <atomic>
#include "./graphics.hpp"
#include "./node.hpp"

#define GRAPH_X_MAX 250
#define GRAPH_Y_MAX 250


class AStar
{
 private:
  GraphNode graph[GRAPH_X_MAX][GRAPH_Y_MAX];
  list<Point> open;
  list<Point> closed;
  list<Point> goalRoute;
  list<Point> checkedPoints;


  bool goalReached, cantReach;
  bool isReady, isError;
  int ymax, xmax;
  Point targetPt;
  Point startPt;

  void ConstructGoalRoute(GraphNode* goalNode);
  Point FindLowestNode();
  bool ClosedContains(Point g);
  bool OpenContains(Point g);
 
  void PrintOpen();
  void PrintClosed();
public:
  int ManhattenCost(Point target, Point start);
  vector<GraphNode> getNeighbours(GraphNode* node);
  AStar(vector <vector<uint8_t> > map, int maxX, int maxY, Point* _target, Point* _start);
  int getTotalGraphCount();
  void Start();
  void Step();
  list<Point> getCheckedBoxes();
  list<Point> getGoalRoute();
  bool isGoalReached();
};


#endif

