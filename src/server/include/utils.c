#include <stdio.h>



void util_TrimString(char* str)
{
  // List of bad characters if found replace with terminate zero
  char bad_chars[] = {'\r', '\n' };
  
  int i = 0;
  int c = 0;


  printf("Function called string : %s \n", str);

  while (str[i] != 0)
    {
      for (c=0; c < 2; c++)
	{
	  if ( str[i] == bad_chars[c] )
	    {
	      str[i] = 0;
	      break;
	    }
	}

      i++;
    }

}




