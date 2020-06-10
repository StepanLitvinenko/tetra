#include "tetra_deinterleave.h"

namespace Tetra {

void deinterleave_depuncturing( const int8_t* descrData, uint32_t descrSize,
                                                int8_t* deinterleaveData, uint8_t interleaveParam )
{
    int32_t i, k, j;
    uint8_t p[ 3 ] = { 0, 1, 4 };

	for( i = 0; i < descrSize; i++ ) {
		k = ( interleaveParam * ( i + 1) ) % descrSize;
		j = 8*(i/3) + p[ i - 3*(i/3) ];

		deinterleaveData[ j ] = ( 1 - ( descrData[ k ] << 1 ) ) << 2;
		//deinterleaveData[ j ] = descrData[ k ];
	}
}

}
