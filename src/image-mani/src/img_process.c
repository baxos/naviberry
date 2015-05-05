#include "img_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


// No longer needed ***
unsigned long BytesToDecimal(char* val, size_t byte_size)
{
  // This could should be changed to more automatic process!!!!
  unsigned long result = 0;
	
	if (byte_size == 1)
	{
		return ((long) val);
	}
	else if (byte_size == 2)
	{
		result = (unsigned char) val[0];
		//printf("result += byte[0] == %d \n", (unsigned int) result);

		result += ((unsigned char) val[1] * 256);
		//printf("result += byte[1] == %d \n", (unsigned int) result);
	}
	else if (byte_size == 4)
	{	
		result = (unsigned char) val[0];
		//printf("result += byte[0] == %d \n", (unsigned int) result);

		result += ((unsigned char) val[1] * 256);
		//printf("result += byte[1] == %d \n", (unsigned int) result);

		result += ((unsigned char) val[2] * 256 * 256);
		//printf("result += byte[2] == %d \n", (unsigned int) result);

		result += ((unsigned char) val[3] * 256 * 256 * 256);
		//printf("result += byte[3] == %d \n", (unsigned int) result);
	}

	return result;
}

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


void RemovePadBytes(Bitmap** bmp)
{
  printf("[+]Entering RemovePadBytes \n");
  printf("[+]Setting local variables..\n");
  int pixel_width = (*bmp)->std_bitmap_info.bitmap_pixel_width;
  int pixel_height = (*bmp)->std_bitmap_info.bitmap_pixel_height;
  int curr_res = (*bmp)->image_resolution;
  int new_res = pixel_width * pixel_height;

  // Allocate memory
  printf("[+]Allocating memory..\n");
  int8_t* currBytes = malloc(sizeof(struct BITMAP_STD_PIXEL) * curr_res);
  int8_t* newBytes = malloc(sizeof(struct BITMAP_STD_PIXEL) * new_res);

  // Error check
  if (currBytes == NULL || newBytes == NULL)
    {
      printf("[-]Memory failure..\n");
      exit(1);
    }
  
  // Copy orignal data
  printf("[+]Copying data..\n");
  memcpy(currBytes, (*bmp)->pixel_data, curr_res);

  // Iterate through each row
  // Copy pixel_width data to new array
  // Skip pad bytes..
  int h = 0;
  printf("[+]Entering loop\n");
  for (; h < pixel_height; h++)
    {
      uint32_t oldPos = h * (pixel_width + (*bmp)->numPad);
      uint32_t newPos = h * (pixel_width);
      memcpy(&newBytes[newPos],&currBytes[oldPos], sizeof(StdPixel)*pixel_width);
    }

  // Move data back to orignal place
  printf("[+]Moving back to orignal location\n");
  (*bmp)->pixel_data = realloc((*bmp)->pixel_data, sizeof(struct BITMAP_STD_PIXEL) * new_res );

  (*bmp)->image_resolution = new_res;

  free(newBytes);
  free(currBytes);
}

int CalculateRowSize(int32_t bpi, int32_t width)
{
  double d = ((bpi * width + 31)/32)*4;
  return (int) d;
}

// ================================== Bitmap class starting here.. ==================================== //

// Flips the bitmaps pixel data
void BMP_Flip(Bitmap** bmp)
{
  // Allocate memory on the same size as the current
  printf("[+]Allocating new memory..\n");
  uint8_t* allBytes = malloc(sizeof(struct BITMAP_STD_PIXEL) * (*bmp)->image_resolution);
  uint8_t* newBytes = malloc(sizeof(struct BITMAP_STD_PIXEL) * (*bmp)->image_resolution);

  // Error check
  if (allBytes == NULL || newBytes == NULL)
    {
      printf("[-]Memory failure\n");
      exit(1);
    }

  // move memory
  memmove(allBytes, (*bmp)->pixel_data, (*bmp)->image_resolution);

  // Prepare for copy loop
  int32_t pixel_amount = (*bmp)->image_resolution;
  int32_t index = 0;
 
  // Loop through all pixels
  printf("[+]Entering copy loop\n");

  printf("\t \t \t [] SizeOf pixel %zu \n", sizeof(StdPixel));

  while (index < pixel_amount)
    {
      StdPixel* p = malloc(sizeof(StdPixel));
      memcpy(p, &allBytes[pixel_amount-index], sizeof(StdPixel));
      uint8_t tmp = p->RED;
      p->RED = p->BLUE; // flip R nd B
      p->BLUE = tmp;
      memcpy(&newBytes[index], p, sizeof(StdPixel));
      index += 3;
      free(p);
    }
  printf("[+]Moving data back..\n");
  memmove((*bmp)->pixel_data, newBytes, (*bmp)->image_resolution);
  printf("[+]Freeing memory..\n");
  free(allBytes);
  free(newBytes);
  printf("[+]Done..\n");
}

void BMP_Load(char* filename, Bitmap** bmp)
{
  const size_t HEADER_SIZE = 14;

  FILE* pFile = NULL;
  char* buffer = NULL;
  


  // if bmp provided is not null quit!
  if ((*bmp) != NULL)
    {
      printf("[-]Wrong arguments recieved!\n");
      exit(1);
    }

  // Try open file
  pFile = fopen(filename, "r");
  
  // if file doesn't open, quit the program	
  if (pFile == NULL)
    {!
      printf("[-]Some error occured openening file : %s \n", filename);
      exit(1);
    }
  
  // File is now open, prepare it for being read.
  (*bmp) = malloc(sizeof(Bitmap));
  if ((*bmp) == NULL)
    {
      printf("[-]Error asking for ram\n");
      exit(1);
    }

  
  // Zero out, all memory in struct before using it!
  memset(&(*bmp)->bitmap_header, 0, 14);
  memset(&(*bmp)->std_bitmap_info, 0, 40);  
  
  // Read fist X[HEADER_SIZE] of bytes
  buffer = malloc(HEADER_SIZE);

  size_t bytes_read = fread(buffer, 1, HEADER_SIZE, pFile);
  
  // Check if size matches with what we actually read!
  if (bytes_read < HEADER_SIZE)
    {
      printf("[-]Something went wrong reading file header! \n");
      exit(1);
    }
  
  // Copy data to our struct
  // Done handling the first header of the file ! Hooorray!!
  memcpy(&(*bmp)->bitmap_header, buffer, 14);

  // Re-allocate buffer, now we just read the next 4 bytes. 
  // To confirm it's the correct version 
  buffer = realloc(buffer, 4);
  fread(buffer, 1, 4, pFile);	
	
  if(buffer[0] != 0x28)
    {
      printf("[-]NON-SUPPORTED Bitmap version..\n");
      exit(1);
    }


  // Now we re-allocate again this time to the 40 bytes
  // the std_header fills
  // load it and add it to structure
  // Apply buffer size
  size_t header_size =(char)  buffer[0];

  buffer = realloc(buffer, header_size);
  fseek(pFile, 14, SEEK_SET);
  fread(buffer, 1, header_size, pFile);
  memcpy(&(*bmp)->std_bitmap_info, buffer, sizeof((*bmp)->std_bitmap_info));
  // Done using buffe for now..
  free(buffer);
  
  // Dump headers to console
  DumpStdHeader(&(*bmp)->std_bitmap_info);
  DumpBitmapHeader(&(*bmp)->bitmap_header);


  // Prepare to load pixel data
  BOOL     img_padding = FALSE;
  int32_t  img_height = (*bmp)->std_bitmap_info.bitmap_pixel_height;
  int32_t  img_width = (*bmp)->std_bitmap_info.bitmap_pixel_width;
  int32_t img_realWidth = img_width;
  int32_t  img_byte_count = (*bmp)->std_bitmap_info.bitmap_bit_count / 8; // How many bytes per pixel
  int32_t img_res = img_height * (img_width * img_byte_count);
  (*bmp)->numPad = 0;
  // If width is not dividible with 4, there is zero padding
  // So align for that
  if ( img_realWidth % 4 != 0)
    {
      img_padding = TRUE;
      printf("[+] Padding calculations..\n");
      img_realWidth = CalculateRowSize(img_byte_count*8, img_width);
      printf("\t[+] RowSize  : %d \n",img_realWidth);
      printf("\t[+] Padding  : %d \n", img_realWidth-(img_width*3));
      (*bmp)->numPad = img_realWidth - (img_width * 3 );
      img_res = img_height * img_realWidth;
    }


  // Add to bitmap structure
  (*bmp)->image_resolution = img_res;
  

  // Allocate memory to store image in
  printf("[+]Trying to reserve : %d amount of bytes\n", img_res);
  buffer = malloc(img_res);
  if (buffer == NULL)
    {
      perror("[-]Error reserving memory\n");
      exit(1);
    }
  printf("[+]Done\n");

  
  // Zeroset memory and start reading the data and store them in memory
  // ** Edit hardcoded offset in fseek, to header value
  memset(buffer, 0, img_res); 
  fseek(pFile, 54, SEEK_SET); // <-- [hardcoded] , Change this later.. // Change is needed to automatic handle the offset 
  
  unsigned long int n;
  n = fread(buffer, 1, img_res+1, pFile); // Read img_res + 1 for (EOF)
  
  // Error checking, check for end of file AND if we had any errors while reading the content
  // * Add error check : cmp info_header + bmp_header + pixel array is equal to file_size
  if (feof(pFile))
    printf("[-]End of file reached..%lu bytes read\n", n);
  
  // Check file for read error
  if (ferror(pFile))
    {
      perror("[-]Some error has eccoured\n");
    }

  // Compare we allocated same memory amount as we read
  if (img_res != n)
    {
      printf("[-]Memory allocated and memory read from file, differs in size!\n");
      exit(1);
    }
  
  
  
  // Create an equal size amount of memory, but as a structure..
  // ** Need change to bitmap_std_pixel ARGB
  (*bmp)->pixel_data =  malloc(sizeof(struct BITMAP_STD_PIXEL) * (img_res/img_byte_count));
  if ((*bmp)->pixel_data == NULL)
    {
      perror("[-]Some memory error has occured..\n");
    }
  else
    {
      printf("[+]Copying to a structured buffer..\n");
      memcpy((*bmp)->pixel_data, buffer, img_res);
      printf("[+]Done..\n");
    }
  
  // Free byte buffer memory [name-chage]
  free(buffer);
  
  // Is padding used? If so, remove them.
  if (img_padding)
    {
      RemovePadBytes(&(*bmp));
    } 

  

  // everything is done
  // No corruption that we know of has happened when we are here~
  (*bmp)->corrupted = FALSE;

  BMP_Flip(&(*bmp));

  // Done	
}
