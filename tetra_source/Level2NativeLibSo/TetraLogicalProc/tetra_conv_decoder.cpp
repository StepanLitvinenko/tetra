#include "tetra_conv_decoder.h"
#include "viterbi.h"
//#include "chip/vcp.h"
namespace Tetra {

//#pragma DATA_SECTION( ".heap" )
//int8_t branchMetrics[ 146*8 ];

void conv_decoder( const int8_t* codedData, uint32_t size, uint32_t* decodedData, uint8_t type )
{
   ViterbiCodec vCodec(9, {0x90, 0x70, 0xB0,0xD0});
#ifndef HOST_CODE

//	Vcp& vcp = Vcp::Locate( );
//	Vcp::VcpConfig vcpConfig;

//	if( type == bsch ) {
//		vcpConfig.polynomes[ 0 ] 	= 0x90;
//		vcpConfig.polynomes[ 1 ] 	= 0x70;
//		vcpConfig.polynomes[ 2 ] 	= 0xB0;
//		vcpConfig.polynomes[ 3 ] 	= 0xD0;
//		vcpConfig.constraint	 	= 5;
//		vcpConfig.rate			 	= Vcp::rate1_4;
//        vcpConfig.frameLen		 	inTmp= 76;
//		vcpConfig.convModeEn	 	= FALSE;
//		vcpConfig.decisionType   	= Vcp::hard;
//		vcpConfig.outOrder		 	= 0;
//		vcpConfig.stateNum		 	= 0;
//		vcpConfig.traceBackIndexEn 	= 0;
//		vcpConfig.traceBackIndex 	= 0;
//	}
//	else if( type == bnch ) {
//		vcpConfig.polynomes[ 0 ] 	= 0x90;
//		vcpConfig.polynomes[ 1 ] 	= 0x70;
//		vcpConfig.polynomes[ 2 ] 	= 0xB0;
//		vcpConfig.polynomes[ 3 ] 	= 0xD0;
//		vcpConfig.constraint	 	= 5;
//		vcpConfig.rate			 	= Vcp::rate1_4;
//		vcpConfig.frameLen		 	= 140;
//		vcpConfig.convModeEn	 	= FALSE;
//		vcpConfig.decisionType   	= Vcp::hard;
//		vcpConfig.outOrder		 	= 0;
//		vcpConfig.stateNum		 	= 0;
//		vcpConfig.traceBackIndexEn 	= 0;
//		vcpConfig.traceBackIndex 	= 0;
//	}

//	vcp.Configure( &vcpConfig );
//	vcp.CalcBranchMetric( codedData, branchMetrics );
//	vcp.Start( branchMetrics, decodedData );
#endif //HOST_CODE
}

/*
void bsch_decoder( const int8_t* codedData, uint32_t size, uint32_t* decodedData )
{
	VcpConfig_t vcpConfig;
	BranchMetricsCalc( codedData, branchMetrics, size, rate1_4 );
	
	vcpConfig.polynomes[ 0 ] 	= 0x90;
	vcpConfig.polynomes[ 1 ] 	= 0x70;
	vcpConfig.polynomes[ 2 ] 	= 0xB0;
	vcpConfig.polynomes[ 3 ] 	= 0xD0;
	vcpConfig.constraint	 	= 5;
	vcpConfig.rate			 	= rate1_4;
	vcpConfig.frameLen		 	= 76;
	vcpConfig.convModeEn	 	= 1;
	vcpConfig.decisionType   	= hard;
	vcpConfig.outOrder		 	= 0;
	vcpConfig.stateNum		 	= 0;
	vcpConfig.traceBackIndexEn 	= 0;
	vcpConfig.traceBackIndex 	= 0;
	
	VcpSetup( &vcpConfig, branchMetrics, decodedData ); 		
}

void bnch_decoder( const int8_t* codedData, uint32_t size, uint32_t* decodedData )
{
	VcpConfig_t vcpConfig;
	BranchMetricsCalc( codedData, branchMetrics, size, rate1_4 );

	vcpConfig.polynomes[ 0 ] 	= 0x90;
	vcpConfig.polynomes[ 1 ] 	= 0x70;
	vcpConfig.polynomes[ 2 ] 	= 0xB0;
	vcpConfig.polynomes[ 3 ] 	= 0xD0;
	vcpConfig.constraint	 	= 5;
	vcpConfig.rate			 	= rate1_4;
	vcpConfig.frameLen		 	= 140;
	vcpConfig.convModeEn	 	= 1;
	vcpConfig.decisionType   	= hard;
	vcpConfig.outOrder		 	= 0;
	vcpConfig.stateNum		 	= 0;
	vcpConfig.traceBackIndexEn 	= 0;
	vcpConfig.traceBackIndex 	= 0;

	VcpSetup( &vcpConfig, branchMetrics, decodedData ); 	
}
   	after_viterbi_flow[0]=805487041;
	after_viterbi_flow[1]=2130708849;
	after_viterbi_flow[2]=2211446849;
	after_viterbi_flow[3]=3471540224;
	after_viterbi_flow[4]=3923;
*/

}
