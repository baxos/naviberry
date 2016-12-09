#ifndef COMMUNICATIONS__HPP
#define COMMUNICATIONS__HPP

#include <iostream>
#include <cstdint>
#include <ctime>
#include <bitset>
#include <vector>
#include <cstring>
#include <mutex>
#include <memory>
#include <utility>
#include <thread>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
}

#include "./utility.hpp"
#include "./buffer.hpp"

namespace Naviberry {

  struct NetPacketCore {
    uint32_t size;
    uint32_t type;
    uint32_t time;
  };

  struct Netpacket{
    NetPacketCore core;
    std::string time_str;
    std::vector<uint8_t> data;
  };



  // Public functions..
  uint32_t ConstructTimeUint32();
  void SearchForPackets();


  class NetPacket
  {
  private:
    std::vector<uint8_t> data;
  public:
    struct NetPacketCore info;
    uint32_t getSize() { return info.size; };
    uint32_t getType() { return info.type; };
    uint32_t getTime() { return info.time; };
    uint8_t* getData() { return data.data(); };
    uint32_t getDataSize() { return data.size(); };
    void setSize(size_t _size)
    {
      data.reserve(_size);
      info.size = _size;
    }
    void setType(uint32_t _type)
    {
      info.type = _type;
    }
    void setTime(uint32_t _time)
    {
      info.time = _time;
    }
  };


  class Communications
  {
  private:
    std::string ip_address;
    int32_t port;
    int32_t sockfd;
    int32_t confd;
    struct sockaddr_in server_addr;
    bool ready_to_connect;
    bool connected;
    std::vector<Netpacket> income_packets;
    std::mutex read_mutex;
    static const uint32_t MINIMUM_PACKET_SIZE = 12;
    std::unique_ptr<Naviberry::Buffer> buffer;
  public:
    enum class Types { System = 0 , Text = 170 };
    ~Communications();
    Communications();
    Communications(std::string _ip, int32_t _port);
    void DumpBuffer();
    void Connect();
    void Read();
    void StartRead();
    void ConnectTo(std::string _ip, int32_t _port);
    void WriteText(std::string _data);
    void WriteData(uint8_t *_data, size_t _length);
    bool isConnected();
    bool CreateServer();
    bool AwaitConnection();
    void AwaitConnectionNonBlock();
    size_t getPacketCount();
    std::vector<Netpacket> PopPackets();
  };
  

}

#endif
