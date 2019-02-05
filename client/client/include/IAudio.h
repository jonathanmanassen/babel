#ifndef IAUDIO_H
#define IAUDIO_H

#include <vector>

class IAudio
{
public:
	virtual int init() = 0;
	virtual int read(std::vector<unsigned short> &captured) = 0;
	virtual int write(std::vector<unsigned short> const &decoded) = 0;
};

#endif