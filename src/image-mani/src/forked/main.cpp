#include "img_process.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
  if (argc == 2)
    {
      std::string filename = (std::string) argv[1];
      printf("Trying to load : %s \n", filename.c_str());

      naviberry::Bitmap bmp;
      bmp.Load2(filename);
    }
  
  return 0;
}
