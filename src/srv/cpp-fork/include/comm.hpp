/*
  Filename    : comm.hpp
  Author      : J.E Bacher
  Description : Network class header, contains string constants of various server messages
                and have the network class for handling the communication.
*/
#include <string>
#include <cstdint>
#include <map>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
}
//struct sockaddr_in;


//===================================================================== network packet ================================== //
// Network packet structs and class
// this is used to by the network class to send data between client<->server


struct HEADER_PACKET
{
  uint8_t packetSize;
  uint8_t dataType;
  uint16_t dataId;
  uint32_t dataSize;
} typedef HeaderPacket;

struct BODY_PACKET
{
  uint32_t packetSize;
  uint16_t dataId;
  uint8_t data[1024];
} typedef BodyPacket;


class NetworkPacket
{
private:
  HeaderPacket hpacket;                         // Header packet, structure is defined in this file
  BodyPacket bpacket;                           // Body packet,   structure is defined in this file 
  static uint16_t count;                        // Static counter, used for keeping unique ID's
  static std::map<uint16_t, bool> idMap;        // Static map,used for checking whether an ID is used or not
  uint16_t dataId;                              // Unique ID for the packet, both header and body use same ID
  uint8_t* TotalBytes;
  uint32_t TotalBytesCount;
public:
  NetworkPacket();                              // Constructor, description is defined in cpp file
  ~NetworkPacket();                             // Deconstructor, free memory 
  void CreateTextPacket(std::string txt);       // Create a body and header packet from a text string
  void CreateBinaryPacket(uint8_t* val);        // Create a body and header packet from raw bytes 
  uint8_t* getTotalBytes(){return TotalBytes;};
  uint32_t getTotalBytesCount(){return TotalBytesCount;};
};

//===========================================================================================================================================================
//
// Networking class
// Handles socket TCP communcation
class Network 
{
private:
  std::string hostname;                         // The name of the host, this should be local, since we are the server
  uint16_t port;                                // Port number
  uint8_t sockfd;                               // Socket file descriptor for listen
  uint8_t confd;                                // Socket file descriptor connected to client
  struct sockaddr_in server_addr;               // Structure for keeping information about the adress port etc.
  struct sockaddr_in client_addr;               // Saeme
  bool writeRaw(uint8_t* val, size_t len);
public:                                         // Public functions
  Network(std::string _host, uint16_t _port);   // Constructor takes in hostname and port number as arguments as it constructs the class
  bool Create();                                // Initialize the class , returns false on error and true on success
  bool Bind();                                  // Binds the socket, returns false on error and true on success
  bool Listen();                                // Listen on the socket, returns false on error and true on success
  bool Accept();                                // Accept a connection attempt, returns false on error and true on success 

  bool SendTextPacket(std::string txt);
  bool SendBinaryPacket(uint8_t* data);
  bool WriteText(std::string txt);              // Write data to a packet and send it to the client
  std::string ReadText();                       // Reads a packet of text data from the client
};


// ===================================================================================================================================================== //
// Cosntant strings for server communication

/*
  CLIENT MESSAGES
*/
const char comm_CLIENT_START[] = "CLIENT_START\n";
const char comm_CLIENT_PING[]  = "CLIENT_PING\n";
const char comm_CLIENT_DISCONNECT[] = "CLIENT_DISCONNECT\n";
const char comm_CLIENT_STATUS[]     = "CLIENT_STATUS\n";

/*
 REPLY MESSAGES
*/

const char comm_REPLY_SUCCESS[]     = "SERVER_SUCCESS\n";
const char comm_REPLY_FAILURE[]     = "SERVER_FAILURE\n";
const char comm_REPLY_WAITING[]     = "SERVER_WAITING\n";
const char comm_REPLY_PONG[]        = "SERVER_PONGING\n";
const char comm_REPLY_DISCONNECT[]  = "SERVER_DISCONNECT\n";
const char comm_REPLY_BUSY[]        = "SERVER_BUSY\n";
const char comm_REPLY_UNKNOWN_CMD[]   = "SERVER_UNKOWN_COMMAND\n";

//
//========================================================================================================================================================= //

/*
  PUBLIC FUNCTIONS
*/

void comm_sendtxt(int socket, const char* txt);
