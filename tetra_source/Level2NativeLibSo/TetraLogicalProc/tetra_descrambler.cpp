#include "tetra_descrambler.h"

namespace Tetra {

void descrambler( int8_t* demodData, uint32_t size, uint32_t initState )
{
	uint32_t res;
	//uint32_t shiftReg = 0x00000003;
	uint32_t shiftReg = initState;
	int32_t i;

	for( i = 0; i < size; i++ ) {
		res = ( shiftReg ^( shiftReg>>6 )^( shiftReg>>9)^( shiftReg>>10 )^( shiftReg>>16 )^
				( shiftReg>>20 )^( shiftReg>>21 ) ^( shiftReg>>22 )^( shiftReg>>24 )^
				( shiftReg>>25 )^( shiftReg>>27 )^( shiftReg>>28 )^( shiftReg>>30 )^( shiftReg>>31 ) ) & 0x01;

		demodData[ i ] ^= res;  
		res <<= 31;
		shiftReg >>= 1;
		shiftReg |= res;
	}
}

}


