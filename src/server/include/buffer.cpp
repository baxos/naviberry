#include "buffer.hpp"

#include <iostream>

Naviberry::Buffer::Buffer(size_t _size)
{
  this->maxSize = _size;
  this->index = 0;
  this->data.resize(_size);
}

void Naviberry::Buffer::IncreaseIndex(int32_t _val)
{
  this->index += _val;
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
  printf("Buffer->Delete : Param [ %u , %u ] \n", _start, _end);
  printf("Buffer.size()  : %zu \n", this->data.size());
  this->data.erase(data.begin() + _start, data.begin() + _end);
  printf("\t [Done] \n");
}

bool Naviberry::Buffer::isFull()
{
  if (this->data.size() == this->maxSize)
    {
      return true;
    }

  return false;
}

void Naviberry::Buffer::Reset()
{
  this->data.erase(this->data.begin(), this->data.end());
  this->data.reserve(this->maxSize);
}
