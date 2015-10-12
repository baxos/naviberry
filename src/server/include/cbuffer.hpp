/* filename    : cbuffer.hpp
   author      : J.E.Bacher
   Description : Circular buffer header class
*/
#ifndef CBUFFER__HPP
#define CBUFFER__HPP

#include<cstdint>
#include<map>
#include "./naviberryio.hpp"


class NaviBuffer
{
private:
  uint8_t* data;
  uint32_t buffer_size;
  uint32_t start_pt;
  uint32_t end_pt;
  uint16_t block_size;
  uint16_t blocks;

  std::map <uint16_t, bool> blockmap;
  uint32_t freeSpace();
public:
  NaviBuffer(uint32_t _buffer_size);
  ~NaviBuffer();
  void Add(uint8_t* val, uint32_t size);
  void Remove(int size);
  // gets
  uint32_t getStartPt() { return start_pt; };
  uint32_t getEndPt()  { return end_pt;   };
  uint32_t getBufferSize() { return buffer_size; };
  uint8_t* getData() { return data; };
  // sets
};

#endif
