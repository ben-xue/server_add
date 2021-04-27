#ifndef CONVERT_H

#define CONVERT_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

static char HEX_value[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

unsigned char* bin2hex(unsigned char * pbin,int binlen, char* phex,int *hexlen);

unsigned char* hex2bin(char * phex,int hexlen, unsigned char* pbin,int* binlen);


#ifdef __cplusplus
#endif /* __cplusplus */

#endif /* TCONVERT_H */
