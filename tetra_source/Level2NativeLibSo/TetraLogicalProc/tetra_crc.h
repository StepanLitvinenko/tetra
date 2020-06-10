#ifndef TETRA_CRC_H
#define TETRA_CRC_H

#include "types.h"

namespace Tetra {

uint32_t crc_calc( const uint32_t* data, uint32_t size );

}

#endif
