
#include <iostream>
#include <cstdint>

extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
}
// Use of project libraries
#include "comm.hpp"
#include "naviberryio.hpp"

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

// Network WriteText bool function
// Takes a std::string as input 
// attempts to send the data to the connected socket
// on succes true is returned, false is returned on error
bool Network::WriteText(std::string txt)
{
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
	  // Success
	  return true;
	}
    }
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
      print_warning("Error reading text from socket");
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
