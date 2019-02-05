#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include "IAudio.h"
#include "portaudio.h"
#include "opus.h"

class Audio : public IAudio
{
public:
	Audio();
	~Audio();
	int init();
	int read(std::vector<unsigned short> &captured);
	int write(std::vector<unsigned short> const &decoded);

private:
	PaError paErr;
	int const channels = 1;
	int const bufferSize = 480;
	int const sampleRate = 48000;
	PaStream* stream;
};

#endif