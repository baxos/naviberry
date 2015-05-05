/* This program is written by Jan Emil Bacher anno 2o14
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "img_process.h"
#include "stack.h"

int main(int argc, char* argv[])
{
	const int HEADER_SIZE = 14;
	FILE* pImageFile;
	struct BITMAP_HEADER bit_header;
	struct BITMAP_STD_INFO_HEADER_NEW info_header;
	char* pImageHeader;

	printf("[+]sizeof new struct : %zu \n", sizeof(struct BITMAP_STD_INFO_HEADER_NEW));
	
	// If arguments are less than 2, just quit and tell user how do use the god damn program!
	if (argc < 2)
	{
		printf("Usage : %s FILENAME.bmp \n", argv[0]);
		exit(1);
	}

	

	// A renderer for drawing pixels to the screen
       	SDL_Renderer *renderer;
	// An event listener for quitting the image
	SDL_Event event;
	//The window we'll be rendering to 
	SDL_Window* window = NULL; 
	//The surface contained by the window 
	SDL_Surface* screenSurface = NULL;
	

	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	  { 
	    printf( "[-]SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
	    exit(1);;
	  }
	 
	Bitmap* bmp = NULL;
	BMP_Load(argv[1], &bmp);

	if (bmp == NULL)
	  {
	    printf("[-]ERROR LOADING BMP\n");
	    exit(1);
	  }
	if (bmp->corrupted==TRUE)
	  {
	    printf("[-]IMAGE CORRUPTED\n");
	    exit(1);
	  }

	// Now everything is handled and we can draw the picture
	//Create window 	
	int32_t img_width = bmp->std_bitmap_info.bitmap_pixel_width;
	int32_t img_height = bmp->std_bitmap_info.bitmap_pixel_height;
	int32_t img_res = img_width * img_height * 3;

	printf("[+]Trying to create window. \t Width : %d \t Height : %d \n", img_width, img_height);
	SDL_CreateWindowAndRenderer(img_width, img_height, 0, &window, &renderer);
	
	if( window == NULL ) 
	  {
	    printf( "[-]Window could not be created! SDL_Error: %s\n", SDL_GetError() );
	    exit(0);
	  }
	
	printf("[+]Window created!\n");
	// For BMP STD Header default color used is RGB 8Bit resolution ( 3x8 = 24bit total )
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, img_width, img_height);
	// Update the screen
	printf("[+]Updating texture from bmp->data_pixels\n");
	printf("\t[+]Pitch \n");
	printf("\t\t[+]Byte per pixel : %d \n", bmp->std_bitmap_info.bitmap_bit_count/8);
	printf("\t\t[+]Image width : %d \n", img_width);
	SDL_UpdateTexture(texture, NULL, bmp->pixel_data, img_width*(bmp->std_bitmap_info.bitmap_bit_count/8));
	printf("[+]Done\n");

	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, texture, NULL, NULL);
	SDL_RenderPresent( renderer);
	
	char quit =0 ;
	
	while (!quit)
	  {
	    SDL_WaitEvent(&event);
	    
	    switch(event.type)
	      {
	      case SDL_QUIT:
		quit=1;
		break;
	      }
	  }
	
	
	// Clean up memory and exit nicely
	SDL_DestroyTexture ( texture );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window ); 
	
	// Free the buffer,we are about to close
	SDL_Quit();
	return 0;
	
}
