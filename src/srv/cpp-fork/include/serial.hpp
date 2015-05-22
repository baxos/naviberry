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
  SerialComm(string port);
  void Read(string dest);
  void Write(string data);
}
