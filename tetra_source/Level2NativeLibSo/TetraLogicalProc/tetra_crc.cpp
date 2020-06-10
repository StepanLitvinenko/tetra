#include "tetra_crc.h"

namespace Tetra {
	
uint32_t crc_calc( const uint32_t* data, uint32_t size )
{
    const uint32_t* input = data;
    uint32_t out = 0;

    uint8_t invin = 1;
    uint8_t invout = 1;
    uint8_t mergin = 0;

    uint8_t reg[ 16 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (int32_t i = 0; i < size /*+ 16*/; i++ ) {
      uint8_t tmp = reg[15];
      reg[15] = reg[14]; reg[14] = reg[13]; reg[13] = reg[12];
      reg[12] = reg[11]; reg[11] = reg[10]; reg[10] = reg[9];
      reg[9] = reg[8]; reg[8] = reg[7]; reg[7] = reg[6];
      reg[6] = reg[5]; reg[5] = reg[4]; reg[4] = reg[3];
      reg[3] = reg[2]; reg[2] = reg[1]; reg[1] = reg[0];
      if (i < size +16) {
          reg[0] = ((input[i>>5] >> (i%32)) & 0x1);
	  }
      else {
          reg[0] = 0;
      }
      if ((i < 16)||(( i >= ( size - 16 ) )&&(i<size))){
          reg[0] ^= invin;
      }
      if (tmp == 1) { 
          reg[12] = reg[12] ^ 1;
          reg[5] = reg[5] ^ 1;
          reg[0] = reg[0] ^ 1;
       }
    }

    if ( invout ) { 
        for (uint8_t i = 0; i< 16; i++){
            reg[i] = reg[i] ^ 1;
        }
    }

    if (mergin == 0 ) {
	    for (int i = 0; i<16; i++) {
            out |= ((((uint8_t) reg[i]) & 0x1 )<< i);
		}
	}
    if (mergin == 1) {
    	for (int i = 0; i<16; i++) {
            out |= ((((uint8_t) reg[15-i]) & 0x1 )<< i);
		}
	}
    
    return out;
}

}
