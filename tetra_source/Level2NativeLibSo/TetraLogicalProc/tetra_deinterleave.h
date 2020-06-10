#ifndef TETRA_DEINTERLEAVE_H
#define TETRA_DEINTERLEAVE_H

#include "types.h"

namespace Tetra {

void deinterleave_depuncturing( const int8_t *descrData, uint32_t descrSize,
                                    int8_t *deinterleaveData, uint8_t interleaveParam );
}

#endif
