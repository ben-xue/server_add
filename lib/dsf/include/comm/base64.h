#pragma once

#include "stdinc.h"

NS_BEGIN(dsf)

int                 b64_encode(std::string & b64, const char * buff, int ibuff);
int                 b64_decode(std::string & buff, const std::string & b64);
int                 b64_encode(const unsigned char *data, unsigned int datalen, char *out);
int                 b64_decode(const char *in, unsigned int inlen, unsigned char *out, unsigned int& outlen);
int                 hex2bin(std::string & bin, const char * hex);
int                 bin2hex(std::string & hex, const char * buff, int ibuff);

NS_END()