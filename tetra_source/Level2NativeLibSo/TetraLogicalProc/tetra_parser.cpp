#include "tetra_parser.h"

namespace Tetra{

void ParseBSCH( const uint32_t* data, BschSysInfo* bschInfo )
{
    for (int i = 0; i < 4; i++) {
        bschInfo->MODE_OP |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 4 - i);
    }

    for (int i = 4; i<10; i++) {
        bschInfo->CC |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 9 - i);
		bschInfo->colorCode |= ((data[i>>5] >> (i & 31))& 0x1) << ( 11 - i );
    }

    for (int i = 10; i<12; i++)
        bschInfo->TS |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 11 - i);
    for (int i = 12; i<17; i++)
        bschInfo->FN |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 16 - i);
    for (int i = 17; i<23; i++)
        bschInfo->MN |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 22 - i);
    for (int i = 23; i < 25; i++)
        bschInfo->SM |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 24 - i);

    for (int i = 25; i < 28; i++)
        bschInfo->TS_R |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 27 - i);

    bschInfo->UDTX = ((data[28>>5] >> (28 & 31))& 0x1);
    bschInfo->F18E = ((data[29>>5] >> (29 & 31))& 0x1);

    for (int i = 31; i < 41; i++) {
        bschInfo->MCC |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 40 - i);
		bschInfo->colorCode |= ((data[i>>5] >> (i & 31))& 0x1) << ( 62 - i );
    }

    for (int i = 41; i<55; i++) {
        bschInfo->MNC |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 54 - i);
		bschInfo->colorCode |= ((data[i>>5] >> (i & 31))& 0x1) << ( 62 - i );
    }

    for (int i = 55; i<57; i++) {
        bschInfo->NCB |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 56 - i);
    }

    for (int i = 57; i<59; i++) {
        bschInfo->CSL |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 58 - i);
    }

    bschInfo->LEI = data[1] >> (59 & 31)& 0x1;
	bschInfo->colorCode |= 0x3;
}

void ParseBNCH( const uint32_t* data, BnchSysInfo* bnchInfo )
{
    int32_t reverse = 0;
    //int Duplex_spacing = 0, Num_of_SCCH = 0, MS_TX_PWRMAX = 0, RXLEV_ACSS_MIN = 0, ACCS_PAR = 0;
    //int RADIO_DWL_TIMEOUT  = 0, Hyperframe_CCK = 0, Hyperframe_Num  = 0, CCK_ind = 0;
    //int Option_field_flag = 0;
    int32_t Subscr_Class = 0, BS_Service_details = 0;
    int32_t IMMM = 0, Wt = 0, Nu = 0, FLF = 0, TimeslPointer = 0, Prior = 0;


    for (int32_t i = 0; i<2; i++) {
        bnchInfo->PDU_type |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 1 - i);
    }
    for (int32_t i = 2; i<4; i++) {
        bnchInfo->Broadcast_type |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 3 - i);
    }
    for (int32_t i = 4; i<16; i++) {
        bnchInfo->Main_carrier |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 15 - i);
    }
    for (int32_t i = 16; i<20; i++) {
        bnchInfo->Frequency_band |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 19 - i);
    }
    for (int32_t i = 20; i<22; i++) {
        bnchInfo->offset |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 21 - i);
    }
    for (int32_t i = 22; i<25; i++) {
        bnchInfo->Duplex_spacing |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 24 - i);
    }

    reverse |= ((data[25/32] >> (25 % 32))& 0x1);
    for (int32_t i = 26; i<28; i++) {
        bnchInfo->Num_of_SCCH |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 27 - i);
    }
    for (int32_t i = 28; i<31; i++) {
        bnchInfo->MS_TX_PWRMAX |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 30 - i);
    }
    for (int32_t i = 31; i<35; i++) {
        bnchInfo->RXLEV_ACSS_MIN |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 34 - i);
    }
    for (int32_t i = 35; i<39; i++) {
        bnchInfo->ACCS_PAR |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 38-  i);
    }
    for (int32_t i = 39; i<43; i++) {
        bnchInfo->RADIO_DWL_TIMEOUT |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 42 - i);
    }

    bnchInfo->Hyperframe_CCK |= ((data[43/32] >> (43 % 32))& 0x1);
    for (int32_t i = 44; i<60; i++) {
        bnchInfo->Hyperframe_Num |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 59 - i);
    }
    bnchInfo->CCK_ind = bnchInfo->Hyperframe_Num;
    for (int32_t i = 60; i < 62; i++) {
        bnchInfo->Option_field_flag |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 61 - i);
    }
    
    if (bnchInfo->Option_field_flag == 2) {
       for (int32_t i = 62; i < 66; i++) {
           IMMM |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 65 - i);
       }
       for (int32_t i = 66; i < 70; i++) {
           Wt |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 69 - i);
       }
       for (int32_t i = 70; i < 74; i++) {
           Nu |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 73 - i);
       }
       for (int32_t i = 74; i < 75; i++) {
           FLF |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 74 - i);
       }
       for (int32_t i = 75; i < 79; i++) {
           TimeslPointer |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 78 - i);
       }
       for (int32_t i = 79; i < 82; i++) {
           Prior |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 81 - i);
       }
    }

    int32_t frames1_18 = 0;
    int32_t security_info = 0;
    int32_t SDS_TL_METHOD = 0;
    int32_t GCK_Support = 0;
    int32_t Section = 0;
    int32_t data_of_section = 0;
    if (bnchInfo->Option_field_flag == 3) {
       for (int32_t i = 62; i < 70; i++) {
           security_info |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 69 - i);
       }
       for (int32_t i = 70; i < 72; i++) {
           SDS_TL_METHOD |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 71 - i);
       }
       for (int32_t i = 72; i < 73; i++) {
           GCK_Support |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 72 - i);
       }
       for (int32_t i = 73; i < 75; i++) {
           Section |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 74 - i);
       }
       for (int32_t i = 75; i < 82; i++) {
           data_of_section |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 81 - i);
       }
    
    }
    if ((bnchInfo->Option_field_flag == 1)||(bnchInfo->Option_field_flag == 0)) {
       for (int32_t i = 62; i < 80; i++){
           frames1_18 |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 79 - i);
       }
       // 2 bits - reserved
    }
    for (int32_t i = 82; i < 98; i++) {
       bnchInfo->LA |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 97 - i);
    }
    for (int32_t i = 98; i < 114; i++) {
       Subscr_Class |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 113 - i);
    }
    for (int32_t i = 114; i < 124; i++) {
       BS_Service_details |= ((data[i>>5] >> (i & 31))& 0x1)<< ( 123 - i);
    }
	bnchInfo->LA >>= 2;
}

}
