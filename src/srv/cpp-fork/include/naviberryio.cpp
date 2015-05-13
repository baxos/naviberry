#include <iostream>
#include <string>
#include "naviberryio.hpp"

void print_msg(std::string msg)
{
  Color::Modifier TXT_GREEN (Color::FG_GREEN);
  Color::Modifier TXT_DEF (Color::FG_DEFAULT);
  std::cout << TXT_GREEN << "[+] " << msg << TXT_DEF <<std::endl;
}

void print_warning(std::string warning)
{
  Color::Modifier TXT_RED (Color::FG_RED);
  Color::Modifier TXT_DEF (Color::FG_DEFAULT);
  std::cout << TXT_RED << "[-] " << warning << TXT_DEF << std::endl;
}

void print_error(std::string error)
{
  Color::Modifier TXT_RED (Color::FG_RED);
  Color::Modifier TXT_DEF (Color::FG_DEFAULT);
  std::cout << TXT_RED << "ERROR HAPPENED, EXITING PROGRAM" << std::endl;
  std::cout << "[-] " << error << TXT_DEF << std::endl;
  exit(-1);
}
