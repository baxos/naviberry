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
  uint8_t* pointer_head;
  uint8_t* pointer_tail;
  uint32_t buffer_size;
  uint16_t block_size;
  uint16_t blocks;
  bool isFull;
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

  uint32_t getBufferSize() { return buffer_size; };
  uint8_t* getData() { return data; };
  // sets
};

#endif
