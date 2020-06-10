#include "tetra_corr.h"
#include "reoutils/C6xSimulator.h"
#include "reoutils/common_macro.h"

#ifdef _DEBUG
#include <stdio.h>
#endif


namespace Tetra {

uint32_t corr_calc( const uint32_t* iq, uint16_t corrStep, uint16_t sampleStep,
                                                    const uint32_t*  sync, uint32_t corrSize,
                                                    uint32_t syncSize, WordComplex* corrRes, uint8_t shift )
{
#ifdef _DEBUG	
	FILE* fp = fopen( "t_corr.sig", "wb" );
#endif
	
    const uint32_t* iqPtr;
    const uint32_t* syncPtr;

    uint8_t 	corrStep2 	= corrStep << 1;
    uint8_t   sampleStep2 = sampleStep << 1;
    uint32_t	corrMax = 0;
    uint32_t 	pos = 0;
    uint32_t	currCorr;
    int32_t   maxReal = 0, maxImagine = 0;
	double 		iqPair, syncPair;
	long long 	currReal, currImage;
    int32_t	tmpReal, tmpImage;
    int32_t	real, image;
	shift++;
    uint8_t shift1 = 32 - shift;
    for( int32_t j = 0; j < corrSize; j++ ) {
		iqPtr = iq;
		syncPtr = sync;
		real = image = 0;
#ifndef HOST_CODE
        for( int32_t i = 0; i < syncSize; i++ ) {
			iqPair 		= _amemd8_const( iqPtr );
			syncPair	= _amemd8_const( syncPtr );
            //currReal = ( (int32_t)(_hi( iqPair ))*(int32_t)(_hi( syncPair )) + (int32_t)(_lo( iqPair ))*(int32_t)(_lo( syncPair )) );
            //currImage = ( (int32_t)(_hi( iqPair ))*(int32_t)(_lo( syncPair )) - (int32_t)(_lo( iqPair ))*(int32_t)(_hi( syncPair )) );

			currReal = _mpylill( _hi( syncPair ), _hi( iqPair ) ) + _mpylill( _lo( syncPair ), _lo( iqPair ) );
			currImage = _mpylill( _lo( syncPair ), _hi( iqPair ) ) - _mpylill( _hi( syncPair ), _lo( iqPair ) );

			tmpReal = _loll( currReal ) >> shift;
			tmpReal |= _hill( currReal ) << shift1;
			tmpImage = _loll( currImage ) >> shift;
			tmpImage |= _hill( currImage ) << shift1;

			real += tmpReal;
			image += tmpImage;
			iqPtr += sampleStep2;
			syncPtr += 2;
        }
#endif // HOST_CODE

		currCorr = COMPLEX_ABS( real, image );
#ifdef _DEBUG
        fwrite( &currCorr, sizeof( uint32_t ), 1, fp );
#endif
		if( currCorr > corrMax ) {
			corrMax = currCorr;
			pos = j;
			maxReal = real;
			maxImagine = image;
		}
		iq += corrStep2;
	}
#ifdef _DEBUG
	fclose( fp );
#endif

    corrRes->re = maxReal;
    corrRes->im = maxImagine;
	return pos;
}

}

