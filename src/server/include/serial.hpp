#include <iostream>
#include <string>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

class SerialComm
{
private:
  int sfd;
  bool ready;
public:
  SerialComm(std::string port);
  std::string Read();
  void Write(std::string data);
};
