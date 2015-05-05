#ifndef IMG_PROCESS_H_  /* Include guard */
#define IMG_PROCESS_H_
#include "img_process.h"
#include "bool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

// Global Functions

unsigned long BytesToDecimal(char* val, size_t byte_size);
void DumpV4Header(struct BITMAP_STD_INFO_HEADER_NEW *info_header);
void DumpStdHeader(struct BITMAP_STD_INFO_HEADER_NEW *info_header);


// Our Bitmap

struct BITMAP
{
  struct BITMAP_HEADER bitmap_header;
  struct BITMAP_STD_INFO_HEADER_NEW std_bitmap_info;
  struct BITMAP_STD_PIXEL* pixel_data;
  int32_t width;
  int32_t height;
  int32_t widthBytes;
  int8_t numPad;
  int32_t image_resolution;
  BOOL corrupted;
};
typedef struct BITMAP Bitmap;

void BMP_Create(uint32_t height, uint32_t width, Bitmap* bmp);
void BMP_Load(char* filename, Bitmap** bmp);
void BMP_Flip(Bitmap** bmp);


#endif
