
/*
封装加密算法
@arrow 20140910
 */
#include <string.h>
#include "Dodxx.h"

#define DELTA 0x9e3779b9
#define MX ((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z));

/************ static functions common to all instances ***********************/

//static inline void handle_interrupts(timer16_Sequence_t timer, volatile uint16_t *TCNTn, volatile uint16_t* OCRnA)
//{
//}


/****************** end of static functions ******************************/

Dodxx::Dodxx()
{
}


static void btea(uint32_t *v, int n, uint32_t *k) {
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1) {          /* Coding Part */
      rounds = 6 + 52/n;
      sum = 0;
      z = v[n-1];
      do {
        sum += DELTA;
        e = (sum >> 2) & 3;
        for (p=0; p<n-1; p++)
          y = v[p+1], z = v[p] += MX;
        y = v[0];
        z = v[n-1] += MX;
      } while (--rounds);
    } else if (n < -1) {  /* Decoding Part */
      n = -n;
      rounds = 6 + 52/n;
      sum = rounds*DELTA;
      y = v[0];
      do {
        e = (sum >> 2) & 3;
        for (p=n-1; p>0; p--)
          z = v[p-1], y = v[p] -= MX;
        z = v[n-1];
        y = v[0] -= MX;
      } while ((sum -= DELTA) != 0);
    }
  }

void Dodxx::calHash(unsigned char *inputText, unsigned int inputTextLength, unsigned char *hash)
{
  unsigned int numBlocks = (inputTextLength <= HASH_LENGTH ? 1 : inputTextLength/HASH_LENGTH), i,j;
  unsigned char H[HASH_LENGTH]={HASH_IV};
  unsigned char tempBuffer[BLOCK_SIZE];

  for (i=0; i<numBlocks;i++){
     memcpy(&tempBuffer,inputText,BLOCK_SIZE);
     doxx((unsigned char *)&tempBuffer,KEY_SIZE,(uint32_t *)&H);

     // Se actualiza el puntero de la entrada, x_i
     for (j=0; j<HASH_LENGTH;j++)
         H[j]=tempBuffer[j]^(*inputText++);
  }

  memcpy(hash,&H,HASH_LENGTH);
}

uint32_t* Dodxx::genxx(const char* szRand)
{
	static uint8_t aucKey[HASH_LENGTH];
	memset(aucKey, 0, sizeof(aucKey));

	calHash((unsigned char *)szRand, strlen(szRand), aucKey);
	return (uint32_t*)aucKey;
}

int Dodxx::doxx(unsigned char *inputText, unsigned int inputTextLength, uint32_t *k)
{ 
  unsigned int numBlocks = (inputTextLength <= BLOCK_SIZE ? 1 : inputTextLength/BLOCK_SIZE);
  unsigned int offset, i;

  if (inputTextLength < BLOCK_SIZE)	{
		return -1;
  }

  // Padding if necessary till a full block size
  //if ((offset = inputTextLength % BLOCK_SIZE) != 0)
  //	  memset(inputText+inputTextLength, 0x00, BLOCK_SIZE - offset);

  //超过BLock剩余的字节不做任何处理
  #if 0
  for (i=0; i<numBlocks;i++){
     btea((uint32_t *)inputText, BLOCK_SIZE/4,k);
     inputText+=BLOCK_SIZE;
  }
  #endif

  int iEncLen = numBlocks * BLOCK_SIZE;
  btea((uint32_t *)inputText, iEncLen/4, k);
    
  return numBlocks;
}

int Dodxx::dexx(unsigned char *inputText, unsigned int inputTextLength, uint32_t *k)
{
  unsigned int numBlocks = (inputTextLength <= BLOCK_SIZE ? 1 : inputTextLength/BLOCK_SIZE), i;
  if (inputTextLength < BLOCK_SIZE)	{
		return -1;
  }

    #if 0
  for (i=0; i<numBlocks;i++){
     btea((uint32_t *)inputText, BLOCK_SIZE/4*(-1),k);
     inputText+=BLOCK_SIZE;
  }
  #endif
  int iEncLen = numBlocks*BLOCK_SIZE;
  btea((uint32_t *)inputText, iEncLen/4*(-1),k);  
  return numBlocks;
}



