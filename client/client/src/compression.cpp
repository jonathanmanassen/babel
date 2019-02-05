
#include "compression.h"

Compression::Compression()
{
}

Compression::~Compression()
{
	opus_encoder_destroy(enc);
	opus_decoder_destroy(dec);
}

int Compression::init()
{
	enc = opus_encoder_create(sampleRate, channels, OPUS_APPLICATION_AUDIO, &opusErr);
	if (opusErr != OPUS_OK)
		return (1);
	dec = opus_decoder_create(sampleRate, channels, &opusErr);
	if (opusErr != OPUS_OK)
		return (1);
	return (0);
}

int Compression::encode(std::vector<unsigned short> const &captured, std::vector<unsigned char> &encoded)
{
	if (opusErr != OPUS_OK)
		return (-1);
	if ((enc_bytes = opus_encode(enc, reinterpret_cast<opus_int16 const*>(captured.data()), 480, encoded.data(), encoded.size())) < 0)
		return (-1);
	return (static_cast<int>(enc_bytes));
}

int Compression::decode(std::vector<unsigned char> const &encoded, std::vector<unsigned short> &decoded, int encb)
{
	if (opusErr != OPUS_OK)
		return (-1);
	if ((dec_bytes = opus_decode(dec, encoded.data(), encb, reinterpret_cast<opus_int16*>(decoded.data()), 480, 0)) < 0)
		return (-1);
	return (0);
}