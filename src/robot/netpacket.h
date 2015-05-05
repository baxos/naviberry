#ifndef __NETPACKET__H__
#define __NETPACKET__H__

struct netPacket
{
  uint8_t secCreated;
  uint8_t msCreated;
  uint8_t packSender;
  uint8_t packReciever;
  uint8_t dataSize;
  char* data;
};


struct netPacket * createNetPacket(char * data, int sender, int reciever);

#endif
