#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "./astar.hpp"

#define SLEEP_DURATION 1000 // 1000 ms / 1 sec


AStar::AStar(vector< vector<uint8_t> > map, int maxX, int maxY, Point* _target, Point* _start)
{
  // Initialize graph
  int x,y;
  y=0;
  x=0;


  // Set flags
  isReady = false;
  isError = false;
  goalReached = false;

  this->targetPt = *_target;
  this->startPt = *_start;

  // Allocate data

  graph = new GraphNode*[maxY];
  if (graph == nullptr)
    {
      // Error
      isError = true;
    }
  for (auto i = 0; i < maxY; i++)
    {
      graph[i] = new GraphNode[maxX];

      if (graph[i] == nullptr)
	{
	  isError = true;
	}
    }




  if (!isError)
    {
      cout << "Starting to analyze data" << std::endl;
      cout << "Pool size : " << maxX * maxY << std::endl;

      for (auto it = map.begin(); it != map.end(); it++)
	{
	  for (auto jt = it->begin(); jt != it->end(); jt++)
	    {
	      GraphNode* n = new GraphNode();
	      n->setPosition(x, y);
	      n->setH(this->ManhattenCost(targetPt, n->getPosition()));
	  
	      cout << "[Adding] N = [x, y] " << n->getPosition().x << ", " << n->getPosition().y << endl;
	      cout << "[Cost] F = " << n->getCost() << endl;
	      graph[x][y] = *n;
	  
	      x++;
	    }

	  xmax = x;
	  x= 0;
	  y++;
	}
      ymax = y;
    }

  if (!isError)
    {
      cout << "Init done!" << endl;
      isReady = true;
    }
  else
    {
      cout << "Error during init, exiting program.." << endl;
      exit(-1);
    }
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

vector<GraphNode> AStar::getNeighbours(GraphNode* node)
{
  vector<GraphNode> result (0);
  Point pt;
  pt.x = node->getPosition().x;
  pt.y = node->getPosition().y;

  cout << "Pt     : x " << pt.x << "\t y " << pt.y << endl;  
  Point newPt = pt;
  cout << "newPt  : x : " << newPt.x << "\t y " << newPt.y << endl; 
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
	  GraphNode g = graph[newPt.x][newPt.y];
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
  GraphNode *currentNode = &graph[startPt.x][startPt.y];
  currentNode->parent = nullptr;
  currentNode->setH(0);
  currentNode->setG(0);


  // add to open list
  openPoints.push_back(currentNode->getPosition());
  open.push_back(currentNode);

  cout << "Entering loop" << endl;
  while (open.size() > 0)
    {
      // Every iteration
      // print vector counts
      this->PrintOpen();


      // Sleep for human viewabillity??
      std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));


      // Take lowest cost node in the open list
      cout << "Finding lowest node in open list" << endl;
      currentNode = FindLowestNode();
      printf("x %d \t y %d \t cost %d", currentNode->getPosition().x, currentNode->getPosition().y, currentNode->getCost());


      // Add to closed list
      cout << "Adding current node to closed list" << endl;
      closed.push_back(currentNode);


      // Check if this is the goal node
      cout << "Checking whether this node is goal node or not" << endl;
      if (currentNode->getPosition().x == targetPt.x && currentNode->getPosition().y == targetPt.y)
	{
	  // Goal reached
	  cout << "Goal node reached.." << endl;
	  break;
	}
      else
	{
	  cout << "\t Not goal node" << endl;
	}

      
      // get neighbours
      cout << "Finding neighbour nodes to this one" << endl;
      auto vneigh = this->getNeighbours(currentNode);

      cout << "Analyzing neighbour nodes" << endl;
      for (auto v : vneigh)
	{
	  // If its blocked or already in closed list, skip it
	  if (v.getCost() > 100000 || ClosedContains(v))
	    continue;

	  // calculate path cost
	  auto pathcost = currentNode->getCost() + 10;
	  // If new cost is lower than current, update to this path
	  // Or if the box hasn't been checked yet, update to this path
	  if (pathcost < v.getCost() || OpenContains(v) == false)
	    {
	      v.setG(pathcost);
	      v.setParent(currentNode);

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

  cout << "=========== Dumpimg list ============ " << endl;
  for (auto v : open)
    {
      printf ("[%d] POS : %d , %d \t Cost = %d \n", counter, v->getPosition().x, v->getPosition().y, v->getCost());
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



GraphNode* AStar::FindLowestNode()
{
  int index, counter, low;
  counter = 0;
  index = 0;
  low = 100000;
  GraphNode* n;
  GraphNode* res;

  for (auto it = open.begin(); it != open.end(); it++)
    {
      n = *it;

      printf("Current cost : %d \t Current low : %d \n", n->getCost(), low);

      if (n->getCost() < low)
	{
	  index = counter;
	  low = n->getCost();
	}
   
      counter++;
    }

  
  res = *(open.begin() + index);


  printf("\t Found [Node] X : %d \t Y : %d \t Cost : %d \n", res->getPosition().x, res->getPosition().y, res->getCost());
  return n;

   //   open.erase(open.begin() + i);
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
  for (auto i = 0; i < ymax; i++)
    {
      delete graph[i];
    }

  delete graph;
}
