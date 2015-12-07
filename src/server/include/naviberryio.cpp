#include <iostream>
#include <sstream>
#include <string>
#include "naviberryio.hpp"


// print_msg 
// Prints out std::string msg to stdout console using green
void print_msg(std::string msg)
{
  Color::Modifier TXT_GREEN (Color::FG_GREEN);
  Color::Modifier TXT_DEF (Color::FG_DEFAULT);
  std::cout << TXT_GREEN << "[+] " << msg << TXT_DEF <<std::endl;
}

// print_str
// Prints out string formatted via a stringstream
void print_str(std::stringstream& sstream)
{
  Color::Modifier TXT_GREEN (Color::FG_GREEN);
  Color::Modifier TXT_DEF   (Color::FG_DEFAULT);
  std::cout << TXT_GREEN << "[+]" << sstream.str() << TXT_DEF << std::endl;
}

// print_warning
// Prints warning std::string warning to stdout using red
void print_warning(std::string warning)
{
  Color::Modifier TXT_RED (Color::FG_YELLOW);
  Color::Modifier TXT_DEF (Color::FG_DEFAULT);
  std::cout << TXT_RED << "[-] " << warning << TXT_DEF << std::endl;
}

// print_error
// Prints error std::string error to stdout using red
// exits program
void print_error(std::string error)
{
  Color::Modifier TXT_RED (Color::FG_RED);
  Color::Modifier TXT_DEF (Color::FG_DEFAULT);
  std::cout << TXT_RED << "ERROR HAPPENED, EXITING PROGRAM" << std::endl;
  std::cout << "[-] " << error << TXT_DEF << std::endl;
  exit(-1);
}

uint8_t* NaviberryIO::reserveBytes(uint32_t _size)
{
  uint8_t* data = new uint8_t[_size];

  if (data == nullptr)
    {
      // error
      print_error("Memory problems");
    }
  else
    {
      return data;
    }
}
