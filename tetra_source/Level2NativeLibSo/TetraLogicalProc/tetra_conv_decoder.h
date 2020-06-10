#ifndef TETRA_CONV_DECODER_H
#define TETRA_CONV_DECODER_H
#include <stdint.h>

namespace Tetra {

enum eDecoderType {
	bsch = 0,
	bnch = 1	
};

void conv_decoder( const int8_t* codedData, uint32_t size, uint32_t* decodedData, uint8_t type );

}

#endif
