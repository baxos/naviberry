#ifndef BUFFER__HPP
#define BUFFER__HPP

#include <cstdint>
#include <cstddef>
#include <vector>

namespace Naviberry {


  class Buffer
  {
    std::vector<uint8_t> data;
    int32_t index;
    size_t maxSize;
    size_t dataUsed;
  public:
    Buffer(size_t _size);
    void Reset();
    void Delete(uint32_t _start, uint32_t _end);
    void IncreaseIndex(int32_t _val);
    bool isFull();
    uint8_t* getDataPointer();
    int32_t getIndex();
    int32_t getMemoryUsed();
  };

}


#endif
