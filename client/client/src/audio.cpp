
#include "audio.h"

Audio::Audio()
{
}

Audio::~Audio()
{
	if ((paErr = Pa_StopStream(stream)) != paNoError)
		return;
	if ((paErr = Pa_CloseStream(stream)) != paNoError)
		return;
	if ((paErr = Pa_Terminate()) != paNoError)
		return;
}

int Audio::init()
{
	if ((paErr = Pa_Initialize()) != paNoError)
		return (1);
	stream = nullptr;
	if ((paErr = Pa_OpenDefaultStream(&stream,
		channels, channels, paInt16, sampleRate,
		bufferSize, nullptr, nullptr)) != paNoError)
		return (1);
	if ((paErr = Pa_StartStream(stream)) != paNoError)
		return (1);
	return (0);
}

int Audio::read(std::vector<unsigned short> &captured)
{
	if ((paErr = Pa_ReadStream(stream,
		captured.data(), bufferSize)) != paNoError)
		return (-1);
	return (0);
}

int Audio::write(std::vector<unsigned short> const &decoded)
{
	if ((paErr = Pa_WriteStream(stream, decoded.data(), bufferSize)) != paNoError)
		return (-1);
	return (0);
}