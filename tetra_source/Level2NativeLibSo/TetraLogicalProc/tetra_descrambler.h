#ifndef TETRA_DESCRAMBLER_H
#define TETRA_DESCRAMBLER_H

#include "types.h"

namespace Tetra {

void descrambler( int8_t* demodData, uint32_t size, uint32_t initState );

}

#endif
