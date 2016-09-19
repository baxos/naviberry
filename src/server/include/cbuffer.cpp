/* filname     : cbuffer.cpp
   author      : J.E.Bacher
   description : Circular buffer code class
*/
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include "./naviberryio.hpp"
#include "./cbuffer.hpp"

extern bool debugFlag;


NaviBuffer::NaviBuffer(uint32_t _buffer_size)
{
  std::cout << "Setting up circular buffer of " << _buffer_size << " bytes." << std::endl;

  // Set start and end pointers
  this->data = new uint8_t[_buffer_size];
  this->data_amount = 0; // amount of bytes stored in buffer
  this->buffer_size = _buffer_size;
  this->buff_start = this->data;
  this->buff_end = this->data + (_buffer_size-1);
  this->buff_head = this->data;
  this->buff_tail = this->data;

  int val = 0 ;
  printf("Memcpy directly this->data. \n");
  std::memcpy(this->data, &val, sizeof(val));
  printf("Memcpy using pointer to array, this->buff_head.\n");
  std::memcpy(this->buff_head, &val, sizeof(val));


  printf("Address of data field : \t 0X%x8 \n", this->data);
  printf("Address of head       : \t 0X%x8 \n", this->buff_head);
}

NaviBuffer::~NaviBuffer()
{
  // Free memory
  delete[] data;
}


void NaviBuffer::Add(uint8_t* _val, size_t  _size)
{
  // Check if we have enough free space in buffer
  // if not print error
  if ( debugFlag)
    {
      print_msg("NaviBuffer::Add() \t Call()");
    }
  
  // Check if buffer is full
  if (this->isFull())
    {
      print_error("Network buffer is full..");
      return;
    }

  // Check if there is enough space avaiable..
  if (this->getFreeSpace() < _size)
    {
      print_warning("Can't fit data in buffer, resets it.");
      printf("Address of head : 0X%x8 \n", this->buff_head);
      printf("Address of tail : 0X%x8 \n", this->buff_tail);
      printf("Space left      : %d \n", this->getFreeSpace());
      printf("Data size       : %zu \n", _size);
      this->Reset();
      print_msg("OK");
    }

  // Lets check if we well exceed the boundary of the buffer
  if (this->buff_head + _size >= this->buff_end)
    {
      // We need to split the data
      
      // first we fill up from our current position to end of buffer
      auto distance_to_end = this->buff_end - this->buff_head;
      std::memcpy(this->buff_head, _val, distance_to_end);
      this->buff_head = this->buff_start;

      // Now we fill up rest
      auto distance_rest = _size - distance_to_end;
      std::memcpy(this->buff_head, _val + distance_to_end, distance_rest);
      this->buff_head += distance_rest;
      
    }
  else
    {
      // We can simply make one write
      std::memcpy(this->buff_head, _val, _size);
      this->buff_head += _size;
    }

}

void NaviBuffer::Reset()
{
  // Zero set pointers, and memory..
  this->buff_head = this->buff_start;
  this->buff_tail = this->buff_start;

  std::memset(this->buff_start, 0, this->buffer_size);
}

bool NaviBuffer::isFull()
{
  if (this->buff_head + 1 == this->buff_tail)
    {
      return true;
    }
  return false;
}

bool NaviBuffer::isEmpty()
{
  if (this->buff_head == this->buff_tail)
    {
      return true;
    }

  return false;
}

void NaviBuffer::RemoveAt(int _startIndex, int _size)
{
  if (debugFlag)
    {
      print_msg("NaviBufffer::RemoveAt() \t Call()");
      std::cout << "_startIndex = " << _startIndex << "\t _size = " << _size << std::endl;
    }

  if ( this->buff_tail == this->buff_start + _startIndex)
    {
      this->buff_tail += _startIndex;
    }

  // zero set memory at specified location
  std::memset(&data[_startIndex], 0, _size);
}

uint32_t NaviBuffer::getSize()
{
  // If header is greater than the tail we can simply say size of data  = head - tail
  if ( this->buff_head >= this->buff_tail)
    {
      auto distance = this->buff_head - this->buff_tail;
      return (uint32_t) distance;
    }

  // if the tail is greater than the head, firstly we need to find how many elements there is between the tail and the buffer_max_position
  // secondly we need to find how many elements there are from buffer_start to buffer_head

  auto distance_to_end = this->buff_end - this->buff_tail;
  auto distance_to_start = this->buff_head - this->buff_start;
  auto result = distance_to_end  + distance_to_start;
  return (uint32_t) result;
}

uint32_t NaviBuffer::getFreeSpace()
{
  return (uint32_t) (this->buffer_size - this->getSize());
}
