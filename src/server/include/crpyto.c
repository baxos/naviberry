#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void GenerateEncryptionKey();

int main()
{
  unsigned char* test;

  GenerateEncryptionKey(&test);
  
  int i;
  for (i = 0; i < 256; i++)
    {
      printf("0x%x \t", test[i]);
    }
  printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

  free(test);
  return 0;
}

void GenerateEncryptionKey(unsigned char** key)
{
  // Key size
  const size_t key_size = 256;
  // The key
  unsigned char* encrypt_key = NULL;

  // Allocate memory
  encrypt_key = malloc(sizeof(unsigned char) * key_size);


  // First we fill up the key with 0..255
  int i;
  for (i=0; i < key_size; i++)
    {
      encrypt_key[i] = i;
    }
  
  // Now we start shaking the key
  srandom(time(NULL));

  int counter = random()%10000 + 1000;
  while (counter>0)
    {
      int curr_pos = random()%255;
      int next_pos = random()%255;

      unsigned char buffer = encrypt_key[curr_pos];
      encrypt_key[curr_pos] = encrypt_key[next_pos];
      encrypt_key[next_pos] = buffer;

      counter--;
    }

  // Apply to input ptr
  (*key) = encrypt_key;
}
