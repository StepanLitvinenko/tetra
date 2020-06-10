#ifndef DIFF_SIGNAL_CALC_H
#define DIFF_SIGNAL_CALC_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void diff_signal_calc( const WordComplex* in, WordComplex*  out,
                                                        uint32_t size, uint16_t inStep, uint16_t outStep );

#ifdef __cplusplus
}
#endif

#endif
