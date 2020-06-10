#include "tetra_dl_stream.h"
#include "tetra_corr.h"
#include "tetra_demod.h"
#include "tetra_deinterleave.h"
#include "tetra_descrambler.h"
#include "tetra_conv_decoder.h"
#include "tetra_parser.h"
#include "tetra_crc.h"
#include "diff_signal_calc.h"
#include "reoutils/C6xSimulator.h"
#include <cmath>
#include <string>
#include <string.h>

namespace Tetra {

//const double M_PI 	= 3.14159265358979;
//const double M_PI_4	= 0.78539816339744;

TetraDlStream::TetraDlStream( uint8_t step_ ):
	trackCount( 2 ), // this version only for 2 track
	step( step_ )
{
	for ( int32_t i = 0; i < TETRA_SYNC_SIZE; i++ ) {
		if( ( i < 8 ) || ( i > 71 ) ){
			syncBits[ i ] = 1;
		}
        else {
			syncBits[ i ] = 0;
		}
    }
	GenerateSync( );
	GenerateDiffSync( );
}

TetraDlStream::~TetraDlStream( )
{

}

uint8_t	TetraDlStream::Process( const WordComplex* data, uint32_t size )
{
	WordComplex max;
	uint32_t corrPos, startPos;
	uint32_t corrSize = trackCount * TETRA_SYNC_PERIOD; 
	uint32_t crc = 0;
	uint32_t crcSize;
	uint8_t  ret = noDecoded;
	
	for( int32_t i = 0; i < ( size/TETRA_SYNC_PERIOD - 1 ); i++ ) {
		diff_signal_calc( data, diffSignal, 2 * trackCount * TETRA_SYNC_PERIOD, step, 1 );

		corrPos = corr_calc( ( uint32_t* )diffSignal, 1, 1, ( uint32_t* )diffSync, corrSize, TETRA_SYNC_SIZE - 1, &max, 11  );

		startPos = GetBeginPos( corrPos );
		diff_signal_calc( &data[ startPos ], diffSignal, TETRA_SYNC_PERIOD, trackCount * step, 1 );

		demod( ( uint32_t* )diffSignal, &max, 1, TETRA_SLOT_SIZE/2, demodData );
	
		descrambler( &demodData[ SB_BLOCK1_SHIFT ], SB_BLOCK1_SIZE, 0x00000003 );
        memset( codedData, 0, BNCH_CODED_DATA_SIZE );
		deinterleave_depuncturing( &demodData[ SB_BLOCK1_SHIFT ], SB_BLOCK1_SIZE, codedData, 11 );	
		conv_decoder( codedData, BSCH_CODED_DATA_SIZE, decodedData, bsch );
		
		crcSize = BSCH_BITS_COUNT + TETRA_CRC_SIZE;
		crc = crc_calc( decodedData, crcSize );
        memset( &bschInfo, 0, sizeof( bschInfo ) );

		if( crc == 65535 ) {
			if( !ret ) {
				ret = bschDecoded;
			}
			ParseBSCH( decodedData, &bschInfo );

            memset( decodedData, 0, 8*4 );
			
			descrambler( &demodData[ SB_BLOCK2_SHIFT ], SB_BLOCK2_SIZE, bschInfo.colorCode );
            memset( codedData, 0, BNCH_CODED_DATA_SIZE );
			deinterleave_depuncturing( &demodData[ SB_BLOCK2_SHIFT ], SB_BLOCK2_SIZE, codedData, 101 );
			
			conv_decoder( codedData, BNCH_CODED_DATA_SIZE, decodedData, bnch );
			crcSize = BNCH_BITS_COUNT + TETRA_CRC_SIZE;
			crc = crc_calc( decodedData, crcSize );

            memset( &bnchInfo, 0, sizeof( bnchInfo ) );
			if( crc == 65535 ) {
				ParseBNCH( decodedData, &bnchInfo );
				ret = bnchDecoded;
			}
		}
		
		data += trackCount*step*TETRA_SYNC_PERIOD;
	}
	return ret;
}

BschSysInfo*	TetraDlStream::GetBschInfo(  ) 
{
	return &bschInfo;
}

BnchSysInfo*	TetraDlStream::GetBnchInfo(  ) 
{
	return &bnchInfo;
}

uint32_t		TetraDlStream::GetBeginPos( uint32_t corrPos )
{
	uint8_t track;
#ifndef HOST_CODE
	uint8_t posShift = 30 - _norm( trackCount ); 
#else
    uint8_t posShift = 30;
#endif //HOST_CODE
    uint8_t posMask = trackCount - 1;

	track = corrPos & posMask; 
	corrPos >>= posShift;
	if( corrPos >= 4 ) {
		corrPos -= 4; // 7 - trainsec size in samples, 1 is because diff signal
	}
	return ( corrPos * step * trackCount + (track+1) * step );
}

void 	TetraDlStream::GenerateSync( )
{
	double phase = 0.0, dphase = 0.0;

	phase = M_PI;

	for ( int32_t i = 0; i < TETRA_SYNC_SIZE/2; i++ )	{
		if ( ( syncBits[ i + 1 ] == 1 ) && ( syncBits[ i ] == 1 ) ) 
			dphase = -3*M_PI_4;
		if ( ( syncBits[ i + 1 ] == 0 ) && ( syncBits[ i ] == 1 ) ) 
			dphase =  -M_PI_4;
		if ( ( syncBits[ i + 1 ] == 1 ) && ( syncBits[ i ] == 0 ) ) 
			dphase = 3*M_PI_4;
		if ( ( syncBits[ i + 1 ] == 0 ) && ( syncBits[ i ] == 0 ) ) 
			dphase = M_PI_4;

		phase += dphase;

        sync[ 2*i 	].re		= ( int32_t )( 128.0 * std::cos( phase - dphase/2 ) );
        sync[ 2*i	].im	= ( int32_t )( 128.0 * std::sin( phase - dphase/2 ) );
        sync[ 2*i+1 ].re 		= ( int32_t )( 128.0 * std::cos( phase ) );
        sync[ 2*i+1 ].im 	= ( int32_t )( 128.0 * std::sin( phase ) );
	}
}

void TetraDlStream::GenerateDiffSync( )
{
	diff_signal_calc( sync, diffSync, TETRA_SYNC_SIZE, 1, 1  );
}

}
