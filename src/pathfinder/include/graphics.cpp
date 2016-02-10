#include <iostream>
#include "./graphics.hpp"

#define EMPTY_BLOCK 0;
#define WALLED_BLOCK 1;



#define BOX_SIZE 3;

using namespace std;

void Graphics::ConstructImage(vector< vector<uint8_t> > v)
{
  map = v;
}

void Graphics::ClearImage()
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear ( renderer );
}

Graphics::Graphics(SDL_Renderer *_render)
{
  renderer = _render;
}


void Graphics::setColor(Color c)
{
  uint8_t r,g,b;

  switch (c)
    {
    case Color::Black:
      r = 0;
      g = 0;
      b = 0;
      break;
    case Color::Red:
      r = 255;
      g = 0;
      b = 0;
      break;
    case Color::Green:
      r = 0;
      g = 255;
      b = 0;
      break;
    case Color::White:
      r = 255;
      g = 255;
      b = 255;
      break;
    case Color::Blue:
      r = 0;
      g = 0;
      b = 255;
      break;
    case Color::DarkGreen:
      r = 0;
      g = 123;
      b = 0;
      break;
    case Color::Gray:
      r = 133;
      g = 133;
      b = 133;
    }

  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void Graphics::DrawVisited(list<Point> boxes, Color c)
{
  this->setColor(c);

  for (auto p : boxes)
    {
      SDL_Rect rect;
      rect.x = p.x * BOX_SIZE;
      rect.y = p.y * BOX_SIZE;
      rect.h = BOX_SIZE;
      rect.w = BOX_SIZE;

      SDL_RenderFillRect(renderer, &rect);
    }
}

void Graphics::DrawGrid()
{

  // 
  this->setColor(Color::Black);
  
  int x,y;
  y = 0;
  x = 0;
  for (auto it = map.begin(); it != map.end(); it++)
    {
      for (auto jt = it->begin(); jt != it->end(); jt++)
	{
	  SDL_Rect rect;
	  rect.x = x * BOX_SIZE;
	  rect.y = y * BOX_SIZE;
	  rect.h = BOX_SIZE;
	  rect.w = BOX_SIZE;

	  if (*jt == 0)
	    {

	    }
	  else if(*jt == 1)
	    {
	      SDL_RenderFillRect(renderer, &rect);
	    }
	  

	  SDL_RenderDrawRect(renderer, &rect);


	    x++;
	}
    
      x = 0;
      y++;
    }
}


void Graphics::DrawTarget()
{
  this->setColor(Color::Red);
  SDL_Rect rect;
  rect.x = targetX * BOX_SIZE;
  rect.y = targetY * BOX_SIZE;
  rect.h = BOX_SIZE;
  rect.w = BOX_SIZE;

  SDL_RenderFillRect(renderer, &rect);
  
}

void Graphics::DrawStart()
{
  this->setColor(Color::Green);
  SDL_Rect rect;
  rect.x = startX * BOX_SIZE;
  rect.y = startY * BOX_SIZE;
  rect.h = BOX_SIZE;
  rect.w = BOX_SIZE;

  SDL_RenderFillRect(renderer, &rect);
}


void Graphics::setStart(int x, int y)
{
  startX = x;
  startY = y;
}

void Graphics::setTarget(int x, int y)
{
  targetX = x;
  targetY = y;
}






