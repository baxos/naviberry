#ifndef COMM__HPP
#define COMM__HPP
/**
  * @name comm.hpp
  * @author Jan Emil Bacher
  * @date 2015-2016
  *
  *
  * Communication and networking.
  **/
#include <string>
#include <cstdint>
#include <map>
#include <list>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
}
#include "./cbuffer.hpp"



#define TEXT_TYPE             0XFF
#define SENSOR_TYPE           0XAA
#define MAP_TYPE              0xBB


//===================================================================== network packet ================================== //
// Network packet structs and class
// this is used to by the network class to send data between client<->se



/**
  * @struct HeaderPacket
  * @brief The header part of the NetworkPacket
  * @var HeaderPacket::packetSize
  * Size of this packet.
  * @var HeaderPacket::dataType
  * The type of this packet
  * @var HeaderPacket::dataId
  * The unique ID of the packet
  * @var HeaderPacket::dataSize
  * The size of the data the body packet contains
  * @var HeaderPacket::bodyPacketSize
  * The size of the BodyPacket
  **/
struct HEADER_PACKET
{
  uint8_t packetSize;
  uint8_t dataType;
  uint16_t dataId;
  uint32_t dataSize;
  uint32_t bodyPacketSize;
} typedef HeaderPacket;


/**
 * @struct BodyPacket
 * @brief The body part of the NetworkPacket
 * @var BodyPacket::packetSize
 * The size of this packet.
 * @var BodyPacket::dataId
 * The unique ID of the packet
 * @var BodyPacket::data
 * The byte array
 **/
struct BODY_PACKET
{
  uint32_t packetSize;
  uint16_t dataId;
  uint8_t data[4096];
} typedef BodyPacket;

/**
 * @class TextPacket
 * @brief Wraps data into a text packet
 **/
class TextPacket
{
public:
  TextPacket();
  std::string tag;
  std::string value;
  uint16_t dataId;

  void setValue(std::string _val) { value = _val;};
  void setTag(std::string _tag) { tag = _tag;};
  void setId(uint16_t _id) { dataId = _id;};

  std::string getValue() { return value; };
  std::string getTag()   { return tag  ; };
  uint16_t    getDataId(){ return dataId;};
};


/**
  * @class NetworkPacket
  * @brief Packet used for network communication.
  * 
  *
  * A NetworkPacket is the combination of the HeaderPacket and BodyPacket.
  * All messages sent on the network consist of 1 headerpacket and 1 bodypacket.
  * The header contain meta information and the body contains the information itself
  **/
class NetworkPacket
{
private:
  HeaderPacket hpacket;                         // Header packet, structure is defined in this file
  BodyPacket bpacket;                           // Body packet,   structure is defined in this file 
  static uint16_t count;                        // Static counter, used for keeping unique ID's
  static std::map<uint16_t, bool> idMap;        // Static map,used for checking whether an ID is used or not
  uint16_t dataId;                              // Unique ID for the packet, both header and body use same ID
  uint8_t* bodyBytes;
  uint32_t bodyBytesCount;
  uint8_t* headerBytes;
  uint8_t headerBytesCount;
public:
  NetworkPacket();                              // Constructor, description is defined in cpp file
  ~NetworkPacket();                             // Deconstructor, free memory

  uint8_t* getbodyBytes(){return bodyBytes;};
  uint32_t getbodyBytesCount() { return bodyBytesCount;};
  uint8_t* getheaderBytes() { return headerBytes;};
  uint8_t getheaderBytesCount() { return headerBytesCount; };


  void CreateTextPacket(std::string txt);       // Create a body and header packet from a text string
  void CreateDataPacket(uint8_t* _data, uint32_t _dataSize, uint8_t _type);        // Create a body and header packet from raw bytes
};

//===========================================================================================================================================================
//
// Networking class
// Handles socket TCP communcation

/**
  * @class Network
  * @brief Master class for networking.
  *
  *
  * This class contains all the functions to etablish a connection between the server and the client.
  * And contains all the core functions of sending and recieving data.
  *
  * Network.Read()
  *
  * Network.WriteText()
  *
  **/
class Network 
{
private:
  std::string hostname;                         // The name of the host, this should be local, since we are the server
  uint16_t port;                                // Port number
  uint8_t sockfd;                               // Socket file descriptor for listen
  uint8_t confd;                                // Socket file descriptor connected to client
  bool connected;                             // Boolean value for whether the class is connected or not
  // uint32_t buffer_size;
  // uint8_t* buffer;
  // uint32_t buffer_offset;
  // uint32_t buffer_end;
  NaviBuffer* cbuffer;
  std::list<HeaderPacket> openHeaderPackets;
  std::list<BodyPacket> openBodyPackets;
  std::list<TextPacket> TextPacketQueue;
  struct sockaddr_in server_addr;               // Structure for keeping information about the adress port etc.
  struct sockaddr_in client_addr;               // Saeme
  bool writeRaw(uint8_t* val, size_t len);
public:                                         // Public functions
  Network(std::string _host, uint16_t _port);   // Constructor takes in hostname and port number as arguments as it constructs the class
  bool Create();                                // Initialize the class , returns false on error and true on success
  bool Bind();                                  // Binds the socket, returns false on error and true on success
  bool Listen();                                // Listen on the socket, returns false on error and true on successs
  bool Accept();                                // Accept a connection attempt, returns false on error and true on success 
  bool Connect();                               // Connection attempt, returns false on error
  bool CreateServer();                          // Wrapper calls -> [Create,Bind,Listen,Accept]
  void DumpBuffer();                            // For debugging. Dumps the buffer to STDIO
  bool SendTextPacket(std::string txt);         // Sends a text packet
  bool SendBinaryPacket(uint8_t* data);         // Sends a binary packet 
  void setBufferSize(uint32_t size);            // Re-allocate buffer size
  void WriteText(std::string txt);              // Write data to a packet and send it to the client
  void WriteData(uint8_t* _data, uint32_t _dataSize, uint8_t _type);
  void Read();                                  // Reads a packet of text data from the client
  bool isConnected() { return connected; };

  void clearTextQueue() { TextPacketQueue.clear(); };
  std::list<TextPacket> getTextQueue(){ return TextPacketQueue;};
  int getTextQueueCount() { return TextPacketQueue.size(); };
  void CheckForCombinations();
  void CheckForPackets();
};


#endif
