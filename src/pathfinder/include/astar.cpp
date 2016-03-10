#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <climits>
#include "./astar.hpp"

#define SLEEP_DURATION 1 // 1000 ms / 1 sec


extern bool verboseView;

AStar::AStar(vector< vector<uint8_t> > *map, int maxX, int maxY, Point* _target, Point* _start)
{
  // Initialize graph
  int x,y;
  y=0;
  x=0;


  // Set flags
  isReady = false;
  goalReached = false;
  

  this->targetPt.x = _target->x;
  this->targetPt.y = _target->y;

  this->startPt.x = _start->x;
  this->startPt.y= _start->y;

  cout << "Starting to analyze data" << std::endl;
  cout << "Pool size : " << maxX * maxY << std::endl;
  printf("Start pt  : { %d , %d } \n", startPt.x, startPt.y);
  printf("Target pt : { %d , %d } \n", targetPt.x, targetPt.y);


  for (auto it = map->begin(); it != map->end(); it++)
    {
      for (auto jt = it->begin(); jt != it->end(); jt++)
	{
	  Point currentPt;
	  currentPt.x = x;
	  currentPt.y = y;
	  graph[x][y].setPosition(x, y);
	  
	  if (*jt == 1)
	    {
	      // this unit is blocked
	      graph[x][y].setWalkable(false);
	      graph[x][y].setH(99999);
	    }
	  else
	    {
	      graph[x][y].setH(this->ManhattenCost(targetPt, currentPt));
              graph[x][y].setWalkable(true);
	    }
	      x++;
	    }

	  xmax = x;
	  x= 0;
	  y++;
	}
      ymax = y;
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

list<Point> AStar::getNeighbours(Point pt)
{
  list<Point> result;
  Point newPt = pt;


  bool flowProblems = false;


  if (newPt.x > xmax || newPt.x < 0)
    {
      cout << (int) newPt.x << " > " << (int) xmax << "\t = " << (bool) (newPt.x > xmax)  <<endl;
      cout << (int) newPt.x << " < " << (int) 0 <<    "\t = " << (bool) (newPt.x < 0)    <<endl;
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
      newPt.y = pt.y-1;
      newPt.x = pt.x;
      if (newPt.y >= 0)
	{	  
	  if (verboseView)
	    printf("Adding [%d , %d] \n", newPt.x, newPt.y);
	  result.push_back(newPt);
	}
      
      // get lower
      newPt.y = pt.y + 1;
      newPt.x = pt.x;
      if (newPt.y < ymax-1)
	{
	  if (verboseView)
	    printf("Adding [%d , %d] \n", newPt.x ,newPt.y);
	  result.push_back(newPt);
	}
      
      // get left
      newPt.y = pt.y;
      newPt.x = pt.x - 1;
      if (newPt.x >= 0)
	{
	  if (verboseView)
	    printf("Adding [%d , %d] \n", newPt.x ,newPt.y);
	  result.push_back(newPt);
	}
      
      
      // get right
      newPt.y = pt.y;
      newPt.x = pt.x + 1;
      if ( (newPt.x <= xmax-1) )
	{
	  if (verboseView)
	    printf("Adding [%d , %d] \n", newPt.x ,newPt.y);
	  result.push_back(newPt);
	}
    }

  return result;
}


int AStar::getTotalGraphCount()
{
  return ymax * xmax;
}


void AStar::Step()
{
  // Check whether we are started or we have erache goal
  if (open.size () == 0 && goalReached == false)
    {
      // Initialize
      auto currentNode = graph[startPt.x][startPt.x];
      currentNode.setG(0);
      currentNode.setH(0);
      open.push_back(currentNode.getPosition());
    }
  else
    {
      auto currPt = FindLowestNode();
      auto currentNode = graph[currPt.x][currPt.y];

      if (verboseView)
	{
	  printf("[CURRENT_NODE] [X,Y] { %d , %d } | Cost = %d \n", currentNode.getPosition().x, currentNode.getPosition().y, currentNode.getCost());
	}

      // Let 
      checkedPoints.push_back(currentNode.getPosition());
      


      // Add to closed list
      //cout << "Adding current node to closed list" << endl;
      closed.push_back(currentNode.getPosition());


      // Check if this is the goal node
      // cout << "Checking whether this node is goal node or not" << endl;
      if (currentNode.getPosition().x == targetPt.x && currentNode.getPosition().y == targetPt.y)
	{
	  // Goal reached
	  cout << "Goal node reached.." << endl;
	  // Construct goal 
	  ConstructGoalRoute(&currentNode);

 
	}


      
      // get neighbours
      // cout << "Finding neighbour nodes to this one" << endl;
      auto vneigh = this->getNeighbours(currPt);

      if (verboseView)
	cout << "Analyzing neighbour nodes" << endl;


      for (auto v : vneigh)
	{

	  // If its blocked or already in closed list, skip it
	  if (ClosedContains(v))
	    continue;



	  // calculate path cost
	  auto pathcost = currentNode.getCost() + 10;
	  // If new cost is lower than current, update to this path
	  // Or if the box hasn't been checked yet, update to this path
	  if (pathcost < graph[v.x][v.y].getCost() || OpenContains(v)== false)
	    {
	      if (verboseView)
		{
		  printf("\t Selected [ %d , %d ] \n", v.x, v.y);
		  printf("\t \t Setting n [ %d , %d ] parent ==> [%d , %d] \n",
			 v.x,
			 v.y,
			 currPt.x,
			 currPt.y
			 );
		}
	      graph[v.x][v.y].setG(pathcost);
	      graph[v.x][v.y].setParent(currPt);

	      if (verboseView)
		printf("\t \t Node [%d ,%d] parent = { %d, %d} \n",
		       v.x,
		       v.y,
		       graph[v.x][v.y].parent.x,
		       graph[v.x][v.y].parent.y
		       );


	      // Add to open, if not already there
	      if (OpenContains(v) == false)
		{
		  open.push_back(v);
		}
	    }
	  
	}

      // repeat


    }
}

void AStar::Start()
{

      // Initialize
      auto currentNode = graph[startPt.x][startPt.x];
      currentNode.setG(0);
      currentNode.setH(0);
      open.push_back(startPt);

      // add to open..

  while (open.size() > 0)
{
      auto currPt = FindLowestNode();
      auto currentNode = graph[currPt.x][currPt.y];

      if (verboseView)
	printf("[CURRENT_NODE] [X,Y] { %d , %d } | Cost = %d \n", currentNode.getPosition().x, currentNode.getPosition().y, currentNode.getCost());


      // Let 
      checkedPoints.push_back(currentNode.getPosition());
      


      // Add to closed list
      //cout << "Adding current node to closed list" << endl;
      closed.push_back(currentNode.getPosition());


      // Check if this is the goal node
      // cout << "Checking whether this node is goal node or not" << endl;
      if (currentNode.getPosition().x == targetPt.x && currentNode.getPosition().y == targetPt.y)
	{
	  // Goal reached
	  cout << "Goal node reached.." << endl;
	  // Construct goal 
	  ConstructGoalRoute(&currentNode);

	  // jump out of loop
	  break;
 
	}


      
      // get neighbours
      // cout << "Finding neighbour nodes to this one" << endl;
      auto vneigh = this->getNeighbours(currPt);

      if (verboseView)
	cout << "Analyzing neighbour nodes" << endl;


      for (auto v : vneigh)
	{

	  // If its blocked or already in closed list, skip it
	  if (ClosedContains(v))
	    continue;



	  // calculate path cost
	  auto pathcost = currentNode.getCost() + 10;
	  // If new cost is lower than current, update to this path
	  // Or if the box hasn't been checked yet, update to this path
	  if (pathcost < graph[v.x][v.y].getCost() || OpenContains(v)== false)
	    {
	      if (verboseView)
		{
		  printf("\t Selected [ %d , %d ] \n", v.x, v.y);
		  printf("\t \t Setting n [ %d , %d ] parent ==> [%d , %d] \n",
			 v.x,
			 v.y,
			 currPt.x,
			 currPt.y
			 );
		}
	      graph[v.x][v.y].setG(pathcost);
	      graph[v.x][v.y].setParent(currPt);

	      if (verboseView)
		printf("\t \t Node [%d ,%d] parent = { %d, %d} \n",
		       v.x,
		       v.y,
		       graph[v.x][v.y].parent.x,
		       graph[v.x][v.y].parent.y
		       );


	      // Add to open, if not already there
	      if (OpenContains(v) == false)
		{
		  open.push_back(v);
		}
	    }
	  
	} 
 }
}

void AStar::PrintOpen()
{
  auto counter = 0;

  cout << "=========== Dumpimg list ============ " << endl;
  for (auto v : open)
    {
      printf("[%d] : [x, y] \t [ %d , %d ] \n", counter, v.x, v.y);
      counter++;
    }
}

bool AStar::OpenContains(Point g)
{
  for ( auto& v : open)
    {
      if (v == g)
	return true;
    }

  return false;
}

bool AStar::ClosedContains(Point g)
{
  for (auto& v : closed)
    {
      if (v == g)
	return true;
    }

  return false;
}



Point AStar::FindLowestNode()
{
  // Scan through all elements
  int lowest = INT_MAX-1;
  Point result;


  for (const auto& p : open)
    {
      //      printf("Coordinate [ %d , %d ] cost = %d \n", p.x, p.y, graph[p.x][p.y].getCost());

      if (graph[p.x][p.y].getCost() < lowest)
	{
	  lowest = graph[p.x][p.y].getCost();
	  result.x = p.x;
	  result.y = p.y;
	}
    }

  
  // Remove p from open
  for (auto it = open.begin(); it != open.end(); it++)
    {
      if ( *it == result)
	{
	  open.erase(it);
	  break;
	}
    }
  
  //  printf("\t Found [Node] X : %d \t Y : %d \t Cost : %d \n", result.x, result.y, 255);
  return result;
}



list<Point> AStar::getGoalRoute()
{
  if (goalReached)
    {
      return goalRoute;
    }
  else
    {
      throw ;
    }
}

void AStar::ConstructGoalRoute(GraphNode* goalNode)
{
  vector<Point> route;
  GraphNode* currentNode = goalNode;

  cout << "Constructing goal route, by reversing route" << endl;
  printf("============================================== \n");


  while (currentNode->getPosition() != startPt)
    {
      Point currPt;
      currPt.x = currentNode->getPosition().x;
      currPt.y = currentNode->getPosition().y;
      Point parPt;
      parPt.x = currentNode->parent.x;
      parPt.y = currentNode->parent.y;


      if (verboseView)
	printf("X: %d \t Y: %d \n", currentNode->getPosition().x, currentNode->getPosition().y);

      currentNode = &graph[parPt.x][parPt.y];

      // copy to list for outsiders
      goalRoute.push_back(currPt);
    }

  printf("================ done ============== \n");


  // Copy to list for outsiders can reach it
  goalReached = true;
}

bool AStar::isGoalReached()
{
  return goalReached;
}

list<Point> AStar::getCheckedBoxes()
{
  return checkedPoints;
}


