#include "img_process.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

void print_usage();

int main(int argc, char** argv)
{
  if (argc != 2)
    {
      print_usage();
    }
  else
    {
      std::string filename = (std::string) argv[1];
      printf("Trying to load : %s \n", filename.c_str());

      naviberry::Bitmap bmp;


      // load image nd flip it
      bmp.Load2(filename);
      bmp.SaveTo("tester123.bmp");
      bmp.Flip();



      // lets try to display it..

      
      SDL_Renderer *renderer;
      SDL_Window *window;
      SDL_Event event;
      SDL_Surface *screenSurface;
      SDL_Texture *texture;

      std::cout << "[+] sdl_init_video" << std::endl;

      // init sdl 
      if ( SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	  exit(-1);
	}
      
      std::cout << "[+] sdl createwindow and renderer" << std::endl;
      SDL_CreateWindowAndRenderer( bmp.getWidth() , bmp.getHeight(), 0, &window, &renderer);

      if (window == NULL)
	{
	  exit(1);
	}
      else if ( renderer == NULL)
	{
	  exit(1);
	}

      std::cout << "[+] sdl_createtexture" << std::endl;
      texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, bmp.getWidth(), bmp.getHeight());
      
      if (texture == NULL)
	{
	  exit(1);
	}

      std::cout << "[+] updatetexture" << std::endl;
      SDL_UpdateTexture(texture, NULL, bmp.getPixelTable(),bmp.getBytesPerPixel() * bmp.getPitch());
      std::cout << "[+] RenderClear" << std::endl;
      SDL_RenderClear ( renderer );
      std::cout << "[+] RenderCopy" << std::endl;
      SDL_RenderCopy( renderer, texture, NULL, NULL);
      SDL_RenderPresent ( renderer );

      char quit = 0;
      std::cout << "[+] entering main loop" << std::endl;
      while (!quit)
	{
	  SDL_WaitEvent(&event);

	  switch(event.type)
	    {
	      case SDL_QUIT:
		quit = 1;
	      break;
	    }
	}

    }
  
  return 0;
}




void print_usage()
{
  printf("./a.out filename \n");
  exit(-1);
}
