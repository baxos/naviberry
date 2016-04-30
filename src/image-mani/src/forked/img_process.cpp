#include "img_process.hpp"

#include <iostream>
#include <vector>
#include <stack>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*

void DumpBitmapHeader(struct BITMAP_HEADER *bitmap_header)
{
  unsigned long int PIXEL_OFFSET = BytesToDecimal(bitmap_header->PIXEL_OFFSET, sizeof(bitmap_header->PIXEL_OFFSET));
  unsigned long int FILE_SIZE = BytesToDecimal(bitmap_header->FILE_SIZE, sizeof(bitmap_header->FILE_SIZE));
  printf("[+]BIT HEADER\n");
  printf("\t[+]HEADER_FIELD = %c%c \n", bitmap_header->HEADER_FIELD[0], bitmap_header->HEADER_FIELD[1]);
  printf("\t[+]PIXEL_OFFSET = %lu \n", PIXEL_OFFSET);
  printf("\t[+]FILE SIZE = %lu bytes \n", FILE_SIZE);
}

void DumpStdHeader(struct BITMAP_STD_INFO_HEADER_NEW *info_header)
{
  struct BITMAP_STD_INFO_HEADER header;
  int32_t image_width = info_header->bitmap_pixel_width;
  int32_t image_height = info_header->bitmap_pixel_height;
  
  uint32_t image_header_size = info_header->HEADER_SIZE;
  uint32_t image_size = info_header->bitmap_image_size;
  
  int16_t image_color_planes = info_header->bitmap_color_planes;
  int16_t image_bit_count = info_header->bitmap_bit_count;
  int32_t image_compression = info_header->bitmap_compression;
  int32_t image_hori_res = info_header->bitmap_hori_res;
  int32_t image_vert_res = info_header->bitmap_vert_res;
  uint32_t image_colors = info_header->bitmap_colors_used;
  uint32_t image_important_colors = info_header->bitmap_important_colors_used;
  
  printf("[+]Dumping bitmap header\n");
  printf("\t[+]Image width = %d pixels\n", image_width);
  printf("\t[+]Image height = %d pixels\n", image_height);
  printf("\t[+]Image header file size = %d bytes\n", image_header_size);
  printf("\t[+]Image size = %d bytes\n", image_size);
  printf("\t[+]Image color planes = %d bytes\n", image_color_planes);
  printf("\t[+]Image bit count = %d bytes\n", image_bit_count);
  printf("\t[+]Image compression = %d bytes\n", image_compression);
  printf("\t[+]Image hori resolution = %d bytes\n", image_hori_res);
  printf("\t[+]Image vert resolution = %d bytes\n", image_vert_res);
  printf("\t[+]Image colors = %d bytes\n", image_colors);
  printf("\t[+]Image important colors = %d bytes\n", image_important_colors);
}
*/


using namespace naviberry;

void Bitmap::RemovePadBytes()
{
  printf("[+]Entering RemovePadBytes \n");
  printf("[+]Setting local variables..\n");
  int pixel_width = width;
  int pixel_height = height;
  int curr_res = resolution;
  int new_res = pixel_width * pixel_height;

  // Allocate memory
  printf("[+]Allocating memory..\n");

  uint8_t* currBytes = new uint8_t[ sizeof(struct BITMAP_STD_PIXEL) * curr_res];
  uint8_t* newBytes  = new uint8_t[ sizeof(struct BITMAP_STD_PIXEL) * new_res];


  // Error check
  if (currBytes == nullptr || newBytes == nullptr)
    {
      printf("[-]Memory failure..\n");
      exit(1);
    }
  
  // Copy orignal data
  printf("[+]Copying data..\n");
  memcpy(currBytes, pixel_data, curr_res);

  // Iterate through each row
  // Copy pixel_width data to new array
  // Skip pad bytes..
  int h = 0;
  printf("[+]Entering loop\n");
  for (; h < pixel_height; h++)
    {
      uint32_t oldPos = h * (pixel_width + padbytes);
      uint32_t newPos = h * (pixel_width);
      memcpy(&newBytes[newPos],&currBytes[oldPos], sizeof(StdPixel)*pixel_width);
    }

  // Move data back to orignal place
  printf("[+]Moving back to orignal location\n");


  delete pixel_data;
  pixel_data = new uint8_t[sizeof(struct BITMAP_STD_PIXEL) * new_res];

  std::memcpy(pixel_data, newBytes, sizeof(struct BITMAP_STD_PIXEL) * new_res);

  resolution = new_res;

  delete currBytes;
  delete newBytes;
}


int32_t Bitmap::CalculateRowSize(int32_t bpi, int32_t width)
{
  double d = ((bpi * width + 31)/32)*4;
  return (int32_t) d;
}


// Flips the bitmaps pixel data
void Bitmap::Flip()
{
  // Bitmap::Flip2()

  std::stack<Byte> bytestack();

  /*
  // Allocate memory on the same size as the current
  printf("[+]Allocating new memory..\n");
  uint8_t* allBytes = new uint8_t[resolution];
  uint8_t* newBytes = new uint8_t[resolution];
  // uint8_t* allBytes = malloc(sizeof(struct BITMAP_STD_PIXEL) * (*bmp)->image_resolution);
  // uint8_t* newBytes = malloc(sizeof(struct BITMAP_STD_PIXEL) * (*bmp)->image_resolution);

  // Error check
  if (allBytes == nullptr || newBytes == nullptr)
    {
      printf("[-]Memory failure\n");
      exit(1);
    }

  // move memory
  memmove(allBytes, pixel_data, resolution);

  // Prepare for copy loop
  int32_t pixel_amount = resolution;
  int32_t index = 0;
 
  // Loop through all pixels
  printf("[+]Entering copy loop\n");

  printf("\t \t \t [] SizeOf pixel %zu \n", sizeof(StdPixel));

  while (index < pixel_amount)
    {
      StdPixel p;
      memcpy(&p, &allBytes[pixel_amount-index], sizeof(StdPixel));
      uint8_t tmp = p.RED;
      p.RED = p.BLUE; // flip R nd B
      p.BLUE = tmp;
      memcpy(&newBytes[index], &p, sizeof(StdPixel));
      index += 3;
    }
  printf("[+]Moving data back..\n");
  memmove(pixel_data, newBytes, resolution);
  printf("[+]Freeing memory..\n");

  delete allBytes;
  delete newBytes;

  printf("[+]Done..\n");
  */
}

typedef uint8_t Byte;

bool Bitmap::UsesPadding()
{
  if (width % 4 != 0)
    {
      return true;
    }
  else
    {
      return false;
    }
}

void Bitmap::Load2(std::string filename)
{
  // Get file size of the image
  struct BITMAP_HEADER bmp_header;
  struct BITMAP_STD_INFO_HEADER_NEW bmp_infoheader;

  bool file_verified = false;
  struct stat fst;
  size_t file_size;

  Byte* buffer;

  if ( stat(filename.c_str(), &fst) != 0)
    {
      // File error, does the file exsist?
      // we have the correct rights?
    }
  else
    {
      file_size = fst.st_size;
      file_verified = true;
    }


  // Ok, now we now that 
  //     1) Whether the file exsist or not
  //     2) The size of the file
  if (file_verified)
    {
      FILE* file = fopen(filename.c_str(), "rb");
      if (file == nullptr)
	{
	  printf("[-] Error opening the file \n");
	  exit(-1);
	}


      buffer = new Byte[file_size];
      if (buffer == nullptr)
	{
	  printf("[-] Memory error. try restart \n");
	  exit(-1);
	}


      // We want the block_size as high as possible
      // for make as little as possible system calls
      // fread dont like numbers given above 32 bit
      size_t loaded_datasize = 0;
      const auto block_size = 65535;
      while (loaded_datasize < file_size)
	{
	  size_t n = 0;
	  if (loaded_datasize + block_size >= file_size)
	    {	    
	      auto new_block_size = file_size - loaded_datasize;
	      printf("[+] Calculating new block size : %lu \n", new_block_size);
	      n = fread(&buffer[loaded_datasize], 1, new_block_size, file);
	    }
	  else
	    {
	      n = fread(&buffer[loaded_datasize], 1, block_size, file);
	    }
	  loaded_datasize += n;
	}


      // confirm file size
      if (loaded_datasize != file_size)
	{
	  printf("[-] Difference in the file size and the data sized loaded \n");
	  printf("[-] %zu \t != %zu \n", file_size, loaded_datasize);
	  fclose(file);
	  exit(-1);
	}
      else
	{
	  printf("[+] All data is loaded, and size is verified \n");
	}

 
      // check whether its correct version
      if (buffer[14] != 0x28)
	{
	  printf("[-] Non-valid bitmap version. \n");
	  exit(-1);
	}

      std::memcpy(&bmp_header, &buffer[0], 14);
      std::memcpy(&bmp_infoheader, &buffer[14], sizeof(bmp_infoheader));

      // copy data to class

      auto realWidth = CalculateRowSize(bmp_infoheader.bitmap_bit_count, bmp_infoheader.bitmap_pixel_width);



      height = bmp_infoheader.bitmap_pixel_height;
      padbytes =  0;
      width = bmp_infoheader.bitmap_pixel_width;
      resolution = width * height;
      corrupted = false;
      

      if (UsesPadding() == true)
	{
	  padbytes = bmp_infoheader.bitmap_pixel_width - realWidth;
	}


      printf("[+] Image dimensions : %d x %d \n", height, width);
      printf("[+] Resolution       : %d bytes \n[+] Padbytes         : %d byte(s)\n", resolution, padbytes);

      delete buffer; 
    }
}

