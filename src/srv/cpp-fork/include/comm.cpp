
#include <iostream>
#include <cstdint>
#include <cstring>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
}
// Use of project libraries
#include "comm.hpp"
#include "naviberryio.hpp"

//===================================== packet class ============================================= //

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
  if (idMap.count(dataId) == true)
    {
      idMap.erase(dataId);
    }

  if (TotalBytes != NULL)
    delete TotalBytes;
}

void NetworkPacket::CreateTextPacket(std::string txt)
{
  // set header packet
  hpacket.packetSize =                               // =64
    sizeof(hpacket.packetSize) +                     //   8
    sizeof(hpacket.dataType) +                       //   8
    sizeof(hpacket.dataId) +                         //  16
    sizeof(hpacket.dataSize);                        //+ 32 
  hpacket.dataType = 255;                            // 0xFF text data
  hpacket.dataId = dataId;                           // Semi unique id

  // Prepare for body packet
  auto body_size = sizeof(bpacket.packetSize) + sizeof(bpacket.dataId) +  txt.size();
  // reserve memory
  uint8_t* txtdata = new uint8_t[txt.size()];
  // copy each byte
  for (auto i = 0; i < txt.size(); i++)
    {
      auto c = txt[i];
      txtdata[i] = c;
    }

  // set body packet
  bpacket.packetSize = body_size;
  bpacket.dataId = dataId;
  if ( txt.size() < sizeof(bpacket.data))
    {
      memcpy(bpacket.data, &txtdata, sizeof(txt.size()));
    }
  else
    {
      print_warning("Trying to create text packet, failed hence the data was too big to fit");
    }


  // Calculate TOTOAL bytes used by header and body and create a parrellel array
  // That holds  
  // Header
  //   Body
  //     Body data

  auto bytes_used = 0;
  bytes_used =     hpacket.packetSize +  bpacket.packetSize;
  // set value to class var
  TotalBytesCount = bytes_used;

  // Prepare for copy the data
  // reserve memory
  TotalBytes = new uint8_t[TotalBytesCount];

  // Start with header
  auto offset = 0;

  // Heaer packet
  TotalBytes[offset]= (uint8_t) hpacket.packetSize;
  offset += sizeof(hpacket.packetSize);

  TotalBytes[offset] = (uint8_t) hpacket.dataType;
  offset += sizeof(hpacket.packetSize);

  TotalBytes[offset] = (uint16_t) hpacket.dataId;
  offset += sizeof(hpacket.dataId);

  TotalBytes[offset] = (uint32_t) hpacket.dataSize;
  offset += sizeof(hpacket.dataSize);

  // Body packet
  TotalBytes[offset] = (uint32_t) bpacket.packetSize;
  offset += sizeof(bpacket.packetSize);

  TotalBytes[offset] = (uint16_t) bpacket.dataId;
  offset += sizeof(bpacket.packetSize);
  
  // Loop through the last one byte array
  for (auto n = 0; n > hpacket.dataSize; n++)
    {
      TotalBytes[offset] = (uint8_t) bpacket.data[n];
      offset += sizeof(uint8_t);
    }

  if (offset != TotalBytesCount)
    {
      print_error("Failed trying to create parrellel byte array, size mismatch");
    }
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
bool Network::WriteText(std::string txt)
{
  // Create packet container
  NetworkPacket packet;

  // Create new packet with txt as data
  packet.CreateTextPacket(txt);

  // Send it to client 
  if ( (writeRaw(packet.getTotalBytes(), packet.getTotalBytesCount())) == true)
    {
      print_msg("Successfully send data");
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

  NetworkPacket packet;
  packet.CreateTextPacket(txt);
  
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
