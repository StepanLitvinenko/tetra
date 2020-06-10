#ifndef TETRA_DEMOD_H
#define TETRA_DEMOD_H

#include "types.h"

namespace Tetra {

void demod(  const uint32_t* diffSignal, WordComplex* freqError, uint16_t step, uint32_t size, int8_t* demodData );

}

#endif
