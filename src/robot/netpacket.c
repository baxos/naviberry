#include <stdint.h>
#include "netpacket.h"
#include <string.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

struct netPacket * createNetPacket(char * data, int sender, int reciever)
{
  struct timespec spec;
  int sec;
  long msec;

  // Get current time
  clock_gettime(CLOCK_REALTIME, &spec);
  sec = spec.tv_sec;
  msec = lround(spec.tv_nsec / 1.0e6);

  // Allocate memory for new struct
  struct netPacket* currentPacket = NULL;
  currentPacket =  malloc(sizeof(struct netPacket) * 1);
  // Set data to new struct
  currentPacket->secCreated = sec;
  currentPacket->msCreated = msec;
  currentPacket->packSender = sender;
  currentPacket->packReciever = reciever;
  currentPacket->dataSize = strlen(data);
  // Allocate new memory to data and set struct pointer to it
  char* buffer = malloc( sizeof(char) * currentPacket->dataSize);
  // Copy data to new memory section
  memcpy(buffer, data, currentPacket->dataSize);
  // Point to it
  currentPacket->data = buffer;

  // Done, return the packet
  return currentPacket;
}
