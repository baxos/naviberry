#include <iostream>
#include <sstream>
#include <cstdint>
#include <map>
#include <cstring>
#include <iostream>
#include <list>
#include <iomanip>
#include <cstdio>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>

}
// Use of project libraries
#include "comm.hpp"
#include "naviberryio.hpp"

// For debugging
bool debugFlag = false;


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
/**
  * @name NetworkPacket
  * @brief Constructs the class
  * @retval None
  *
  * Constructs the class, set the unique ID, increment id counter or reset it
  **/
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
/**
  * @name ~NetworkPacket
  * @brief Deconstruct the class
  * @retval None
  *
  *
  * Deconstructs the class, free the memory used. 
  * Let the class know, that the unique ID is now free for use.
  **/
NetworkPacket::~NetworkPacket()
{
  if(debugFlag)
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


/**
  * @name CreateDataPacket
  * @brief Creates a NetworkPacket
  * @param uint8_t* _data : Pointer to byte array
  * @param uint32_t _dataSize : Size of the array
  * @param uint8_t _type : Which channel
  *
  *
  * Constructs a header and body packet out of the data given. 
  * Used for sending information in a controlled way.
  **/
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
  if (_dataSize > 4096)
    {
      print_error("Max data size is : 4096 bytes");
    }
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

  if(debugFlag)
    print_msg("Packets constructed");
}

/**
  * @name CreateTextPacket
  * @brief Creates a NetworkPacket
  * @param string txt : The text string
  * @retval None
  *
  *
  * Constructs a header and body packet from the text string given.
  * This is used for sending text messages back and forth between client/server.
  **/
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
  

  if (debugFlag)
    print_msg("Packet created");
}





//====================================== network class =========================================== //
// Network Class Constructor
// Takes input host nd port
// sets class variables
/**
  * @name Network
  * @brief Constructs the Network class
  * @param string _host : Host address
  * @param uint16_t _port : Port
  * @retval None
  *
  *
  * Initializes the Network class, sets data to local class.
  * Initializing buffer.
  **/
Network::Network(std::string _host, uint16_t _port)
{
  // Initialize buffer
  cbuffer = new NaviBuffer(100000);

  // Set class variables
  hostname = _host;
  port = _port;

  // is not connected
  connected = false;

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

/**
  * @name CreateServer
  * @brief Makes the class act as server
  * @retval True on success
  * @retval False on error
  *
  * 
  * This function runs following commands :
  *
  * Create socket
  *
  * Bind socket
  *
  * Listen on socket
  *
  * Accept connection
  *
  * If any of the steps fails, it will return false
  **/
bool Network::CreateServer()
{
  bool success = true;

  // Create socket
  if (Create() == true)
    {
      // Next
    }
  else
    {
      success = false;
    }


  // Bind to address
  if (Bind() == true)
    {
      // Next
    }
  else
    {
      success = false;
    }

  // Listen for incoming calls
  if (Listen() == true)
    {
      // Next
    }
  else
    {
      success = false;
    }

  // Accept any calls
  if (Accept() == true)
    {
      // Do nothing
    }
  else
    {
      success = false;
    }

  // Return the definition of success
  return success;
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

// Network Connect bool function
// Tries to connect to the specified hostname and port
// On success it returns true and the connection is ready
// On failure it will return false
bool Network::Connect()
{
  if ( (confd = connect(sockfd,(struct sockaddr *)  &server_addr, sizeof(server_addr)))== -1)
    {
      print_warning("Failed to connect");
      return false;
    }
  else
    {
      print_msg("Connected to server");
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
  confd = accept(sockfd, NULL, NULL);
  if (confd ==  -1)
    {
      // Error
      print_warning("Failed to accept socket");
      return false;
    }
  else
    {
      // Success
      print_msg("Socket accepted");
      connected = true;
      return true;
    }  
}



/**
  * @name writeRaw
  * @brief Writes raw data to the connected socket
  * @param uint8_t* val : Pointer to byte array
  * @param size_t len : The size of the data
  * @retval True on success
  * @retval False on error
  *
  **/
bool Network::writeRaw(uint8_t* val, size_t len)
{
  auto n = send(confd, val, len, 0);
  if ( n == -1 )
    {
      // Error
      std::cout << std::strerror(errno) << std::endl;
      print_error("Error sending raw bytes.");
      return false;
   }
  else
    {
      // Success
      if (debugFlag)
	print_msg("Successfully send raw data");


      return true;
    }
}

/**
  * @name writeData
  * @brief Writes data to specified chanel
  * @param uint8_t* _data : Pointer to the byte array
  * @param uint32_t _dataSize : Size of the data
  * @param uint8_t _type : Which channel
  * @retval None
  *
  *
  * Writes out raw data to the connected client on a specified channel.
  **/
void Network::WriteData(uint8_t* _data, uint32_t _dataSize, uint8_t _type)
{
  NetworkPacket packet;
  auto error_happened = false;
  
  // Create a new packet with the data 
  packet.CreateDataPacket(_data, _dataSize, _type);
  
  
  // Send signal
  uint8_t* signal = new uint8_t[4];
  if (signal == nullptr)
    {
      print_error("Memory error");
    }
  else
    {
      
    }
  std::memcpy(signal, "NAVI", 4);
  writeRaw(signal, 4);


  
  // Send header
  auto write_check = writeRaw(packet.getheaderBytes(), packet.getheaderBytesCount());
  if ( (write_check) == true)
    {
      if (debugFlag)
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
  write_check = writeRaw(packet.getbodyBytes(), packet.getbodyBytesCount());
  if (write_check == true)
    {
      if (debugFlag)
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
/**
  * @name WriteText
  * @brief Writes text to the connected
  * @param string _txt : The text string
  * @retval None
  *
  **/
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
      if (debugFlag)
	print_msg("Successfully send package");
    }
  else
    {
      print_warning("Failed to send package");
    }
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
		      
		      if (debugFlag)
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
		  if (debugFlag)
		    print_msg("Iterated through buffer");
		  isDone = true;
		  break;
		}
	      
	      if ( buffer[n] == 'N' && buffer[n+1] == 'A' && buffer[n+2] == 'V' && buffer[n+3] == 'I')
		{
		  // Found packet
		  if (debugFlag)
		    std::cout << "Signature bytes found on offset " << n << std::endl;
		  if (buffer[n+4] == sizeof(HeaderPacket))
		    {
		      if (debugFlag)
			print_msg("Packet is a headerpacket");

		      // Header packet found
		      HeaderPacketFound = true;
		    }
		    else
		      {
			if(debugFlag)
			  print_msg("Packet is a body packet");

			// Body packet found
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

	      if (debugFlag)
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
		  if (debugFlag)
		    print_msg("Adding body packet to open list");

		  // Add to list
		  openBodyPackets.push_back(bpacket);
		}
	      else
		{
		  if (debugFlag)
		    print_msg("Data does not match a packet");
		}
	    } 
	  else
	    {

	      // print_warning("Nothing matched, dumping buffer");
	      // DumpBuffer();
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
/**
  * @name Read
  * @brief Tries to read data from the connected.
  * @retval None
  *
  *
  * This function will read data from the connected socket. Maximum read data at once is 1024 bytes.
  * If any data is read, it will be added to the class buffer, for later to be analyzed.
  **/
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
      if (debugFlag)
	std::cout << "[+] Recieved : " << n << " data from server" << std::endl;


      cbuffer->Add(local_buffer, (uint32_t) n);
      if (debugFlag)
	print_msg("Added data to buffer");

    }
}
