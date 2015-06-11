
#include <iostream>
#include <cstdint>
#include <map>
#include <cstring>
#include <iostream>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
}
// Use of project libraries
#include "comm.hpp"
#include "naviberryio.hpp"

// For debugging
bool debugFlag = true;


//===================================== packet class ============================================= //

// Static variables
uint16_t NetworkPacket::count = 0;
std::map<uint16_t, bool> NetworkPacket::idMap = {};



// NetworkPacket constructor
// set unique id
// increment static counter
NetworkPacket::NetworkPacket()
{
  // Set id upon class creation
  if (idMap.count(count) == false)
    {
      dataId = count;
      idMap[dataId] = true;
    }
  else
    {
      // Duplicate in id's
    }
  // Increment static counter for keeping "unique" id's
  if (count > 65000)
    {
      // Reset before overflow ^^
      count = 0;
    }
  else
    {
      count++;
    }
}

// NetworkPacket deconstructor
// remove id from map
// if data is filled, free it
NetworkPacket::~NetworkPacket()
{
  print_warning("Packet deconstructor called");
  if (idMap.count(dataId) == true)
    {
      idMap.erase(dataId);
    }


  print_warning("Trying to free memory if it's used");


  if (bodyBytesCount > 0)
    {
      if (debugFlag)
	{
	  std::cout << "[+]Freeing " << bodyBytesCount << " bytes of memory" << std::endl;
	}
      delete bodyBytes;
    }

  if (headerBytesCount > 0)
    {
      if (debugFlag)
	{
	  std::cout << "[+]Freeing " << bodyBytesCount << " bytes of memory" << std::endl;
	}
      delete headerBytes;
    }

}

void NetworkPacket::CreateTextPacket(std::string txt, std::string tag)
{
  // set header packet
  if (debugFlag)
    std::cout << "\t [+]Sizeof(HeaderPacket) = " << sizeof(HeaderPacket) << std::endl;

  hpacket.packetSize = sizeof(HeaderPacket);         // size of this structure
  hpacket.dataType = 255;                            // 0xFF text data
  hpacket.dataId = dataId;                           // Semi unique id
  hpacket.dataSize = txt.size();                     // just size of text string
  hpacket.bodyPacketSize = sizeof(bpacket.packetSize) + sizeof(bpacket.dataId) + hpacket.dataSize;
  // Set tag
  // Check for oversize
  if (tag.size() < 10)
    {
      // Zero-set memory first
      std::memset(hpacket.tag, 0, 10);
      std::memcpy(hpacket.tag, tag.c_str(), tag.size());
    }
  else
    {
      print_warning("Tag input was bigger than allowed,only copying 10 bytes");
    }

  

  // set body packet
  bpacket.packetSize = hpacket.bodyPacketSize;
  bpacket.dataId = dataId;
  // copy to memory to structure
  memcpy(bpacket.data, txt.c_str(), hpacket.dataSize);

  // Copy byte structure
  if (debugFlag)
    {
      std::cout << "Copying " << sizeof(HeaderPacket) << " bytes to other location" << std::endl;
    }
  headerBytesCount = sizeof(HeaderPacket);
  headerBytes = new uint8_t[headerBytesCount];
  std::memcpy(headerBytes, &hpacket, headerBytesCount);

  // Copy body structure
  if (debugFlag)
    std::cout << "Copying " << hpacket.bodyPacketSize << " bytes to body bytearray" << std::endl;
  bodyBytesCount = hpacket.bodyPacketSize;
  bodyBytes      = new uint8_t[bodyBytesCount];
  std::memcpy(bodyBytes, &bpacket, bodyBytesCount);
  

  print_msg("Packet created");
}

void NetworkPacket::CreateBinaryPacket(uint8_t* val)
{

}






//====================================== network class =========================================== //
// Network Class Constructor
// Takes input host nd port
// sets class variables
Network::Network(std::string _host, uint16_t _port)
{
  print_msg("Network constructor() called");
  print_msg("Set class variables..");
  // Set class variables
  hostname = _host;
  port = _port;
  // which type of connection
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}



// Network Create bool function
// Attempts to create a socket
// return true on sucess nd false on error
bool Network::Create()
{
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1)
    {
      print_warning("Socket creation failed!");
      return false;
      // Error
    }

  // on finish return true
  return true;
}



// Network Bind bool function
// Attempts to bind to the given address
// through the socket created earlier
bool Network::Bind()
{
  if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
      // Error
      print_warning("Socket failde to bind");
      return false;
    }
  else
    {
      // Success
      print_msg("Socket bound..");
      return true;
    }
}




// Network listen bool function
// Attemps to start listen on the class' socket file-descritor
// returns true on success and false on failure
bool Network::Listen()
{
  if ( listen(sockfd, 5) == -1)
    {
      // Error
      print_warning("Failed to listen on socket");
      return false;
    }
  else
    {
      // Success
      print_msg("Now listening on socket");
      return true;
    }
}




// Network Accept bool function
// Blocks and wait for a connection attempt
// Then try to accet that attempt to create connection
// on success confd is connected to the client
// and it will return true, otherwise false is returned
bool Network::Accept()
{
  if ( (confd = accept(sockfd,NULL,NULL)) == -1)
    {
      // Error
      print_warning("Failed to accept socket");
      return false;
    }
  else
    {
      // Success
      print_msg("Socket accepted");
      return true;
    }  
}



bool Network::writeRaw(uint8_t* val, size_t len)
{
  
  auto n = send(confd, val, len, 0);
  if ( n == -1 )
    {
      // Error
      print_error("Error sending raw bytes.");
    }
  else
    {
      // Success
      print_msg("Successfully send raw data");
    }
}


// Network WriteText bool function
// Takes a std::string as input 
// attempts to send the data to the connected socket
// on succes true is returned, false is returned on error
void Network::WriteText(std::string txt, std::string tag)
{
  // Create packet container
  NetworkPacket packet;

  // Create new packet with txt as data
  packet.CreateTextPacket(txt, tag);

  // Send signal
  uint8_t* signal = new uint8_t[4];
  std::memcpy(signal, "NAVI", 4);
  writeRaw(signal, 4);

  // Send header
  if (debugFlag)
    std::cout << "Sending " << packet.getheaderBytesCount() << " bytes to client" << std::endl;

  if ( (writeRaw(packet.getheaderBytes(), packet.getheaderBytesCount())) == true)
    {
      print_msg("Successfully send header packet");
    }
  else
    {
      print_warning("Failed to send header packet");
    }

  // Send body packet
  if (debugFlag)
    std::cout << "Sending " << packet.getbodyBytesCount() << " bytes to client" << std::endl;

  // Send signal
  writeRaw(signal, 4);

  
  if (writeRaw(packet.getbodyBytes(), packet.getbodyBytesCount()) == true)
    {
      print_msg("Sucessfully send body packet");
    }
    else
      {
	print_warning("Failed to send body packet");
      }

  



  /* Experiments with code above, this is the original code 
  auto n = 0;
  if ( (n = send(confd, txt.c_str(), txt.size(), 0)) == -1)
    {
      // Error
      print_error("Error happened while trying to send data");
      return false;
    }
  else
    {
      if (n == txt.size())
	{
	  print_msg("Successfully send following data");
	  print_msg(txt);
	  // Success
	  return true;
	}
      return false;
    }

*/
}


bool Network::SendTextPacket(std::string txt)
{

  // NetworkPacket packet;
  //  packet.CreateTextPacket(txt);
  
  // write header


  return true;
}


bool Network::SendBinaryPacket(uint8_t* data)
{

}

// Network ReadText string function
// It attempts to read a block of text
// from the connected socket
// on success the text is returned, on failure empty string is returned.
std::string Network::ReadText(void)
{
  auto n = 0;
  const auto buff_size = 1024; 
  char buffer[buff_size] = {};
  std::string str_buffer = "";

  if ( (n = recv(confd, &buffer, buff_size, 0)) == -1)
    {
      // Error
      print_error("Error reading text from socket");
    } 
  else
    {
      // Success
      std::cout << "Recieved : " << n << " data from server, copying to string" << std::endl;
      for (auto i = 0; i < n; i++)
	{
	  auto c = buffer[i];
	  str_buffer.push_back(c);
	}
    }

  return str_buffer;
}
