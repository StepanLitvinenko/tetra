#ifndef TETRA_DL_STREAM_H
#define TETRA_DL_STREAM_H

#include "types.h"
#include "tetra_cnst.h"
#include "tetra_sys_info.h"


namespace Tetra {

const uint32_t BNCH_CODED_DATA_SIZE = TETRA_CODE_RATE*( BNCH_BITS_COUNT + TETRA_TAIL_SIZE + TETRA_CRC_SIZE );
const uint32_t BSCH_CODED_DATA_SIZE = TETRA_CODE_RATE*( BSCH_BITS_COUNT + TETRA_TAIL_SIZE + TETRA_CRC_SIZE );

class TetraDlStream  {
public:
	// step - sample count between different tracks
	TetraDlStream( uint8_t step_ );
    virtual ~TetraDlStream( );

	// size - data size for current track and current channel
	// size should be multiply of TETRA_SYNC_SIZE
    uint8_t			Process(const WordComplex *data, uint32_t size );
	BschSysInfo*	GetBschInfo(  );
	BnchSysInfo*	GetBnchInfo(  );
	enum StreamState {
		noDecoded = 0,
		bschDecoded = 1,
		bnchDecoded = 2
	};

private:
	WordComplex sync[ TETRA_SYNC_SIZE ];
	WordComplex	diffSync[ TETRA_SYNC_SIZE ];
	WordComplex diffSignal[ 4 * TETRA_SYNC_PERIOD ];
	int8_t 		demodData[ TETRA_SLOT_SIZE ];
	int8_t		syncBits[ TETRA_SYNC_SIZE ];
	int8_t		codedData[ BNCH_CODED_DATA_SIZE ];
	uint32_t		decodedData[ 8 ];
	BschSysInfo	bschInfo;
	BnchSysInfo	bnchInfo;
	uint8_t 		trackCount;
	uint8_t		step;
	void 		GenerateSync( );
	void		GenerateDiffSync(  );
	uint32_t		GetBeginPos( uint32_t corrPos );
};

}

#endif
