
/*
* XXTEA ??
* auth: arrow
*/

#ifndef DODXX_h
#define DODXX_h

#define	MD5_HA	1
#define	SHA1_HA	2
#define	AES_EA	3

#define	HASH_LENGTH	(128/8)
#define	HASH_IV	0x9e, 0x37, 0x79, 0xb9, 0x9e, 0x37, 0x79, 0xb9, \
		0x9e, 0x37, 0x79, 0xb9, 0x9e, 0x37, 0x79, 0xb9

#define	BLOCK_SIZE	(128/8)
#define	KEY_SIZE	(128/8)

#include <inttypes.h>

class Dodxx
{
public:
	Dodxx();
	
	uint32_t* genxx(const char* szRand);
	int doxx(unsigned char *inputText, unsigned int inputTextLength, uint32_t *k);
	int dexx(unsigned char *inputText, unsigned int inputTextLength, uint32_t *k);

private:	
	void calHash(unsigned char *inputText, unsigned int inputTextLength, unsigned char *hash);
};

#endif



