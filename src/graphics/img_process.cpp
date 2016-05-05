#include "img_process.hpp"

#include <iostream>
#include <cassert>
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
  std::stack<uint8_t> bytestack;

  for (auto i = 0; i < this->total_pixel_memory; i++)
    {
      bytestack.push(this->pixel_data[i]);
    }
  for (auto i = 0; i < this->total_pixel_memory; i++)
    {
      this->pixel_data[i] = bytestack.top();
      bytestack.pop();
    }

}


int32_t Bitmap::getWidth()
{
  return this->width;
}

int32_t Bitmap::getHeight()
{
  return this->height;
}

int32_t Bitmap::getPitch()
{
  return this->width + this->padbytes;
}

uint8_t* Bitmap::getPixelTable()
{
  return this->pixel_data;
}

int8_t Bitmap::getBytesPerPixel()
{
  return this->bytes_per_pixel;
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

bool Bitmap::SaveTo(std::string filename)
{
  bool permission_flag = true;
  bool fileOK_flag = true;
  bool verified_flag = false;
  size_t expected_filesize = 0;
  struct stat fst;

  // check whether file exists already
  if ( stat(filename.c_str(), &fst) != 0)
    {
      // no file
    }
  else
    {
      // file already exists, no save nless its forced
      printf("\t[-] File already exist. Please use force save or choose other filename. \n");
      fileOK_flag = false;
    }


  if ( (fileOK_flag == true) && (permission_flag == true))
    {
      FILE* fp;
      fp = fopen(filename.c_str(), "wb");

      // calculate expected file size
      expected_filesize += sizeof(struct BITMAP_HEADER);
      expected_filesize += sizeof(struct BITMAP_STD_INFO_HEADER_NEW);
      expected_filesize += total_pixel_memory;



      // start by writing header
      printf("\t[+] Writing header to file. \n");
      auto n = fwrite(&this->header, 1, sizeof(struct BITMAP_HEADER), fp);
      assert(n == sizeof(struct BITMAP_HEADER));


      // then information header..
      printf("\t[+] Writing information header to file. \n");
      n = fwrite(&this->infoheader, 1, sizeof(struct BITMAP_STD_INFO_HEADER_NEW), fp);
      assert( n == sizeof(struct BITMAP_STD_INFO_HEADER_NEW));
       
	     
      // finally pixel table
      // bug in code, it wont write all the bytes in one call
      printf("\t[+] Writing the pixel table, total amount of bytes : %d \n", total_pixel_memory);
      auto written_bytes = 0;
      auto block_size = 1024;
      while (written_bytes < total_pixel_memory)
	{       
	  if (written_bytes + block_size > total_pixel_memory)
	    {
	      // calcuate new block size, write nd done..
	      auto temp = block_size;
	      block_size = total_pixel_memory - written_bytes;
	      n = fwrite(&this->pixel_data[written_bytes], 1, block_size, fp);
	      written_bytes += n;
	      printf("\t[+] Calculated new block size : %d old was : %d \n", block_size, temp);

	    }
	  else
	    {
	      n = fwrite(&this->pixel_data[written_bytes], 1, block_size, fp);
	      written_bytes += block_size;
	    }
	}
      assert(written_bytes == total_pixel_memory);


      // close
      fclose(fp);



      // check file again..
      if ( (stat(filename.c_str(), &fst)) != 0)
	{
	  // no file, some bad has happened
	  verified_flag = false;
	}
      else
	{
	  // file exist, lets check size
	  auto file_actual_size = fst.st_size;
	  if (expected_filesize == file_actual_size)
	    {
	      verified_flag = true;
	      printf("\n[+] File is saved and verified. \n");
	    }
	  else
	    {
	      verified_flag = false;
	      printf("\t[-] Expected file size and actual file size is different.\n");
	      printf("\t[-] Expected size : %zu bytes \n", expected_filesize);
	      printf("\t[-] Actual size   : %zu bytes \n", file_actual_size);
	    }
	}
    }



  if (verified_flag)
    {
      // file is correctly saved.
      return true;
    }
  else
    {
      // file is corrupted or not saved at all.
      return false;
    }

  return false;
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
	  if ( block_size <= file_size - loaded_datasize)
	    {
	      // load one block
	      auto n = fread(&buffer[loaded_datasize], 1, block_size, file);
	      loaded_datasize += n;
	    }
	  else
	    {
	      // calculate new block size
	      auto new_block_size = file_size - loaded_datasize;
	      auto n = fread(&buffer[loaded_datasize], 1, new_block_size, file);
	      loaded_datasize += n;
	    }
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
      else
	{
	  printf("[+] Valid bitmap version. Continueing.. \n");
	}


      printf("[+] Extracting bitmap header. \n");
      auto current_offset = 0;
      std::memcpy(&bmp_header, &buffer[current_offset], 14);
      current_offset += 14;
      
      printf("[+] Extracting information header. \n");
      std::memcpy(&bmp_infoheader, &buffer[current_offset], sizeof(bmp_infoheader));
      current_offset += sizeof(bmp_infoheader);


      height = bmp_infoheader.bitmap_pixel_height;
      padbytes =  0;
      width = bmp_infoheader.bitmap_pixel_width;
      resolution = width * height;
      corrupted = false;
      

      if (UsesPadding() == true)
	{
	  while ( (bmp_infoheader.bitmap_pixel_width + padbytes) % 4 != 0)
	    {
	      padbytes++;
	    } 
	}

      auto bpp = bmp_infoheader.bitmap_bit_count;
      bytes_per_pixel = bpp / 8;
     


      // allocate memory for pixel table
      printf("[+] Allocating memory and extracting pixel array. \n");
      auto total_memory = height * ( (width+padbytes) * bytes_per_pixel);
      this->total_pixel_memory = total_memory;
      this->pixel_data = new uint8_t[ total_memory ];

      if (this->pixel_data == nullptr)
	{
	  exit(1);
	}

      printf("\t[+] Memory size of pixel table : %d bytes. \n", total_memory);
      printf("\t[+] Buffer size : %zu \t offset : %d \n", file_size, current_offset);
      std::memcpy (&this->pixel_data[0], &buffer[current_offset], total_memory);


      printf("[+] Image dimensions : %d x %d \n", height, width);
      printf("[+] Resolution       : %d bytes \n[+] Padbytes         : %d byte(s)\n", resolution, padbytes);
      printf("[+] Bytes per pixel  : %d byte(s). \n", bytes_per_pixel);


      header = bmp_header;
      infoheader = bmp_infoheader;

      delete buffer; 
    }
}

