
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

bool Network::Listen()
{
  // Listen to the class sock descriptor
  // Returns false on error
  // nd true on success
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
      for (auto i = 0; i > n; i++)
	{
	  str_buffer.push_back(buffer[i]);
	}
    }

  return str_buffer;
}
