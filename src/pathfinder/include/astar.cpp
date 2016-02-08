#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "./astar.hpp"

#define SLEEP_DURATION 1000 // 1000 ms / 1 sec


AStar::AStar(vector< vector<uint8_t> > map, int maxX, int maxY)
{
  // Initialize graph
  int x,y;
  y=0;
  x=0;

  goalReached = false;

  // Allocate data

  graph = new GraphNode*[maxY];
  for (auto i = 0; i < maxY; i++)
    {
      graph[i] = new GraphNode[maxX];
    }


  std::cout << "Starting to analyze data" << std::endl;
  std::cout << "Pool : " << map.size() << std::endl;

  for (auto it = map.begin(); it != map.end(); it++)
    {
      for (auto jt = it->begin(); jt != it->end(); jt++)
	{
	  GraphNode n;
	  n.setPosition(x, y);
	  n.setH(this->ManhattenCost(targetPt, n.getPosition()));
	  graph[x][y] = n;
	  
	  x++;
	}

      xmax = x;
      x= 0;
      y++;
    }


  ymax = y;

  cout << "XMAX : " << xmax << "\t YMAX : " << ymax << endl;
}

int AStar::ManhattenCost(Point target, Point start)
{
  int result = 0;
  int dx = target.x - start.x;
  int dy = target.y - start.y;


  if (dx < 0)
    dx = dx * (-1);

  if (dy < 0)
    dy = dy * (-1);

  result = dx + dy;

  return result;
}

vector<GraphNode> AStar::getNeighbours(GraphNode node)
{
  vector<GraphNode> result;
  Point pt = node.getPosition();
  Point newPt = pt;
  bool flowProblems = false;

  if (newPt.x > xmax || newPt.x < 0)
    {
      cout << (int) newPt.x << " > " << (int) xmax << endl;
      cout << (int) newPt.x << " < " << (int) 0 << endl;
      cout << "Fatal point recieved over/under flow happened!" << endl;
      flowProblems = true;
    }


  if (newPt.y > ymax || newPt.y < 0)
    {
      cout << "Y problem" << endl;
      flowProblems = true;
    }

  if (flowProblems == false)
    {
      // get upper
      newPt.y -= 1;
      if (newPt.y >= 0)
	{
	  result.push_back(graph[newPt.x][newPt.y]);
	}
      
      // get lower
      newPt = pt;
      newPt.y += 1;
      if (newPt.y < ymax)
	{
	  result.push_back(graph[newPt.x][newPt.y]);
	}
      
      // get left
      newPt = pt;
      newPt.x -= 1;
      if (newPt.x >= 0)
	{
	  result.push_back(graph[newPt.x][newPt.y]);
	}
      
      
      // get right
      newPt = pt;
      newPt.x += 1;
      if ( (newPt.x <= xmax) )
	{
	  result.push_back(graph[newPt.x][newPt.y]);
	}
    }
  cout << "Adding : " << result.size() << " elements" << endl;
  
  return result;
}


int AStar::getTotalGraphCount()
{
  return ymax * xmax;
}


void AStar::Start()
{
  // get starting node
  GraphNode currentNode = graph[startPt.x][startPt.y];
  currentNode.parent = nullptr;
  currentNode.setH(0);
  currentNode.setG(0);
  // add to open list
  open.push_back(&currentNode);

  cout << "Entering loop" << endl;
  while (open.size() > 0)
    {
      // Every iteration
      // print vector counts
      this->PrintOpen();


      // Sleep for human viewabillity??
      std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));


      // Take lowest cost node in the open list
      currentNode = FindLowestNode();


      // Add to closed list
      closed.push_back(&currentNode);


      // Check if this is the goal node
      if (currentNode.getPosition().x == targetPt.x && currentNode.getPosition().y == targetPt.y)
	{
	  // Goal reached
	  break;
	}

      
      // get neighbours
      auto vneigh = this->getNeighbours(currentNode);

      for (auto v : vneigh)
	{
	  // If its blocked or already in closed list, skip it
	  if (v.getCost() > 100000 || ClosedContains(v))
	    continue;

	  // calculate path cost
	  auto pathcost = currentNode.getCost() + 10;
	  // If new cost is lower than current, update to this path
	  // Or if the box hasn't been checked yet, update to this path
	  if (pathcost < v.getCost() || OpenContains(v) == false)
	    {
	      v.setG(pathcost);
	      v.setParent(&currentNode);

	      // Add to open, if not already there
	      if (OpenContains(v) == false)
		{
		  open.push_back(&v);
		}
	    }
	  
	}

      // repeat

    }
}

void AStar::PrintOpen()
{
  auto counter = 0;
  for (auto v : open)
    {
      cout << "[" << counter << "] x : " << (int) v->getPosition().x << "\t y : " << (int) v->getPosition().y << endl;
      cout << "[" << counter << "] cost : " << (int) v->getCost() << endl;
      counter++;
    }
}

bool AStar::OpenContains(GraphNode g)
{
  for (auto v : open)
    {
      if (*v == g)
	return true;
    }

  return false;
}

bool AStar::ClosedContains(GraphNode g)
{
  for (auto v : closed)
    {
      if (*v == g)
	return true;
    }

  return false;
}

void AStar::AddToClosed(GraphNode g)
{
  // Check if already on list
  auto alreadyOnList = false;
  if (ClosedContains(g) == false)
    {
      // add to closed list
      closed.push_back(&g);
    }
}

void AStar::AddToOpen(GraphNode g)
{
  if (OpenContains(g) == false && ClosedContains(g) == false)
    {
      open.push_back(&g);
    }
}

int AStar::FindLowestIndex()
{
  int index = 0;
  int counter = 0;
  int low = 99999999;

  for ( auto v : open)
    {
      if (v->getCost() < low)
	{
	  index = counter;
	  low = v->getCost();
	}

      counter++;
    }


  cout << "Open[" << index << "] == " << (int) low << "\t Total elements : " << open.size() << endl;
  return index;
}

GraphNode AStar::FindLowestNode()
{
   auto i = FindLowestIndex();

   GraphNode tmp = *open[i];

   // remove from open
   open.erase(open.begin() + i);

   return tmp;
}


void AStar::setTarget(Point _pt)
{
  targetPt = _pt;


  cout << "Target set : [ " << (int) targetPt.x << ", " << (int) targetPt.y << " ] " << endl;
}

void AStar::setStart(Point _pt)
{
  startPt = _pt;

  cout << "Start set : [ " << (int) startPt.x << ", " << (int) startPt.y << " ] " << endl;
}


vector<GraphNode> AStar::getGoalRoute(GraphNode* goalNode)
{
  vector<GraphNode> route;
  GraphNode* currentNode = goalNode;


  while (currentNode->parent != nullptr)
    {
      route.push_back(*currentNode);
      printf("X: %d \t Y: %d \n", currentNode->getPosition().x, currentNode->getPosition().y);
      currentNode = currentNode->parent;
    }

  return route;
}


vector<Point> AStar::getCheckedBoxes()
{
  if (checkedBoxes.size() > 1)
    {
      std::cout << "Returning " << checkedBoxes.size() << " boxes" << std::endl;

    }
  return checkedBoxes;
}

AStar::~AStar()
{
  // Clean up dynamic memory!
}
