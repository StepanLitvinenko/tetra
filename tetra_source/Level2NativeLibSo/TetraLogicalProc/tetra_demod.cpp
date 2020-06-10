#include "tetra_demod.h"
#include "types.h"
#include "reoutils/C6xSimulator.h"
#ifdef _DEBUG
#include <stdio.h>
#endif

namespace Tetra {

#ifdef _DEBUG
int64_t demodIQ[ 510 * 2 ];
#endif

void demod( const uint32_t* diffSignal, WordComplex* freqError, uint16_t step, uint32_t size, int8_t* demodData )
{
    int32_t i;
    int64_t resReal, resImagine;
	double iq0, iq1;

#ifdef _DEBUG
 	FILE* fp;
#endif

    uint8_t step2 = step << 1;

    //const uint32_t* restrict rawData0 = rawData;
    const uint32_t* rawData1 = diffSignal;
#ifndef HOST_CODE
    iq0 = _itod( freqError->im, freqError->re );

	for( i = 0; i < size; i++ ) {
		iq1 	  = _amemd8_const( rawData1 );
		rawData1 += step2;

        resReal 	= ((int32_t)(_hi(iq0)))*((int32_t)(_hi(iq1))) + ((int32_t)(_lo(iq0)))*((int32_t)(_lo(iq1)));
        resImagine 	= -((int32_t)(_hi(iq0)))*((int32_t)(_lo(iq1))) + ((int32_t)(_lo(iq0)))*((int32_t)(_hi(iq1)));

#ifdef _DEBUG
		demodIQ[ 2*i     ] = resReal;
		demodIQ[ 2*i + 1 ] = resImagine;
#endif

		( resReal > 0 ) ? ( demodData[ 2*i + 1 ] = 0 ) : ( demodData[ 2*i + 1 ] = 1 );
		( resImagine > 0 ) ? ( demodData[ 2*i ] = 0 ) : ( demodData[ 2*i ] = 1 );
		//iq0 = iq1;
	}
#endif //HOST_CODE
#ifdef _DEBUG
	fp = fopen( "demodIQ.sig", "wb" );
	fwrite( demodIQ, sizeof( uint64 ), 510, fp );
	fclose( fp );
#endif
}

}
