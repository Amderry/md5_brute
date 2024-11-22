#include <math.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>
#include <openssl/md5.h>

#define HEX_DIGEST_LENGTH MD5_DIGEST_LENGTH * 2
#define PASSWD_LENGTH 5 
#define ASCII_COUNT 95
//#define SIMPLE_DEBUG
//#define SLEEP

typedef unsigned long long int uint_64;

int md5_hash(char* str, int length, char* md5string)
{
  unsigned char digest[MD5_DIGEST_LENGTH];

  MD5_CTX context;
  MD5_Init(&context);
  MD5_Update(&context, str, length);
  MD5_Final(digest, &context);

  for(int i = 0; i < MD5_DIGEST_LENGTH; ++i)
  {
    sprintf(&md5string[i * 2], "%02x", (unsigned int)digest[i]);
  }
  return 1;
}

int brute_passwd(char* hash)
{
  for(int i = 1; i <= PASSWD_LENGTH; ++i)
  {
    printf("%s%d\n", "Str length: ", i);
    uint_64 iters = pow(ASCII_COUNT, i);
    uint_64 *pows = (uint_64*)malloc(sizeof(uint_64) * i);
    for(int p = 0; p < i; ++p)
    {
      pows[p] = pow(ASCII_COUNT, p); 
    }
  
    #pragma omp parallel for
    for(uint_64 j = 0; j < iters; ++j)
    {
      char passwd[PASSWD_LENGTH + 1];
        
      for(int i = 0; i < PASSWD_LENGTH; ++i)
      {
         passwd[i] = '\0';
      }

      for(int p = i - 1; p >= 0; --p)
      {
          passwd[p] = (j / pows[p]) % ASCII_COUNT + 33;
      }
      char md5string[HEX_DIGEST_LENGTH];
      md5_hash(passwd, i, md5string);
#ifdef SIMPLE_DEBUG
      printf("%s%s%s%s\n", "Passwd: ", passwd, " MD5:", md5string);
#endif
#ifdef SLEEP
      sleep(1);
#endif
      if(!strcmp(md5string, hash))
      {
        printf("%s%s\n", "Password bruted: ", passwd);
        exit(0);
      }
    }
      free(pows);
  }
  return 0;
}

void check_combination(char* passwd, char* hash, int i, uint_64 j, uint_64* pows)
{
  for(int p = i - 1; p >= 0; --p)
  {
    if(!(j % pows[p]))
    {
#pragma omp critical
      passwd[p] = (j / pows[p]) % ASCII_COUNT + 33;
      break;
    }
  }
  char md5string[HEX_DIGEST_LENGTH];
  md5_hash(passwd, i, md5string);
#ifdef SIMPLE_DEBUG
  printf("%s%s%s%s\n", "Passwd: ", passwd, " MD5:", md5string);
#endif
#ifdef SLEEP
  sleep(1);
#endif
  if(!strcmp(md5string, hash))
  {
    printf("%s%s\n", "Password bruted: ", passwd);
    return;
  }
}
