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


CircularBuffer::CircularBuffer(uint32_t buffer_size)
{
  // Set start and end pointers
  start_pt = 0;
  end_pt   = 0;

  // Allocate memory
  buff_size = buffer_size;
  data = new uint8_t[buff_size];
}

CircularBuffer::~CircularBuffer()
{
  // Free memory
  delete data;
}

uint32_t CircularBuffer::freeSpace()
{
  if (debugFlag)
    {
      print_msg("CircularBuffer::freeSpace() \t Call()");
    }
  auto freespace = 0;

  std::cout << " == " << start_pt << " \t " << end_pt << " \t " << buff_size << std::endl;

  if (start_pt > end_pt)
    {
      print_msg("start_pt > end_pt");
      freespace = buff_size - (start_pt - end_pt);
    }
  else if (start_pt < end_pt)
    {
      print_msg("start_pt < end_pt");
      freespace = buff_size - (end_pt - start_pt);
    }
  else if (start_pt == end_pt)
    {
      print_msg("start_pt == end_pt");
      freespace = buff_size;
    }

  if (debugFlag)
    {
      print_msg("CircularBuffer::freeSpace() \t Exit()");
    }

  return freespace;
}

void CircularBuffer::Add(uint8_t* val, uint32_t size)
{
  // Check if we have enough free space in buffer
  // if not print error
  if ( debugFlag)
    {
      print_msg("CircularBuffer::Add() \t Call()");
    }
  if (freeSpace() > size)
    {
      // If we dont exceed array max
      // We can copy it directly
      if ( end_pt + size < buff_size )
	{
	  print_msg("end_pt + size < buff_size \t start memcpy");
	  std::memcpy(&data[end_pt], &val, size * sizeof(uint8_t));
	  end_pt += size;
	}
      else
	{
	  auto first_part_size = buff_size - end_pt;
	  auto second_part_size = size - first_part_size;
	  
	  print_msg("Splitting data, \t start memcpy");
	  std::memcpy(&data[end_pt], &val, first_part_size);
	  end_pt = 0;
	  std::memcpy(&data[end_pt], &val[first_part_size], second_part_size);
	  end_pt += second_part_size;
	}


      print_msg("Pointers : ");
      std::cout << "end_pt = " << end_pt << "\t start_pt  " << start_pt <<  "\t size = " << size << std::endl;
      std::cout << "buffer size = " << buff_size << std::endl;
    }
  else
    {
      print_error("Circular buffer is out of memory!");
    }
}

void CircularBuffer::Remove(int size)
{
  // If we can do, without cross border
  if ( start_pt + size < buff_size)
    {
      // Zero-set memory
      std::memset(&data[start_pt], 0, size);
      // Set new index
      start_pt = start_pt + size; 
    }
  else
    {
      auto first_part = buff_size - start_pt;
      auto second_part = size - first_part;
      
      // Zero-set first part
      std::memset(&data[start_pt], 0, first_part);
      start_pt = 0;
      // Zero-set second part
      std::memset(&data[start_pt], 0, second_part);
      // Set new index
      start_pt = second_part;
      
    }
}
