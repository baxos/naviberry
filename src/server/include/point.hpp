/**
  * @file point.hpp
  * @author Jan Emil Bacher
  * @date 2016
  * @brief This class is abstraction of a Point
  **/

#ifndef POINT__HPP
#define POINT__HHPP

#include <cstdint>


/**
  * @class Point
  * @brief Used for Math, MapHandling etc.
  * 
  *
  * This class is an abstraction of the 'Point'. This is used in maphandling and navigation
  **/
class Point
{
public:
  int32_t x;
  int32_t y;
  Point();
  Point(int32_t _x, int32_t _y);


  Point operator+ (const Point &pt_b)
  {
    Point newPt;
    newPt.x = this->x + pt_b.x;
    newPt.y = this->y + pt_b.y;
    return newPt;
  }

  Point operator- (const Point &pt_b)
  {
    Point newPt;
    newPt.x = this->x - pt_b.x;
    newPt.y = this->y - pt_b.y;
    return newPt;
  }
};


#endif
