#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <vector>
#include "ICompression.h"
#include "opus.h"

class Compression : public ICompression
{
public:
	Compression();
	~Compression();
	int init();
	int encode(std::vector<unsigned short> const &captured, std::vector<unsigned char> &encoded);
	int decode(std::vector<unsigned char> const &encoded, std::vector<unsigned short> &decoded, int bytes);

private:
	int opusErr;
	opus_int32 enc_bytes;
	opus_int32 dec_bytes;
	OpusEncoder* enc;
	OpusDecoder* dec;
	int const channels = 1;
	int const sampleRate = 48000;
};

#endif