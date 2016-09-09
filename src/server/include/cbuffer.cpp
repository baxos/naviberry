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
  // Set start and end pointers

  this->block_size = 1024;
  this->buffer_size = _buffer_size;
  this->blocks = buffer_size / block_size;

  this->pointer_tail = &this->data[0];
  this->pointer_head = &this->data[buffer_size - 1];
  

  // Allocate memory
  data = new uint8_t[buffer_size];

  // Zero set data
  std::memset(data, 0, buffer_size-1);

  // Make ready block map
  for (auto i = 0; i < blocks; i++)
    {
      // Blocks are simply by ID 0..1..2..3
      blockmap[i] = false;
    }
}

NaviBuffer::~NaviBuffer()
{
  // Free memory
  delete data;
}

uint32_t NaviBuffer::freeSpace()
{
  if (debugFlag)
    {
      print_msg("CircularBuffer::freeSpace() \t Call()");
    }



  return 0;
}

uint16_t NaviBuffer::ReserveBlock()
{
  // Search for empty block, return index

  // If debug is on print function name
  if (debugFlag)
    {
      print_msg("NaviBuffer::ReserveBlock \t Call()");
    }

  // Iterate through blockmap until an empty is found
  for (auto it = 0; it < blocks; it++)
    {
      if (blockmap[it] == false)
	{
     	  std::cout << "Returning index : " << it << std::endl;
	  return it;
	}
    }

  return -1;
}



void NaviBuffer::CopyToBlock(uint8_t* _val, uint32_t _size, uint16_t _index)
{
  // Copy chunk of memory to a block of memory in the class

  // If debug is on print function name
  if (debugFlag)
    {
      print_msg("NaviBuffer::CopyTopBlock \t Call()");
    }

  auto start_position = _index * block_size;
  // Copy the data to the class data container
  std::memcpy(&data[start_position], _val, _size);
  // Update block map
  blockmap[_index] = true;
}



void NaviBuffer::Add(uint8_t* _val, uint32_t _size)
{
  // Check if we have enough free space in buffer
  // if not print error
  if ( debugFlag)
    {
      print_msg("NaviBuffer::Add() \t Call()");
    }


  // Copy data to buffer, update offset
  std::memcpy(this->pointer_tail, &_val[0], _size);
  this->pointer_tail += _size;


  // If buffer_offset is greater than buffer_size
  // reset it to zero or whatever position fit better
  if (this->isFull)
    {
      print_warning("Buffer reset position");
      this->pointer_tail = &this->data[0];
    }

}

void NaviBuffer::RemoveAt(int _startIndex, int _size)
{
  if (debugFlag)
    {
      print_msg("NaviBufffer::RemoveAt() \t Call()");
      std::cout << "_startIndex = " << _startIndex << "\t _size = " << _size << std::endl;
    }

  // zero set memory at specified location
  std::memset(&data[_startIndex], 0, _size);

  // update memory map
  // .. not ready 
}
