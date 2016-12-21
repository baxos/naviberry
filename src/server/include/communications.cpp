#include <iostream>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <sstream>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
}
#include "./communications.hpp"

extern bool debugFlag;

std::string DeconstructDataToStr(std::vector<uint8_t> data)
{
  std::ostringstream oss;
  std::string result;

  for ( auto b : data )
    {
      oss << (char) b;
    }

  result = oss.str();
  return result;
}

std::string DeconstructTimeUint32(uint32_t timebytes)
{
  // Format [ HH : MM : SS : XX }
  uint8_t hh = 0;
  uint8_t mm = 0;
  uint8_t ss = 0;
  std::ostringstream oss;
  std::string result;


  hh = ( (timebytes & 0xFF000000) >> 24 );
  mm = ( (timebytes & 0x00FF0000) >> 16 );
  ss = ( (timebytes & 0x0000FF00) >> 8 );

  oss << (uint) hh << ":" << (uint) mm << ":" <<(uint) ss << ":" << "00";
  result = oss.str();
  return result;
}

uint32_t Naviberry::ConstructTimeUint32()
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

// ================================ Channel class =========================== //

Naviberry::Channels::Channels()
{
  this->idCounter = 0;
  this->storedChannels.reserve(1024);

  this->Create("system");
  this->Create("master");
}

std::string Naviberry::Channels::fromId(uint32_t _id)
{
  for ( auto c : this->storedChannels )
    {
      if ( c.id == _id )
	{
	  return c.name;
	}
    }

  return "NOTVALID";
}

uint32_t Naviberry::Channels::fromName(std::string _name)
{
  for ( auto c : this->storedChannels )
    {
      if ( c.name == _name )
	{
	  return c.id;
	}
    }

  return 9999;
}

bool Naviberry::Channels::Create(std::string _name)
{
  for ( auto c : this->storedChannels )
    {
      if ( c.name == _name )
	{
	  return false;
	}
    }


  ChannelStruct newchannel;
  newchannel.id = this->requestId();
  newchannel.name = _name;
  this->storedChannels.push_back(newchannel);
  return true;
}

uint32_t Naviberry::Channels::requestId()
{
  uint32_t temp = this->idCounter;
  this->idCounter++;
  return temp;
}

// ==================================== Communications class ======================= //

size_t Naviberry::Communications::getPacketCount()
{
  this->read_mutex.lock();
  auto temp = income_packets.size();
  this->read_mutex.unlock();

  return (size_t) temp;
}

Naviberry::Communications::Communications()
{
  const auto NETWORK_BUFFER_SIZE = 1024 * 10;

  // Init comm class
  this->port = 2000;
  this->ip_address = "127.0.0.1";

  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_port = htons(this->port);
  this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  this->buffer = make_unique<Naviberry::Buffer> (NETWORK_BUFFER_SIZE);

}

Naviberry::Communications::~Communications()
{
  if (this->connected)
    {
      close(this->confd);
    }
}

std::vector<Naviberry::Netpacket> Naviberry::Communications::PopPackets()
{
  // Lock income_packets
  // Copy the data from it to a new vector and delete it from original

  std::vector<Naviberry::Netpacket> result;

  read_mutex.lock();

  for ( auto p : this->income_packets )
    {
      Naviberry::Netpacket new_packet;

      new_packet.time_str = p.time_str;
      new_packet.data = p.data;
      new_packet.core.size = p.core.size;
      new_packet.core.type = p.core.type;
      new_packet.core.time = p.core.time;
      new_packet.channel = p.channel;
      result.push_back(new_packet);
    }  

  this->income_packets.clear();

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


void DEBUG_PRINT_V(std::vector<uint8_t> v)
{

  std::cout << "[+] DEBUG ============================== " << std::endl;
  std::cout << "[+] SIZE : " << v.size() << std::endl;
  std::cout << " { " ;
  for ( size_t i = 0; i < v.size(); i++)
    {
      printf(" 0x%X , ", v[i]);
    }

  std::cout << " } " << std::endl << " ========= END ========== " << std::endl;
}

void Naviberry::Communications::WriteText(std::string data)
{
  const char start_bytes[] = { 0, 0, 'N', 'A', 'V', 'I', 0, 0 };
  const char end_bytes[]   = { 0, 0, 'I', 'V', 'A', 'N', 0, 0 };
  Naviberry::Netpacket packet;
  // startbytes = 8, endbytes = 8 , hence 16
  size_t packet_total_size = 16;

  // Core data
  packet.core.size = sizeof(NetPacketCore) + data.length();
  packet.core.type = this->channels.fromName("system");
  packet.core.time = ConstructTimeUint32();

  // packet_size + 12
  packet_total_size += sizeof(NetPacketCore);

  // paket_size + data length
  packet_total_size += data.length();


  // vector write

  // prep vectors
  struct iovec iov[4];
  iov[0].iov_base = (void*) start_bytes;
  iov[0].iov_len  = 8;
  iov[1].iov_base = (void*) &packet.core;
  iov[1].iov_len  = sizeof(NetPacketCore);
  iov[2].iov_base = (void*) data.c_str();
  iov[2].iov_len  = data.length();
  iov[3].iov_base = (void*) end_bytes;
  iov[3].iov_len  = 8;

  // write them out
  size_t data_written = 0;

  data_written = writev(this->confd, iov, 4);
  if ( data_written == packet_total_size )
    {
      std::cout << "[+] Data written successfully." << std::endl;
    }
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

      if ( n == 0 )
	{
	  continue;
	}
      else if ( n < 0)
	{
	  // error handling
	}
      

      //      std::cout << "[+] Read Loop " << std::endl
      //	<< "Recieved : " << n << " bytes." << std::endl;
      
      this->buffer->IncreaseIndex(n);


      // ============= [1]  Find signatures ============== //

      
      // Iterate through memory, check for { 0, 0, N, A, V, I, 0, 0 }
      //                                or { 0, 0, I, V, A, N, 0, 0 }
      for (int i = 0; i < this->buffer->getIndex() - 7; i++)
	{      
	  auto data = this->buffer->getDataPointer();
	  
	  if (data[i] == 0 && data[i+1] == 0 && data[i+2] == 'N' && data[i+3] == 'A' && data[i+4] == 'V' && data[i+5] == 'I' && data[i+6] == 0 && data[i+7] == 0)
	    {
	      // We have start of block
	      // Index at i + 8
	      //	      auto index = i + 8;
	      auto index = i;
	      start_blocks.push_back(index);
	    }
	  if (data[0] == 0 && data[i+1] == 0 && data[i+2] == 'I' && data[i+3] == 'V' && data[i+4] == 'A' && data[i+5] == 'N' && data[i+6] == 0 && data[i+7] == 0)
	    {
	      // We have end block
	      // Index at i
	      auto index = i + 8;
	      end_blocks.push_back(index);
	    }
	}


      // ============ [2]  CONFIRM MATCH ========== // 

      // Check for pairs
      for (size_t i = 0; i < end_blocks.size(); i++)
	{
	  if ( i >= start_blocks.size() )
	    {
	      break;
	    }

	  auto paired_block = std::make_pair(start_blocks[i], end_blocks[i]);
	  if ( paired_block.first < paired_block.second )
	    {
	      paired_blocks.push_back(paired_block);
	    }
	}



      // =========== [3]  ALLOCATE MEMORY  ========== //

      if ( paired_blocks.empty() == false)
	{
	  for ( auto e : paired_blocks )
	    {
	      Naviberry::Netpacket netpacket;

	      // Allocate memory for the expected size and add the structure for ready
	      auto data = this->buffer->getDataPointer() + e.first;	     

	      // Copy packet core information
	      std::memcpy(&netpacket.core, &data[8], sizeof(Naviberry::NetPacketCore));

	      // Write str formatted time format to the advanced packet structure
	      netpacket.time_str = DeconstructTimeUint32(netpacket.core.time);
	      
	      // Copy data to the packet structure
	      size_t data_length = netpacket.core.size - sizeof(Naviberry::NetPacketCore);
	      
	      // Make room for it
	      netpacket.data.resize(data_length);
	      
	      // Copy it
	      std::memcpy(netpacket.data.data(), &data[20], data_length);

	      netpacket.channel = netpacket.core.type;

	      if ( debugFlag )
		{

		  std::cout << std::endl;
		  std::cout << "[+] Packet Information :" << std::endl
			    << "\t core.size = " << netpacket.core.size << std::endl
			    << "\t core.type = " << netpacket.core.type << std::endl
			    << "\t data.size = " << netpacket.data.size() << std::endl
			    << "\t time      = " << netpacket.time_str << std::endl 
			    << "data.value   = ";

		  for ( auto c : netpacket.data )
		    {
		      std::cout << (char) c ;
		    }
		  std::cout << std::endl << std::endl;

		
		}

	      this->read_mutex.lock();
	      income_packets.push_back(netpacket);
	      std::cout << "[+] Income packets.size() == " << income_packets.size() << std::endl;
	      this->read_mutex.unlock();



	      this->buffer->Delete(e.first, e.second);
	 
	    }
	}
      
    }
  
  // Disconnected..
  printf("Exiting network read loop. \n");
}


      
