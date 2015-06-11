#include <iostream>
#include <string>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include "./serial.hpp"
using namespace std;

SerialComm::SerialComm(std::string port)
{
  sfd = open(port.c_str(), O_RDWR | O_NONBLOCK);
  if (sfd == -1)
    {
      ready = false;
      cout << "[-]Error initializing serial port.."<< endl;
    }
}

std::string SerialComm::Read(std::string dest)
{
  cout << "[+]Trying to read from serial port" << endl;
  if (ready)
    {
      char cbuffer[1024];
      std::fill(cbuffer, cbuffer+1024, '\0');
      const auto MAX_NOREADS = 100;
      auto no_reads = 0;
      auto counter  =0;

      while (counter < 30)
	{
	  auto n = read(sfd, cbuffer, sizeof(cbuffer));
	  if ( n == -1)
	    {
	      no_reads++;
	      if (no_reads >= MAX_NOREADS)
		break;
	    }
	  else if ( n == 0)
	    {
	      no_reads ++;
	      if (no_reads >= MAX_NOREADS)
		break;
	    }
	  else
	    {
	      std::cout << "[+]Data recieved [" << n << " bytes" << std::endl;
	      for ( auto i = 0; i < n; i++)
		{
		  std::cout << cbuffer[i];
		}
	    }

	  counter += n;
	}
    
      return std::string(cbuffer);
    }

  return "";

}

void SerialComm::Write(std::string data)
{
  auto dataSize = data.size();

  auto n = write(sfd, data.c_str(), dataSize);

  if (n == -1)
    {
      std::cout<<"[-]Error happened, are you sure you have access?" << std::endl;
     
    }
  else if ( n == 0)
    {
      std::cout<<"[-]No data was written, unkown status" << std::endl;
    }
  else if ( n == dataSize)
    {
      std::cout<<"[+]Wrote data successfully" << std::endl;
    }
}

/*
int main()
{
  /* Open file descriptor to the target 
   * Have a buffer of 128 bytes
   * Check for errores
   * in filedescriptor
   

   
  
  while (true)
    {
    
      // WRITE
      // red user input
      // send data to serial port

      cin >> buffer;
      cout << "Writing : " << buffer << " to tty, data size  : "<< buffer.size() << endl;

      if ( buffer == "quit")
	{
	  break;
	}
      else
	{
	  char bytebuffer[512] = {};
	  for (auto i = 0; i < buffer.size(); i++)
	    {
	      // overflow check
	      if ( i == sizeof(buffer)-1)
		break;

	      bytebuffer[i] = buffer[i];

	    }
	  auto n = write(sfd, bytebuffer, buffer.size());
	
	  if ( n == -1)
	    {
	      cout << "[-]Error happened, are you sure you have access?" << endl;
	    }
	  else if ( n == 0 )
	    {
	      cout << "[-]No data was written, unknown status" << endl;
	    }
	  else if ( n == buffer.size())
	    {
	      cout << "[+]Wrote data successfully" << endl;
	    }
	}
    

      // READ
      // read data from serial port
      // print to screen

      char cbuffer[1024];
      const auto MAX_NOREADS = 1000;
      auto no_reads = 0;

      auto counter = 0;

      while (counter < 20)
	{
	  if ( counter < 0)
	    break;

	  auto n = read(sfd, cbuffer, sizeof(cbuffer));
	  
	  if (n == -1)
	    {
	      //cout << "[-]Error occured while reading from filedescriptor" << endl;
	    }
	  else if ( n == 0 )
	    {
	      //cout << "[+]Read zero bytes" << endl;
	      no_reads++;
	      if (no_reads >= MAX_NOREADS)
		break;
	    }
	  else
	    {
	      cout << "Data recieved ["<<n<<" bytes]: " << endl;
	      for (auto i = 0; i < n; i++)
		{
		  cout << cbuffer[i];
		}
	    }
	  
	  
	  counter += n;
	  //cout << "\t [+]Counter ["<<counter<<"|100"<<endl;
	}
    

      cout << "Ready to input : " << endl;
    }


  close(sfd);
}


*/
