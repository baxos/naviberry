/* filename    : cbuffer.hpp
   author      : J.E.Bacher
   Description : Circular buffer header class
*/
#ifndef CBUFFER__HPP
#define CBUFFER__HPP

#include<cstdint>
#include "./naviberryio.hpp"


class CircularBuffer
{
private:
  uint8_t* data;
  uint32_t buff_size;
  uint32_t start_pt;
  uint32_t end_pt;
  uint32_t freeSpace();
public:
  CircularBuffer(uint32_t buffer_size);
  ~CircularBuffer();
  void Add(uint8_t* val, uint32_t size);
  void Remove(int size);
  // gets
  uint32_t getStartPt() { return start_pt; };
  uint32_t getEndPt()  { return end_pt;   };
  uint32_t getBufferSize() { return buff_size; };
  uint8_t* getData() { return data; };
  // sets
};

#endif
