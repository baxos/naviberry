#include <iostream>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <cstdio>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
}
#include "./communications.hpp"




uint32_t ConstructTimeUint32()
{
  uint32_t result = 0;
  std::time_t t = std::time(NULL);
  std::tm *localtm = std::localtime(&t);
  
  uint8_t temp = 0;
  // hour..
  temp = localtm->tm_hour;
  result = result | ( temp << 24 );
  // minutes ..
  temp = localtm->tm_min;
  result = result | ( temp << 16 );
  // seconds ..
  temp = localtm->tm_sec;
  result = result | ( temp << 8 );
  
  
  // for debug
  // std::biset<32> test_view (temp);
  // std::cout << "Time info : " << test_view << std::endl;
  
  
  return result;
}



Naviberry::Communications::Communications()
{
  // Init comm class
  this->port = 2000;
  this->ip_address = "127.0.0.1";

  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_port = htons(this->port);
  this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  this->buffer = make_unique<Naviberry::Buffer> (10000);

}

Naviberry::Communications::~Communications()
{
  if (this->connected)
    {
      close(this->confd);
    }
}

std::vector<Naviberry::NetPacket> Naviberry::Communications::PopPackets()
{
  // Lock income_packets
  // Copy the data from it to a new vector and delete it from original

  std::vector<Naviberry::NetPacket> result;

  read_mutex.lock();
  
  std::move(this->income_packets.begin(), this->income_packets.end(), std::back_inserter(result));
  

  read_mutex.unlock();

  return result;
}
    
bool Naviberry::Communications::CreateServer()
{
  auto error_flag = false;
  // Create socket
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  this->port = 2000;
  if (sockfd == -1)
    {
      // Error
      printf("Failed to create socket! \n");
      error_flag = true;
      goto exit;
    }
  
  // Bind to socket
  if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
      // Error
      error_flag = true;
      goto exit;
    }
  
  // Then API  call AwaitConnection should be used or the Nonblock
  
 exit:
  if (error_flag)
    {
      return false;
    }
  

  return true;
}

bool Naviberry::Communications::AwaitConnection()
{
  // Blocks until there is a client connection OR timeout is triggered
  if ( listen(sockfd, 5) == -1)
    {
      return false;
    }

  // Accept client
  this->confd = accept(sockfd, NULL, NULL);
  if ( this->confd == -1 )
    {
      return false;
    }
  
  // If we are here, we can tell its successful.
  this->connected = true;
  return true;
}

bool Naviberry::Communications::isConnected()
{
  return this->connected;
}

void Naviberry::Communications::StartRead()
{
  std::thread read_thread ( &Naviberry::Communications::Read, this );
  read_thread.detach();
  printf("[+] read thread spawned. \n");
}

void Naviberry::Communications::DumpBuffer()
{
  for (int i = 0; i < this->buffer->getIndex(); i++)
    {
      printf(" %u \t " , *(this->buffer->getDataPointer() + i));
      if ( i % 16 == 0 && i != 0)
	{
	  std::cout << std::endl;
	}
    }

  printf (" \n ");
}

void Naviberry::Communications::Read()
{
  //  const int32_t MAX_READ = 4096;
  //  const auto NAVI_MAGIC_NR = 004E41564900
  //  const auto IVAN_MAGIC_NR = 004956414E00 

  printf("Entering network read loop. \n");
  while (this->isConnected())
    {
      std::vector<int32_t> start_blocks;
      std::vector<int32_t> end_blocks;
      std::vector<std::pair<int32_t, int32_t>> paired_blocks;
      std::vector<std::pair<uint32_t, uint32_t>> delete_blocks;

      auto n = recv(this->confd, this->buffer->getDataPointer() + this->buffer->getIndex(), 1024, 0);
      std::cout << "[+] Read Loop " << std::endl
		<< "Recieved : " << n << " bytes." << std::endl;
      
      //      this->DumpBuffer();

      if (n > 0)
	{
	  this->buffer->IncreaseIndex(n);
	}
      else if ( n == -1 ) 
	{
	  // Error
	}

      
      // Iterate through memory, check for { 0, 0, N, A, V, I, 0, 0 }
      //                                or { 0, 0, I, V, A, N, 0, 0 }
      for (int i = 0; i < this->buffer->getIndex() - 7; i++)
	{      
	  auto data = this->buffer->getDataPointer();
	  
	  if (data[i] == 0 && data[i+1] == 0 && data[i+2] == 'N' && data[i+3] == 'A' && data[i+4] == 'V' && data[i+5] == 'I' && data[i+6] == 0 && data[i+7] == 0)
	    {
	      // We have start of block
	      // Index at i + 8
	      auto index = i + 8;
	      start_blocks.push_back(index);
	    }
	  if (data[0] == 0 && data[i+1] == 0 && data[i+2] == 'I' && data[i+3] == 'V' && data[i+4] == 'A' && data[i+5] == 'N' && data[i+6] == 0 && data[i+7] == 0)
	    {
	      // We have end block
	      // Index at i
	      auto index = i ;
	      end_blocks.push_back(index);
	    }
	}

      // For debug??
      printf (" Found %zu start signatures and %zu end signatures. \n", start_blocks.size(), end_blocks.size());

      // Check for pairs
      for (size_t i = 0; i < end_blocks.size(); i++)
	{
	  if ( i >= start_blocks.size() )
	    {
	      break;
	    }

	  auto paired_block = std::make_pair(start_blocks[i], end_blocks[i]);
	  paired_blocks.push_back(paired_block);
	}


      // If there are pairs found
      if ( paired_blocks.empty() != true)
	{
	  this->read_mutex.lock();
	  for ( auto p : paired_blocks)
	    {
	      if (p.first > p.second )
		{
		  continue;
		}
	      printf("Found packet! \n");
	      
	      size_t expected_size = p.second - p.first;
	      
	      std::vector<uint8_t> packet_block;
	      packet_block.resize(expected_size);
	      auto data_from = this->buffer->getDataPointer();
	      auto data_to = packet_block.data();

	      printf("Expected packet size : %zu \n", expected_size);


	      printf("Copying data from network buffer to a temporary buffer. \n");
	      std::memcpy(data_from, data_to, expected_size);	      
	      
	      Naviberry::NetPacketCore packet_core;
	      printf("Copying data from temporary buffer to Packet Core  Structure. \n");
	      std::memset(&packet_core, 0, sizeof(packet_core));
	      std::memcpy(data_to, &packet_core, sizeof(packet_core));
	      
	      printf("[INFO]  \n core.size : %u \n core.type : %u \n core.Time : %u \n ", packet_core.size, packet_core.type, packet_core.time);

	      Naviberry::NetPacket packet;
	      packet.setSize(packet_core.size);
	      packet.setType(packet_core.type);
	      packet.setTime(packet_core.time);
	      
	      printf("Copying data to newly created packet structure. \n");
	      std::memcpy(data_to + this->MINIMUM_PACKET_SIZE, packet.getData(), packet.getSize());
	      // We have a packet.. add it to queue
	      income_packets.push_back(packet);
	      
	      // Release bytes from buffer..
	      delete_blocks.push_back( std::make_pair( (uint32_t) p.first - 8, (uint32_t) p.second + 8));
	      printf("\t [Done] \n");
	    }				 
	  
	  
	  // Delete
	  for ( auto e : delete_blocks )
	    {
	      printf("Removing old data from network buffer. \n");
	      this->buffer->Delete(e.first, e.second);
	      printf("\t [Done] \n");
	    }

	  this->read_mutex.unlock();
	}
      
      printf("[Done] : Network read iteration. \n"); 
    }
  // Disconnected..
  printf("Exiting network read loop. \n");
}


