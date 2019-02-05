#ifndef ICOMPRESSION_H
#define ICOMPRESSION_H

#include <vector>

class ICompression
{
public:
	virtual int init() = 0;
	virtual int encode(std::vector<unsigned short> const &captured, std::vector<unsigned char> &encoded) = 0;
	virtual int decode(std::vector<unsigned char> const &encoded, std::vector<unsigned short> &decoded, int bytes) = 0;
};

#endif