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
  start_pt = 0;
  end_pt   = 0;
  block_size = 256;
  buffer_size = _buffer_size;

  // If buff size is small, make blck size small too
  if (buffer_size < 256)
    {
      block_size = 8;
    }

  blocks = buffer_size / block_size;

  // Allocate memory
  data = new uint8_t[buffer_size];


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


}

void NaviBuffer::CopyToBlock(uint8_t* _val, uint32_t _size, uint16_t _index)
{
  auto start_position = _index * block_size;
  // Copy the data to the class data container
  std::memcpy(&data[start_position], &_val, _size);
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

  // If the data size is less than a one block, we just reserve that one and copy data
  if (_size < block_size)
    {
      auto blockIndex = ReserveBlock();
      CopyToBlock(&_val, _size, blockIndex);
    }
  else if (_size > block_size)
    {
      auto current_size = _size;
      auto current_index = 0;

      while (current_size > 0)
	{
	  // Reserve block
	  auto blockIndex = ReserveBlock();
	 
	  // Create small pieces of the data
	  uint8_t* block = new uint8_t[block_size];
	  std::memcpy(&_val[current_index], &block, block_size);
	  
	  // Copy to block
	  CopyToBlock(block, _block_size, blockIndex);
	
	  current_size -= block_size;
	  current_index += block_size;
	}
    }
  else
    {
      print_error("Unkown error happened, please refer to the log");
    }
}

void NaviBuffer::Remove(int size)
{
  // Write!
}
