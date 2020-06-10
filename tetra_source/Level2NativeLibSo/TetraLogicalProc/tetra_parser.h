#ifndef TETRA_PARSER_H
#define TETRA_PARSER_H

#include "types.h"
#include "tetra_sys_info.h"

namespace Tetra {

void ParseBSCH( const uint32_t* data, BschSysInfo* bschInfo );
void ParseBNCH( const uint32_t* data, BnchSysInfo* bnchInfo );

}

#endif
