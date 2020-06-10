#ifndef TETRA_CORR_H
#define TETRA_CORR_H

#include "types.h"
namespace Tetra {

uint32_t corr_calc( const uint32_t*  iq, uint16_t corrStep, uint16_t sampleStep,
                const uint32_t* sync, uint32_t corrSize,
                        uint32_t syncSize, WordComplex* corrRes, uint8_t shift );
}

#endif
