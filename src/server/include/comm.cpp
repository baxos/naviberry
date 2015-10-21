#include <iostream>
#include <sstream>
#include <cstdint>
#include <map>
#include <cstring>
#include <iostream>
#include <list>
#include <iomanip>
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


// ===================================== TextPacket ==================== //
TextPacket::TextPacket()
{
  tag ="";
  value ="";
  dataId = 0;
}


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



  if (bodyBytesCount > 0)
    {
      if (debugFlag)
	{
	  std::cout << "[+]Freeing " << (uint32_t) bodyBytesCount << " bytes of memory" << std::endl;
	}
      delete bodyBytes;
    }

  if (headerBytesCount > 0)
    {
      if (debugFlag)
	{
	  std::cout <<  "[+]Freeing " << (uint32_t) headerBytesCount << " bytes of memory" << std::endl;
	}
      delete headerBytes;
    }

}

void NetworkPacket::CreateDataPacket(uint8_t* _data, uint32_t _dataSize, uint8_t _type)
{
  // Set the header packet
  hpacket.packetSize   = sizeof(HeaderPacket);
  hpacket.dataType     = _type;
  hpacket.dataId       = dataId;
  hpacket.dataSize     = _dataSize;
  hpacket.bodyPacketSize = (sizeof(bpacket.packetSize) + sizeof(bpacket.dataId) + hpacket.dataSize);
  

  // Set the body packet
  bpacket.packetSize     = hpacket.bodyPacketSize;
  bpacket.dataId         = dataId;
  // Copy data to the structure
  memcpy(&bpacket.data[0], &_data[0], _dataSize);
	
  if (debugFlag)
    {
      std::cout << "hpacket:"<<std::endl <<
	"HeaderPacket.packetSize \t : " << (int) sizeof(HeaderPacket) << std::endl <<
	"HeaderPacket.dataType   \t : " << (uint16_t) hpacket.dataType << std::endl <<
	"HeaderPacket.dataSize   \t : " << (uint32_t) hpacket.dataSize << std::endl;
    }

  // raw byte copy of header packet
  headerBytesCount = sizeof(HeaderPacket);
  headerBytes      = new uint8_t[headerBytesCount];
  std::memcpy(&headerBytes[0], &hpacket, headerBytesCount);


  // raw byte copy of body packet
  bodyBytesCount = hpacket.bodyPacketSize;
  bodyBytes      = new uint8_t[bodyBytesCount];
  std::memcpy(&bodyBytes[0], &bpacket, bodyBytesCount);
}

void NetworkPacket::CreateTextPacket(std::string txt)
{
  // set header packet
  hpacket.packetSize = sizeof(HeaderPacket);         // size of this structure
  hpacket.dataType = 255;                            // 0xFF text data
  hpacket.dataId = dataId;                           // Semi unique id
  hpacket.dataSize = txt.size();                     // just size of text string
  hpacket.bodyPacketSize = sizeof(bpacket.packetSize) + sizeof(bpacket.dataId) + hpacket.dataSize;
  

  // set body packet
  bpacket.packetSize = hpacket.bodyPacketSize;
  bpacket.dataId = dataId;
  // copy to memory to structure
  std::memcpy(bpacket.data, txt.c_str(), hpacket.dataSize);

  // Copy byte structure
  if (debugFlag)
    {
      std::cout << "Copying " << sizeof(HeaderPacket) << " bytes to other location" << std::endl;
    }
  // Copy body structure
  if (debugFlag)
    {
      std::cout << "Copying " << hpacket.bodyPacketSize << " bytes to body bytearray" << std::endl;
    }

  // Make copy of header bytes
  headerBytesCount = sizeof(HeaderPacket);
  headerBytes      = new uint8_t[headerBytesCount];
  std::memcpy(headerBytes, &hpacket, headerBytesCount);

  // Make copy of body bytes
  bodyBytesCount = hpacket.bodyPacketSize;
  bodyBytes      = new uint8_t[bodyBytesCount];
  std::memcpy(bodyBytes, &bpacket, bodyBytesCount);
  

  print_msg("Packet created");
}





//====================================== network class =========================================== //
// Network Class Constructor
// Takes input host nd port
// sets class variables
Network::Network(std::string _host, uint16_t _port)
{
  print_msg("Network constructor() called");
  print_msg("Set class variables..");
  print_msg("Set standard buffer size to 100.000 bytes");


  // Initialize buffer
  cbuffer = new NaviBuffer(100000);

  // Set class variables
  hostname = _host;
  port = _port;



  // which type of connection
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}


// Network setBufferSize void function
// Takes unsigned int as input
// to specify buffer size
// and (re)allocate
void Network::setBufferSize(uint32_t size)
{
  /*
  auto old_size = buffer_size;
  buffer_size = size;
  if ( buffer != NULL)
    {
      // Create new memory
      uint8_t* tmp = new uint8_t[size];
      std::memcpy(tmp, buffer, old_size);
      delete buffer;
      buffer = tmp;
    }
  else
    {
      buffer = new uint8_t[size];
    }

  buffer_end = size;
  */
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


void Network::WriteData(uint8_t* _data, uint32_t _dataSize, uint8_t _type)
{
  NetworkPacket packet;
  auto error_happened = false;
  
  // Create a new packet with the data 
  packet.CreateDataPacket(_data, _dataSize, _type);
  
  
  // Send signal
  uint8_t* signal = new uint8_t[4];
  std::memcpy(signal, "NAVI", 4);
  writeRaw(signal, 4);
  
  // Send header
  if ( (writeRaw(packet.getheaderBytes(), packet.getheaderBytesCount()) == true))
    {
      print_msg("Successfully send header packet");
    }
  else
    {
      print_warning("Failure happened while trying to send header packet");
      error_happened = true;
    }
  
  
  // Resend signal
  writeRaw(signal, 4);

  // Send body
  if ( (writeRaw(packet.getbodyBytes(), packet.getbodyBytesCount()) == true ))
    {
      print_msg("Successfully send body packet");
    }
  else
    {
      print_warning("Failure happened while trying to send body packet");
      error_happened = true;
    }
    
}

// Network WriteText bool function
// Takes a std::string as input 
// attempts to send the data to the connected socket
// on succes true is returned, false is returned on error
void Network::WriteText(std::string _txt)
{
  // Create packet container
  NetworkPacket packet;
  auto error_happened = false;
  
  // Create new packet with txt as data
  packet.CreateTextPacket(_txt);

  // Send signal
  uint8_t* signal = new uint8_t[4];
  std::memcpy(signal, "NAVI", 4);
  writeRaw(signal, 4);

  // Send header
  if (debugFlag)
    {
      std::cout << "[+] Sending " << (int) packet.getheaderBytesCount() << " bytes to client" << std::endl;
    }
  
  if ( (writeRaw(packet.getheaderBytes(), packet.getheaderBytesCount())) == true)
    {
      //      print_msg("Successfully send header packet");
    }
  else
    {
      print_warning("Failed to send header packet");
      error_happened = true;
    }

  // Send body packet
  if (debugFlag)
    {
      std::cout << "[+]Sending " << packet.getbodyBytesCount() << " bytes to client" << std::endl;
    }
  // Send signal
  writeRaw(signal, 4);

  
  if (writeRaw(packet.getbodyBytes(), packet.getbodyBytesCount()) == true)
    {
      // print_msg("Sucessfully send body packet");
    }
  else
    {
      print_warning("Failed to send body packet");
      error_happened = true;
    }

  if ( error_happened == false)
    {
      print_msg("Successfully send package");
    }
  else
    {
      print_warning("Failed to send package");
    }
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





// Netowrk CheckForCombinations void function
// Looks for header and body packets matches
// If it finds them it will remove them from open list
// and add them to network queue
void Network::CheckForCombinations()
{
  // ita = iterator for header packets
  // itb = iterator for body packets
  if (debugFlag)
    {
      print_msg("CheckForCombinations() \t Call()");
      std::cout << "HeaderPackets.size() = " << openHeaderPackets.size() << "\t BodyPackets.size() = " << openBodyPackets.size() << std::endl;
      
    }

  if ((openHeaderPackets.size() > 1) && (openBodyPackets.size() > 1))
    {
      for (auto ita = openHeaderPackets.begin(); ita != openHeaderPackets.end(); ita++)
	{
	  for (auto itb = openBodyPackets.begin(); itb != openBodyPackets.end(); itb++)
	    {
	      if (ita->dataId == itb->dataId)
		{
		  // We got match!
		  TextPacket tp;
		  if (ita->dataType = 0xFF)
		    {
		      std::string strbuffer ="";
		      // Text packet
		      for (auto n = 0;n < ita->dataSize; n++)
			{
			  strbuffer += (char) itb->data[n];		      
			}
		      // Set value
		      tp.setValue(strbuffer);
		      		      
		      // Set id
		      tp.setId(ita->dataId);

		      if (debugFlag)
			{
			  print_msg("Adding text packets to queue");
			  std::cout << "VAL " << tp.getValue()<< std::endl;
			}
		      
		      // Add to list
		      TextPacketQueue.push_back(tp);
		      
		      
		      print_msg("Removing elements from open list");
		      // Remove elements from list
		      ita = openHeaderPackets.erase(ita);
		      itb = openBodyPackets.erase(itb);
		    }     
		}
	    }
	}      
    }

  if (debugFlag)
    {
      print_msg("CheckForCombinations() \t Exit()");
    }
}



void Network::DumpBuffer()
{
  auto i = 0;
  auto data_size = cbuffer->getDataSize();
  uint8_t* data = cbuffer->getData();
  for (auto i = 0; i < data_size; i++)
    {
      uint8_t val = data[i];
      printf("%u \t", val);
    }
}

// Network CheckForPackets void function
// It searches through the class buffer
// for 4 bytes N,A,V,I
// After those 4 bytes either an body or header packet comes
// If function finds any packets, it will add them to open list
// and zero set the section in the class buffer
void Network::CheckForPackets()
{
  if (debugFlag)
    {
      print_msg("CheckForPackets() \t Call()");
    }

      auto pack_offset        = 0;
      auto pack_offset_start  = 0;
      auto iterations_counted = 0;
      bool packet_found       = false;
      bool isDone             = false;

      uint8_t* buffer = cbuffer->getData();
      uint32_t buffer_size = cbuffer->getDataSize();

      if (debugFlag)
	std::cout << "current buffer data size = " << buffer_size << std::endl;


      // Buffer is too small to we care about it...
      if (buffer_size < 20)
	return;
      

      while (isDone == false)
	{
	  iterations_counted++;
	  packet_found = false;
	  pack_offset_start = 0;
	  bool HeaderPacketFound = false;
	  bool BodyPacketFound   = false;


	  for (auto n=pack_offset; n <=buffer_size ; n++)
	    {
	      // check for overflow
	      if ( (n + 4 ) > buffer_size)
		{
		  print_msg("Iterated through buffer");
		  isDone = true;
		  break;
		}
	      
	      if ( buffer[n] == 'N' && buffer[n+1] == 'A' && buffer[n+2] == 'V' && buffer[n+3] == 'I')
		{
		  // Found packet
		  std::cout << "Signature bytes found on offset " << n << std::endl;
		  if (buffer[n+4] == sizeof(HeaderPacket))
		    {
		      print_msg("Packet is a headerpacket");
		      HeaderPacketFound = true;
		    }
		    else
		      {
			print_msg("Packet is a body packet");
			BodyPacketFound = true;
		      }
		  packet_found      = true;
		  pack_offset       = n+4;
		  pack_offset_start = pack_offset;
		  break;
		}
	    }

	  if (debugFlag)
	    std::cout << "packet_found=" << packet_found << "\t buffer[pack_offset] = " << (uint32_t) buffer[pack_offset] << " should be = " << sizeof(HeaderPacket) << std::endl;

	  if ( ((packet_found) && buffer[pack_offset] == sizeof(HeaderPacket)))
	    {
	      // If n+4th byte is equal to the sizeof headerstructure
	      // its a header packet found
	      // Construct a header packet structure
	      // And add it to the open list
	      HeaderPacket hpacket;
	      
	      // copy raw bytes to headerpacket structure
	      std::memcpy(&hpacket, &buffer[pack_offset], sizeof(HeaderPacket));
	      
	      if (debugFlag)
		{
		  std::cout <<"[+]PACKET \t ID : " << hpacket.dataId << std::endl;
		}
	      // Zero set memory
	      // std::memset(&buffer[pack_offset_start-4], 0, sizeof(HeaderPacket)+4);
	      cbuffer->RemoveAt(pack_offset_start-4, sizeof(HeaderPacket)+4);
	      
	      // Add to open list
	      openHeaderPackets.push_back(hpacket);
	    }
	  else if (packet_found)
	    {
	      // Navi signal is found and th epacket size doesn't match a header packet
	      // So this should be a body packet
	      BodyPacket bpacket;
	      uint32_t packSize = (uint32_t) buffer[pack_offset];


	      pack_offset += sizeof(uint32_t);
	      std::cout << "Pack size : " << packSize << std::endl;

	      if (packSize > 6)
		{
		  uint16_t dataId = (uint16_t) buffer[pack_offset];
		  pack_offset += sizeof(uint16_t);
		  // copy to structure
		  if (packSize > 1024)
		    {
		      // error
		      print_error("Data size is bigger than buffer!");
		    }
		  bpacket.packetSize = packSize;
		  bpacket.dataId = dataId;
		  std::memcpy(bpacket.data, &buffer[pack_offset-1], (bpacket.packetSize - 6));
		  
		  // Remove from buffer
		  // std::memset(&buffer[pack_offset_start-4], 0, bpacket.packetSize+4);
		  cbuffer->RemoveAt(pack_offset_start-4, bpacket.packetSize+4);
		  
		  // Add to open list
		  print_msg("Adding body packet to open list");
		  openBodyPackets.push_back(bpacket);
		}
	      else
		{
		  print_msg("Data does not match a packet");
		}
	    } 
	  else
	    {

	      print_warning("Nothing matched, dumping buffer");
	      DumpBuffer();
	      isDone = true;
	    }
	}
      
      if (debugFlag)
	{
	  std::cout << "Iterations = " << iterations_counted << std::endl;
	  print_msg("CheckForPackets() \t Exit()");
	}
      
}



// Network Read void function
// It attempts to read a block of bytes
// from the connected socket
// on success the bytes are added to the network buffer
// so the network packet handlers can find and match packets correctly.
void Network::Read(void)
{
  auto n                          = 0;
  const auto buff_size            = 1024; 
  uint8_t local_buffer[buff_size] = {};
  std::string str_buffer          = "";

  if ( (n = recv(confd, &local_buffer, buff_size-1, 0)) == -1)
    {
      // Error
      print_error("Error reading text from socket");
    } 
  else
    {
      // Success
      std::cout << "[+] Recieved : " << n << " data from server" << std::endl;
      cbuffer->Add(local_buffer, (uint32_t) n);
      print_msg("Added data to buffer");
      /*for (auto i = 0; i < n; i++)
	{
	  // Check for overflow
	  if ( (buffer_offset + n) >= buffer_size)
	    {
	      print_error("Network buffer is full!");
	    }
	  // Check for buffer is not null
	  if (buffer == NULL)
	    {
	      print_error("Buffer is not initialized!!");
	    }
	  // If not
	  buffer[buffer_offset] = local_buffer[i];
	  buffer_offset++;
	}
      */
    }

  print_msg("Return call");
}
