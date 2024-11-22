#include <stdio.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include "brute.c"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    puts("usage: ./md5 input-file");
    return -1;
  }

  
  char *filename = argv[1];

  FILE* input = fopen(filename, "rb");

  if(!input)
  {
    fprintf(stderr, "cannot open filename: %s", filename);
    return -1;
  }

  fseek(input, 0L, SEEK_END);
  size_t size = ftell(input);
  fseek(input, 0L, SEEK_SET);

  char* buf = (char*)malloc(sizeof(char) * size);
  fread(buf, sizeof(char), size, input);

  if(size != HEX_DIGEST_LENGTH)
  {
    fprintf(stderr, "%s%d%s\n", "Passed file has a string which is not ", HEX_DIGEST_LENGTH, " long. Consider creating file with just cat > input.txt to avoid adding a trailing \\n");
    return -1;
  }

  puts(buf);

  brute_passwd(buf);

  free(buf);
}
