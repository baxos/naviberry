/* filename    : cbuffer.hpp
   author      : J.E.Bacher
   Description : Circular buffer header class
*/
#ifndef CBUFFER__HPP
#define CBUFFER__HPP
#include<cstdint>
#include<map>


/**
 * @class NaviBuffer
 * @brief *** NEEDS REWORK ***
 * 
 *
 * This is the buffer used by the Network class. Only function of this class is to store/delete data.
 **/
class NaviBuffer
{
private:
  uint8_t* data;
  uint32_t buffer_size;
  uint32_t buffer_offset;
  uint32_t current_data_size;
  uint32_t start_pt;
  uint32_t end_pt;
  uint16_t block_size;
  uint16_t blocks;
  std::map <uint16_t, bool> blockmap;
  // Private functions
  uint32_t freeSpace();
  uint16_t ReserveBlock();
  void CopyToBlock(uint8_t* data, uint32_t _size, uint16_t _index);
public:
  NaviBuffer(uint32_t _buffer_size);
  ~NaviBuffer();
  void Add(uint8_t* val, uint32_t size);
  void RemoveAt(int _startIndex, int _size);
  // gets
  uint32_t getStartPt() { return start_pt; };
  uint32_t getEndPt()  { return end_pt;   };
  uint32_t getBufferSize() { return buffer_size; };
  uint32_t getDataSize() { return current_data_size; };
  uint8_t* getData() { return data; };
  // sets
};

#endif
