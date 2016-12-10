#include "buffer.hpp"

#include <iostream>
#include <algorithm>

Naviberry::Buffer::Buffer(size_t _size)
{
  this->maxSize = _size;
  this->index = 0;
  this->data.resize(_size);
  this->dataUsed = 0;
}

void Naviberry::Buffer::IncreaseIndex(int32_t _val)
{
  this->index += _val;
  this->dataUsed += _val;

  if ( this->getMemoryUsed() >= 90 )
    {
      std::cout << "[-] Buffer almost full! " << (100 - this->getMemoryUsed()) << " % left."
		<<std::endl;
      this->index = 0;
    }
}

uint8_t* Naviberry::Buffer::getDataPointer()
{
  return this->data.data();
}

int32_t Naviberry::Buffer::getIndex()
{
  return this->index;
}

void Naviberry::Buffer::Delete(uint32_t _start, uint32_t _end)
{
  // Just zero-set memory, until we start over from index [0]

  //  printf("Buffer->Delete : Param [ %u , %u ] \n", _start, _end);
  //  printf("Buffer.size()  : %zu \n", this->data.size());
  //  this->data.erase(data.begin() + _start, data.begin() + _end);
  //  auto length = _end - _start;
  //  this->dataUsed -= length;
  //  printf("\t [Done] \n");


  std::fill ( this->data.begin() + _start, this->data.begin() + _end, 0);
}

bool Naviberry::Buffer::isFull()
{
  if (this->data.size() == this->maxSize)
    {
      return true;
    }

  return false;
}

int32_t Naviberry::Buffer::getMemoryUsed()
{
  if ( this->dataUsed == 0 )
    {
      return this->dataUsed;
    }


  float f = (  (float) this->dataUsed / (float) this->data.size() ) * 100;
  
  return (int) f;
}

void Naviberry::Buffer::Reset()
{
  this->data.erase(this->data.begin(), this->data.end());
  this->data.reserve(this->maxSize);
}
