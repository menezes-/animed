#ifndef _BASE64_H_
#define _BASE64_H_
#include <vector>
#include <string>

//tirado de:http://stackoverflow.com/a/13935718/482238

typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);

#endif