#include "diff_signal_calc.h"
#include <stdint.h>
#include "reoutils/C6xSimulator.h"
#include "reoutils/common_macro.h"


void diff_signal_calc( const WordComplex* in, WordComplex* out,
                                                        uint32_t size, uint16_t inStep, uint16_t outStep )
{
    uint32_t i;
	double iq0, iq1;
	double diff;
	int64_t real, imagine;
	int64_t module;
    int32_t   resReal, resImage;

    uint32_t hi, lo;
    uint32_t count, currCount;
    uint32_t shift0, shift1;
    const WordComplex* inTmp = in;
#ifndef HOST_CODE
    count = 0;
    for( i = 0; i < size; i++ ) {
        iq0 = _amemd8_const( inTmp );
        inTmp += inStep;
        module = COMPLEX_ABS( ( int32_t )(_hi( iq0 )), ( int32_t )(_lo( iq0 )) );
        hi =  _hill( module );
        lo =  _loll( module );

        ( hi > 0 ) ? ( currCount = 63 - _norm( hi ) ) : ( currCount = 31 - _norm( lo ) );
        count += currCount;
    }
	
    count /= size;
    shift0 = count;
    shift1 = 32 - count;

    iq0 = _amemd8_const( in );
    in += inStep;
    for( i = 0; i < ( size - 1 ); i++ ) {
        iq1 = _amemd8_const( in );
        in += inStep;
        real 	= ((int32_t)(_hi(iq0)))*((int32_t)(_hi(iq1))) + ((int32_t)(_lo(iq0)))*((int32_t)(_lo(iq1)));
        imagine = -((int32_t)(_hi(iq0)))*((int32_t)(_lo(iq1))) + ((int32_t)(_lo(iq0)))*((int32_t)(_hi(iq1)));

        resReal 	= _loll( real ) >> shift0;
        resReal    |= _hill( real ) << shift1;
        resImage    = _loll( imagine ) >> shift0;
        resImage   |= _hill( imagine ) << shift1;

        iq0 = iq1;
        diff = _itod( resImage, resReal );
        _amemd8( out ) = diff;
        out += outStep;
    }
#endif //HOST_CODE
}
