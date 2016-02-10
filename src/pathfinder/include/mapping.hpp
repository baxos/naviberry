#ifndef MAPPING__HPP
#define MAPPING__HPP

#include <cstdint>
#include <vector>

using namespace std;



class Map
{
private:
  vector< vector<uint8_t> > map;
  int xsize, ysize;
public:
  vector< vector<uint8_t> > getMap();
  void setTile(int x, int y, uint8_t val);
  void GenerateEmptyMap();
  void  GenerateMazeMap();
  Map(int x, int y);
};



#endif
