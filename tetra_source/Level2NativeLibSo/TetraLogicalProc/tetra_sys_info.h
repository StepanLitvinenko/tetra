#ifndef TETRA_SYS_INFO_H
#define TETRA_SYS_INFO_H

#include "types.h"
#include <string>

namespace Tetra {

struct BschSysInfo {
	int32_t MODE_OP;
	int32_t CC;
	int32_t TS;
	int32_t FN;
	int32_t MN;
	int32_t SM;
	int32_t TS_R;
	int32_t UDTX;
	int32_t F18E;
	int32_t MCC;
	int32_t MNC;
	int32_t NCB;
	int32_t CSL;
	int32_t LEI;
	uint32_t colorCode;
	BschSysInfo( )
	:	MODE_OP( 0 ), CC( 0 ), TS( 0 ), FN( 0 ), MN( 0 ), SM( 0 ), 
		TS_R( 0 ), UDTX( 0 ), F18E( 0 ), MCC( 0 ), MNC( 0 ), 
		NCB( 0 ), CSL( 0 ), LEI( 0 ), colorCode( 0 )
	{
		
	}
};

struct BnchSysInfo {
	int32_t PDU_type;
	int32_t Broadcast_type;
	int32_t Main_carrier;
	int32_t Frequency_band;
	int32_t offset;
	int32_t Duplex_spacing;
	int32_t Num_of_SCCH;
	int32_t MS_TX_PWRMAX;
	int32_t RXLEV_ACSS_MIN;
	int32_t ACCS_PAR;
	int32_t RADIO_DWL_TIMEOUT;
	int32_t Hyperframe_CCK;
	int32_t Hyperframe_Num;
	int32_t CCK_ind;
	int32_t Option_field_flag;
	int32_t LA;
	BnchSysInfo( )
	:	PDU_type( 0 ), Broadcast_type( 0 ), Main_carrier( 0 ), Frequency_band( 0 ), 
		offset( 0 ), Duplex_spacing( 0 ), Num_of_SCCH( 0 ), MS_TX_PWRMAX( 0 ), 
		RXLEV_ACSS_MIN( 0 ), ACCS_PAR( 0 ), RADIO_DWL_TIMEOUT( 0 ), Hyperframe_CCK( 0 ), 
		Hyperframe_Num( 0 ), CCK_ind( 0 ), Option_field_flag( 0 ), LA( 0 )
	{
	}
};

class SysInfo {
	bool bschValid;
	BschSysInfo bschInfo;
	bool bnchValid;
	BnchSysInfo bnchInfo;

public:
	SysInfo() : bschValid(false), bnchValid(false) { }

	bool getBschValid() const
	{
		return bschValid;
	}

	bool getBnchValid() const
	{
		return bnchValid;
	}

	const BschSysInfo& getBschInfo() const
	{
		return bschInfo;
	}

	const BnchSysInfo& getBnchInfo() const
	{
		return bnchInfo;
	}

	void setBschInfo(const BschSysInfo& info)
	{
		bschValid = true;
		bschInfo = info;
	}

	void setBnchInfo(const BnchSysInfo& info)
	{
		bnchValid = true;
		bnchInfo = info;
	}

	void clearValid()
	{
		bschValid = false;
		bnchValid = false;
	}
};

}

#endif
