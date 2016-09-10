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

  uint8_t* buff_start;
  uint8_t* buff_end;
  uint8_t* buff_head;
  uint8_t* buff_tail;

  uint32_t data_amount;
  uint32_t buffer_size;
  uint32_t freeSpace();
public:
  NaviBuffer(uint32_t _buffer_size);
  ~NaviBuffer();
  void Reset();
  void Add(uint8_t* val, size_t size);
  void RemoveAt(int _startIndex, int _size);
  bool isFull();
  bool isEmpty();
  uint8_t* getData() { return data; };
  uint32_t getSize();
  uint32_t getFreeSpace();
};

#endif
