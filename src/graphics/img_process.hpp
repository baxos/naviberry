#ifndef IMG_PROCESS_HPP_  /* Include guard */
#define IMG_PROCESS_HPP_

// current new include
#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>



typedef uint8_t Byte;

struct BITMAP_HEADER
{
  char HEADER_FIELD[2];
  char FILE_SIZE[4];
  char FILE_SIZE_EXTENDED_1[2];
  char FILE_SIZE_EXTENDED_2[2];
  char PIXEL_OFFSET[4];
};

struct BITMAP_HEADER_NEW
{
  uint8_t HEADER_FIELD[2];
  uint32_t FILE_SIZE;
  int16_t FILE_SIZE_EXTENDED_1;
  int16_t FILE_SIZE_EXTENDED2;
  uint32_t PIXEL_OFFSET;
};

struct BITMAP_STD_INFO_HEADER
{
  char HEADER_SIZE[4];
  char bitmap_pixel_width[4];
  char bitmap_pixel_height[4];
  char bitmap_color_planes[2];
  char bitmap_bit_count[2];
  char bitmap_compression[4];
  char bitmap_image_size[4];
  char bitmap_hori_res[4];
  char bitmap_vert_res[4];
  char bitmap_colors_used[4];
  char bitmap_important_colors_used[4];
};

struct BITMAP_STD_INFO_HEADER_NEW
{
  uint32_t HEADER_SIZE;
  int32_t bitmap_pixel_width;
  int32_t bitmap_pixel_height;
  int16_t bitmap_color_planes;
  int16_t bitmap_bit_count;
  int32_t bitmap_compression;
  uint32_t bitmap_image_size;
  int32_t bitmap_hori_res;
  int32_t bitmap_vert_res;
  uint32_t bitmap_colors_used;
  uint32_t bitmap_important_colors_used;
};

struct BITMAP_V4_INFO_HEADER
{
  char HEADER_SIZE[4];
  char bitmap_pixel_width[8];
  char bitmap_pixel_height[8];
  char bitmap_color_planes[2];
  char bitmap_bit_count[2];
  char bitmap_compression[4];
  char bitmap_image_size[4];
  char bitmap_hori_res[8];
  char bitmap_vert_res[8];
  char bitmap_colors_used[4];
  char bitmap_important_colors_used[4];
  // MORE TO COME!!!!
  // SEE http://msdn.microsoft.com/en-us/library/windows/desktop/dd183380%28v=vs.85%29.aspx
};


struct BITMAP_V4_PIXEL
{
  // RGB - 3 Bytes from each pixel [ 24 bit in total ]
  uint8_t RED;
  uint8_t GREEN;
  uint8_t BLUE;
};

struct BITMAP_STD_PIXEL
{
  uint8_t  RED;
  uint8_t  GREEN;
  uint8_t  BLUE;
};
typedef struct BITMAP_STD_PIXEL StdPixel; 



namespace naviberry
{
  
  class Bitmap
  {
  private:
    int32_t width;
    int32_t height;
    int32_t widthBytes;
    int8_t padbytes;
    int32_t resolution;
    int32_t total_pixel_memory;
    int8_t bytes_per_pixel;
    bool corrupted;
    uint8_t* pixel_data;
    void RemovePadBytes();
    bool UsesPadding();
  public:
    static Bitmap CreateNew(int width, int height);
    int32_t CalculateRowSize(int32_t bpi, int32_t width);
    void Load(std::string filename);
    void Load2(std::string filename);
    void Flip();
    int32_t getWidth();
    int32_t getHeight();
    int32_t getPitch();
    int8_t getBytesPerPixel();
    uint8_t* getPixelTable();

  };

}


#endif
