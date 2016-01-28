#include "./point.hpp"
#include <cstdint>


Point::Point()
{
  this->x = 0;
  this->y = 0;
}

Point::Point(int32_t _x, int32_t _y)
{
  // Set to class variables
  this->x = _x;
  this->y = _y;
}
