#include "stdafx.h"

int l_throttlegate,n1,l_throttled,vhf1_var,vhf2_var,vhf3_var=0,com1,com2,com3,com1_std,com2_std,com3_std,com11,com22,rotator=0;
int time1,time2,deltaT,nav_active,vor_active,ils_active,mls_active,adf_active,bfo_active,vor,vor_std,vor22,ils,ils_std,ils22=0;
int ecam_switch_1,ecam_switch_2=0;
int r_throttlegate=3;

static CALTAB l_throttle[] =
		{ 
			{70,   0},
			{108 ,	 16384}
	};

static CALTAB l_throttle_rev[] =
		{ 
			{54,   -4096},
			{69,	 0}
	};

static CALTAB r_throttle[] =
		{ 
			{70,   0},
			{109,	 16384}
	};

static CALTAB r_throttle_rev[] =
		{ 
			{54,   -4096},
			{69,	 0}
	};


#define ZERO 0x01
#define ONE 0x02
#define TWO 0x04
#define TREE 0x08
#define FOUR 0x10
#define FIVE 0x20
#define SIX 0x40
#define SEVEN 0x80
#define EIGHT 0x100
#define NINE 0x200
#define TEN 0x400
#define ELEVEN 0x800	

unsigned long startTime2_;


void BuildPedestalObjects () {
	//----FSUBS OBJECTS-------//
	MkFsbusObject (BTP_A_IN, L_THROTTLE, "", eventhandlerpedestal, 11, 72, 0);
	MkFsbusObject (BTP_A_IN, R_THROTTLE, "", eventhandlerpedestal, 11, 78, 0);
	MkFsbusObject (BTP_D_OUT, MCDU_FAIL, "", eventhandlerpedestal, 3, 15, 0);
	MkFsbusObject (BTP_DISPLAY, ACTIVE, "", eventhandlerpedestal, 15, 0, 0);
	MkFsbusObject (BTP_DISPLAY, STANDBY, "", eventhandlerpedestal, 16, 0, 0);
	MkFsbusObject (BTP_D_IN, FLIP_FLOP, "", eventhandlerpedestal, 18, 37, 0);
	MkFsbusObject (BTP_D_IN, VHF1, "", eventhandlerpedestal, 18, 36, 0);
	MkFsbusObject (BTP_D_IN, VHF2, "", eventhandlerpedestal, 18, 35, 0);
	MkFsbusObject (BTP_D_IN, VHF3, "", eventhandlerpedestal, 18, 34, 0);
	MkFsbusObject (BTP_D_IN, HF1, "", eventhandlerpedestal, 18, 33, 0);
	MkFsbusObject (BTP_D_IN, HF2, "", eventhandlerpedestal, 18, 32, 0);
	MkFsbusObject (BTP_D_IN, AM, "", eventhandlerpedestal, 18, 6, 0);
	MkFsbusObject (BTP_D_IN, NAV, "", eventhandlerpedestal, 18, 5, 0);
	MkFsbusObject (BTP_D_IN, VOR, "", eventhandlerpedestal, 18, 4, 0);
	MkFsbusObject (BTP_D_IN, ILS, "", eventhandlerpedestal, 18, 3, 0);
	MkFsbusObject (BTP_D_IN, MLS, "", eventhandlerpedestal, 18, 2, 0);
	MkFsbusObject (BTP_D_IN, ADF, "", eventhandlerpedestal, 18, 1, 0);
	MkFsbusObject (BTP_D_IN, BFO, "", eventhandlerpedestal, 18, 0, 0);
	MkFsbusObject (BTP_D_IN, SW_ATT_HDG_1, "", eventhandlerpedestal, 18, 56, 0);
	MkFsbusObject (BTP_D_IN, SW_ATT_HDG_2, "", eventhandlerpedestal, 18, 57, 0);
	MkFsbusObject (BTP_D_IN, SW_AIR_DATA_1, "", eventhandlerpedestal, 18, 59, 0);
	MkFsbusObject (BTP_D_IN, SW_AIR_DATA_2, "", eventhandlerpedestal, 18, 58, 0);
	MkFsbusObject (BTP_D_IN, SW_EIS_DMC_1, "", eventhandlerpedestal, 18, 60, 0);
	MkFsbusObject (BTP_D_IN, SW_EIS_DMC_2, "", eventhandlerpedestal, 18, 61, 0);
	MkFsbusObject (BTP_D_IN, SW_ECAM_ND_1, "", eventhandlerpedestal, 18, 62, 0);
	MkFsbusObject (BTP_D_IN, SW_ECAM_ND_2, "", eventhandlerpedestal, 18, 63, 0);
	MkFsbusObject (BTP_D_IN, ON_OFF, "", eventhandlerpedestal, 18, 38, 0);
	MkFsbusObject (BTP_ROTARY, BIG_ROT, "", eventhandlerpedestal, 18, 48, 0);
	MkFsbusObject (BTP_ROTARY, SMALL_ROT, "", eventhandlerpedestal, 18, 52, 0);
	MkFsbusObject (BTP_D_OUT, BFO_LED, "", eventhandlerpedestal, 18, 0, 0);
	MkFsbusObject (BTP_D_OUT, ADF_LED, "", eventhandlerpedestal, 18, 1, 0);
	MkFsbusObject (BTP_D_OUT, MLS_LED, "", eventhandlerpedestal, 18, 2, 0);
	MkFsbusObject (BTP_D_OUT, ILS_LED, "", eventhandlerpedestal, 18, 3, 0);
	MkFsbusObject (BTP_D_OUT, VOR2_LED, "", eventhandlerpedestal, 18, 4, 0);
	MkFsbusObject (BTP_D_OUT, NAV_LED, "", eventhandlerpedestal, 18, 5, 0);
	MkFsbusObject (BTP_D_OUT, HF1_LED, "", eventhandlerpedestal, 18, 6, 0);
	MkFsbusObject (BTP_D_OUT, HF2_LED, "", eventhandlerpedestal, 18, 7, 0);
	MkFsbusObject (BTP_D_OUT, VHF1_LED, "", eventhandlerpedestal, 18, 8, 0);
	MkFsbusObject (BTP_D_OUT, VHF2_LED, "", eventhandlerpedestal, 18, 9, 0);
	MkFsbusObject (BTP_D_OUT, VHF3_LED, "", eventhandlerpedestal, 18, 10, 0);
	MkFsbusObject (BTP_D_OUT, SEL_LED, "", eventhandlerpedestal, 18, 12, 0);
	MkFsbusObject (BTP_D_OUT, AM_LED, "", eventhandlerpedestal, 18, 11, 0);
	//FsbusWriteFmt3(11, 124, 7);
	//FsbusWriteFmt3(11, 124, 1);

//---------------------------XP OBJECTS-----------------------------//
	MkFsObject (XP_LTHROTTLE_1, "", eventhandlerpedestal, 0x5914, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (XP_LTHROTTLE_2, "", eventhandlerpedestal, 0x5916, 2, TP_UI16, FS_NORMAL, 0);
	//MkFsObject (THROTTLE_GATE_L, "", eventhandlerpedestal, 0x5490, 4, TP_I16, FS_NORMAL, 0);
	MkFsObject (THROTTLE_GATE_R, "", eventhandlerpedestal, 0x5491, 4, TP_UI16, FS_NORMAL, 0);
	MkFsObject (N1_SET, "", eventhandlerpedestal, 0x04EE, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (MCDU_LIGHTS, "", eventhandlerpedestal, 0x052C, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (COM1_FREQ, "", eventhandlerpedestal, 0x034E, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (COM_FREQ_25, "", eventhandlerpedestal, 0x0358, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (COM1_FREQ_STD, "", eventhandlerpedestal, 0x311A, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (COM2_FREQ, "", eventhandlerpedestal, 0x3118, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (COM2_FREQ_STD, "", eventhandlerpedestal, 0x311C, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (FS_FLIP_FLOP, "", eventhandlerpedestal, 0x3123, 1, TP_UI8, FS_NORMAL, 0);
	MkFsObject (AUDIO_SWITCH, "", eventhandlerpedestal, 0x3122, 1, TP_UI8, FS_NORMAL, 0);
	MkFsObject (VOR1_ACTIVE, "", eventhandlerpedestal, 0x0350, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (VOR1_STANDBY, "", eventhandlerpedestal, 0x311E, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (NAV2_ACTIVE, "", eventhandlerpedestal, 0x0352, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (NAV2_STANDBY, "", eventhandlerpedestal, 0x3120, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (ADF1_ACTIVE, "", eventhandlerpedestal, 0x034C, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (ATT_HDG_SW, "", eventhandlerpedestal, 0x5794, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (AIR_DATA_SW, "", eventhandlerpedestal, 0x5795, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (EIS_DMC_SW, "", eventhandlerpedestal, 0x5796, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (ECAM_SW, "", eventhandlerpedestal, 0x5797, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (RADIO_25, "", eventhandlerpedestal, 0x0358, 2, TP_UI16, FS_NORMAL, 0);
}


void SyncroPedestalpanel()
{
	//short n1set=3;
	//FsWriteDirect(0x04EE, 2, &n1set);
	//printf("throttle2 is %d",n1);
	FsbusWriteFmt3(15,131,0);
	FsbusWriteFmt3(16,131,0);
}

void eventhandlerpedestal (int oid, int val, double dval)
{
	// TIME CALCULATION FUNCTION
	startTime2_ = GetTickCount();


	switch (oid)
	{
		


		case L_THROTTLE:
			//l_throttled=val;
			//printf("throttle is %d",val);
			if (val>=69){
				//if (val<=71 && val>69){
				//FsWriteInt(THROTTLE_GATE_L,3);}
				//else if (val<101 && val>=96	){
				//	FsWriteInt(THROTTLE_GATE_L, 4);}
				//else if (val>=101 && val<=103){
				//	FsWriteInt(THROTTLE_GATE_L, 5);}
				//else if (val>=104){
				//	FsWriteInt(THROTTLE_GATE_L, 6);}
				//else{
					int l_throttlecal=Calibrate(val, l_throttle, 2);
					FsWriteInt(XP_LTHROTTLE_1, l_throttlecal);}
					//FsWriteInt(THROTTLE_GATE_L, 0);}			
			
			else if (val<=68){
			//	if (val<=68 && val>66){
			//		FsWriteInt(THROTTLE_GATE_L,2);}
			//	else if (val<65){
			//		FsWriteInt(THROTTLE_GATE_L,1);}
				//else{
				int l_throttlecal_rev=Calibrate(val, l_throttle_rev, 2);
				FsWriteInt(XP_LTHROTTLE_1, l_throttlecal_rev);}
			//}
		break;

		//case N1_SET:
		//	printf("throttle2 is %d",val);
		//break;
		 
		case R_THROTTLE:
			//l_throttled=val;
			//printf("throttle is %d",val);
			if (val>=69){
				if (val<72 && val>69){
					FsWriteInt(THROTTLE_GATE_R, 3);}
				//	r_throttlegate=3;}
				//else if (val>=98 && val<=101){
				//	FsWriteInt(THROTTLE_GATE_R, 4);
				//	r_throttlegate=4;}
				//else if (val>=102 && val<=105){
				//	FsWriteInt(THROTTLE_GATE_R, 5);
				//	r_throttlegate=5;}
				//else if (val>105){
				//	FsWriteInt(THROTTLE_GATE_R, 6);
				//	r_throttlegate=6;}	
				//else{
					int r_throttlecal=Calibrate(val, r_throttle, 2);
					FsWriteInt(XP_LTHROTTLE_2, r_throttlecal);//}			
			}
			 else if (val<64){
				if (val<=64	 && val>60){
					FsWriteInt(THROTTLE_GATE_R,2);
					r_throttlegate=2;}
				else if (val<60){
					FsWriteInt(THROTTLE_GATE_R,1);}
					r_throttlegate=1;}
				//int r_throttlecal_rev=Calibrate(val, r_throttle_rev, 2);
				//FsWriteInt(XP_LTHROTTLE_2, r_throttlecal_rev);}
		break;

		case MCDU_FAIL:
			if (val & TWO){
				FsbusWrite(MCDU_FAIL, 1);}
			else if (val | TWO){
				FsbusWrite(MCDU_FAIL, 0);}
		break;

		//case THROTTLE_GATE_R:
		//	printf("throttle2 is %d",val);
		//if (val>0){
		//	r_throttlegate=val;} 
			//printf("throttle2 is %d",r_throttlegate);}
		//else if (val==0){
		//		do{
		//		FsWriteDirect(0x5491, 4, &r_throttlegate);
		//		FsReadDirect(0x5491, 4, &r_throttlegate);
		//		} while (r_throttlegate==0);}

//		break;


	//	case THROTTLE_GATE_L:
	//		FsWriteDirect(0x5491, 4, &r_throttlegate);
	//	break;


	//------------------- RADIO PANEL ---------------------------//

		case ON_OFF:
			if (val==0){
				FsbusWriteFmt3(15,131,0);
				FsbusWriteFmt3(16,131,0);}
			else if (val==1){
				FsbusWriteFmt3(15,131,100);
				FsbusWriteFmt3(16,131,100);}

	case VHF1:
			if (val==0){
				vhf1_var=0;
				vhf2_var=1;
				vhf3_var=1;
				vor_active=0;
				ils_active=0;
				nav_active=0;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsbusWrite(VHF1_LED,1);
				FsWriteInt(AUDIO_SWITCH,SEVEN);
				FsbusWrite(VHF2_LED,0);
				FsbusWrite(VHF3_LED,0);
				FsbusWrite(ILS_LED,0);
				FsbusWrite(HF1_LED,0);
				FsbusWrite(VOR2_LED,0);
				FsbusWrite(NAV_LED,0);
				FsbusWrite(HF2_LED,0);
				FsbusWrite(SEL_LED,0);
				FsbusWrite(AM_LED,0);
				FsReadDirect(0x034E,2,&com1);
				FsbusWrite(ACTIVE, 10*(BCD2Int(com1)+10000));
				FsReadDirect(0x311A,2,&com1_std);
				FsbusWrite(STANDBY, 10*(BCD2Int(com1_std)+10000));}
	break;

	case COM1_FREQ:
			if (vhf1_var==0){
				//printf("throttle2 is %d", BCD2Int(val));
				com1=val;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				FsbusWrite(ACTIVE, 10*(BCD2Int(val)+10000));}
	break;

	case COM1_FREQ_STD:
			if (vhf1_var==0){
				//if (10*BCD2Int(val)+10000<118000){
				//	FsbusWrite(STANDBY, 118000);}
				com1_std=val;
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsbusWrite(STANDBY, 10*(BCD2Int(val)+10000));}
	break;

	case VHF2:
			if (val==0){
				vhf2_var=0;
				vor_active=0;
				ils_active=0;
				vhf1_var=1;
				vhf3_var=1;
				nav_active=0;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsbusWrite(VHF1_LED,0);
				FsbusWrite(VHF2_LED,1);
				FsbusWrite(VHF3_LED,0);
				FsbusWrite(ILS_LED,0);
				FsWriteInt(AUDIO_SWITCH,SIX);
				FsbusWrite(HF1_LED,0);
				FsbusWrite(HF2_LED,0);
				FsbusWrite(VOR2_LED,0);
				FsbusWrite(NAV_LED,0);
				FsbusWrite(SEL_LED,1);
				FsbusWrite(AM_LED,0);
				FsReadDirect(0x3118,2,&com2);
				FsbusWrite(ACTIVE, 10*(BCD2Int(com2)+10000));
				FsReadDirect(0x311C,2,&com2_std);
				FsbusWrite(STANDBY, 10*(BCD2Int(com2_std)+10000));}
	break;

	case COM2_FREQ:
			if (vhf2_var==0){
				//printf("throttle2 is %d", BCD2Int(val));
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				FsbusWrite(ACTIVE, 10*(BCD2Int(val)+10000));}
	break;

	case COM2_FREQ_STD:
			if (vhf2_var==0){
				DisplayOptions(STANDBY, 6, 0, true, 4);
				com2_std=val;
				FsbusWrite(STANDBY, 10*(BCD2Int(val)+10000));}
	break;

	case FLIP_FLOP:
		if (val==0){
		if(vhf1_var==0){
			FsWriteInt(FS_FLIP_FLOP,TREE);}
		else if(vhf2_var==0){
			FsWriteInt(FS_FLIP_FLOP,TWO);}
		else if(ils_active==1){
			FsWriteInt(FS_FLIP_FLOP,ONE);}
		else if(vor_active==1){
			FsWriteInt(FS_FLIP_FLOP,ZERO);}
		}
	break;


	case VHF3:
			if (val==0){
				vhf3_var=0;
				DisplayOptions(ACTIVE, 6, 0, true, 0);
				DisplayOptions(STANDBY, 6, 0, true, 0);
				FsbusWrite(VHF1_LED,0);
				FsbusWrite(ILS_LED,0);
				FsbusWrite(VOR2_LED,0);
				FsbusWrite(VHF2_LED,0);
				FsbusWrite(VHF3_LED,1);
				FsbusWrite(SEL_LED,0);
				//FsbusWrite(HF1_LED,0);
				//FsbusWrite(HF2_LED,0);
				FsbusWrite(ACTIVE, DISPLAY_BAR);
				//FsReadDirect(0x311C,2,&com3_std);
				FsbusWrite(STANDBY, DISPLAY_BAR);}
	break;

		case HF1:
			if (val==0){
				DisplayOptions(ACTIVE, 6, 0, true, 0);
				DisplayOptions(STANDBY, 6, 0, true, 0);
				FsbusWrite(VHF1_LED,0);
				FsbusWrite(VHF2_LED,0);
				FsbusWrite(VHF3_LED,0);
				FsbusWrite(HF1_LED,1);
				FsbusWrite(SEL_LED,0);
				FsbusWrite(HF2_LED,0);
				//FsbusWrite(AM_LED,0);
				FsbusWrite(ACTIVE, DISPLAY_BAR);
				FsbusWrite(STANDBY, DISPLAY_BAR);}
	break;

	case AM:
			if (val==0){
				DisplayOptions(ACTIVE, 6, 0, true, 0);
				DisplayOptions(STANDBY, 6, 0, true, 0);
				FsbusWrite(VHF1_LED,0);
				FsbusWrite(VOR2_LED,0);
				FsbusWrite(VHF2_LED,0);
				FsbusWrite(VHF3_LED,0);
				FsbusWrite(HF1_LED,0);
				FsbusWrite(SEL_LED,0);
				FsbusWrite(HF2_LED,0);
				FsbusWrite(AM_LED,1);
				FsbusWrite(ACTIVE, DISPLAY_BAR);
				FsbusWrite(STANDBY, DISPLAY_BAR);}
	break;

	case HF2:
			if (val==0){
				DisplayOptions(ACTIVE, 6, 0, true, 0);
				DisplayOptions(STANDBY, 6, 0, true, 0);
				FsbusWrite(VHF1_LED,0);
				FsbusWrite(VHF2_LED,0);
				FsbusWrite(VHF3_LED,0);
				FsbusWrite(HF1_LED,0);
				FsbusWrite(ILS_LED,0);
				FsbusWrite(HF2_LED,1);
				FsbusWrite(AM_LED,0);
				FsbusWrite(ACTIVE, DISPLAY_BAR);
				FsbusWrite(STANDBY, DISPLAY_BAR);}
	break;

	case NAV:
			if (val==0 && nav_active==0){
				nav_active=1;
				ils_active=0;
				vor_active=0;
				vhf1_var=1;
				vhf2_var=1;
				vhf3_var=1;
				DisplayOptions(ACTIVE, 6, 0, true, 0);
		 		DisplayOptions(STANDBY, 6, 0, true, 0);
				FsbusWrite(NAV_LED,1);
				FsbusWrite(VHF1_LED,0);
				FsbusWrite(VHF2_LED,0);
				FsbusWrite(VHF3_LED,0);
				FsbusWrite(VOR2_LED,0);
				FsbusWrite(HF1_LED,0);
				FsbusWrite(SEL_LED,0);
				FsbusWrite(HF2_LED,0);
				FsbusWrite(ACTIVE, DISPLAY_STD);
				FsbusWrite(STANDBY, DISPLAY_STD);
				}
			else if (val==0 && nav_active==1){
				nav_active=0;
				DisplayOptions(ACTIVE, 6, 0, true, 0);
				DisplayOptions(STANDBY, 6, 0, true, 0);
				FsbusWrite(NAV_LED,0);}
	break;

	case VOR:
			if (val==0 && nav_active==1 && vor_active==0){
				vor_active=1;
				ils_active=0;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsReadDirect(0x0352,2,&vor);
				FsbusWrite(ACTIVE, 10*(BCD2Int(vor)+10000));
				FsReadDirect(0x3120,2,&vor_std);
				FsbusWrite(STANDBY, 10*(BCD2Int(vor_std)+10000));
				FsbusWrite(VOR2_LED,1);
				FsbusWrite(ILS_LED,0);}

	break;

	case ILS:
			if (val==0 && nav_active==1 && ils_active==0){
				ils_active=1;
				vor_active=0;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsReadDirect(0x0350,2,&ils);
				FsbusWrite(ACTIVE, 10*(BCD2Int(ils)+10000));
				FsReadDirect(0x311E,2,&ils_std);
				FsbusWrite(STANDBY, 10*(BCD2Int(ils_std)+10000));
				FsbusWrite(VOR2_LED,0);
				FsbusWrite(ILS_LED,1);}
	break;


	case VOR1_STANDBY:
		if (ils_active==1){
				//if (10*BCD2Int(val)+10000<118000){
				//	FsbusWrite(STANDBY, 118000);}
				ils_std=val;
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsbusWrite(STANDBY, 10*(BCD2Int(val)+10000));}
	break;

	case VOR1_ACTIVE:
			if (ils_active==1){
				ils=val;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				FsbusWrite(ACTIVE, 10*(BCD2Int(val)+10000));
}
	break;


	case NAV2_STANDBY:
		if (vor_active==1){
				//if (10*BCD2Int(val)+10000<118000){
				//	FsbusWrite(STANDBY, 118000);}
				vor_std=val;
				DisplayOptions(STANDBY, 6, 0, true, 4);
				FsbusWrite(STANDBY, 10*(BCD2Int(val)+10000));}
	break;

	case NAV2_ACTIVE:
			if (vor_active==1){
				vor=val;
				DisplayOptions(ACTIVE, 6, 0, true, 4);
				FsbusWrite(ACTIVE, 10*(BCD2Int(val)+10000));
}


	case BIG_ROT:
		// TWO ROTATION SPEED SELECTION 
		if (rotator==1){
			rotator=0;
			time1=startTime2_;
			}
		else if (rotator==0){
			rotator=1;
			time2=startTime2_;
			}
		deltaT=abs(time2-time1);
		//end of time calculation
		if (deltaT<20){
			val=val*4;}


		//printf("throttle2 is %d",BCD2Int(com1_std));
		if (vhf1_var==0 && vhf2_var==1){
		if(BCD2Int(com1_std)<=1800){
			com1_std=Int2BCD(3600);}
		//else if(BCD2Int(com1_std)>=3600){
		//	com1_std=Int2BCD(1800);}
		else {com11=BCD2Int(com1_std)+val*100;
		FsWriteInt(COM1_FREQ_STD,Int2BCD(com11));}}

		else if(vhf2_var==0 && vhf1_var==1){
			if(BCD2Int(com2_std)<=1800){
			com1_std=Int2BCD(3600);}
		//else if(BCD2Int(com1_std)>=3600){
		//	com1_std=Int2BCD(1800);}
		else {com22=BCD2Int(com2_std)+val*100;
		FsWriteInt(COM2_FREQ_STD,Int2BCD(com22));}}

		else if (ils_active==1){
			ils22=BCD2Int(ils_std)+val*100;
			FsWriteInt(VOR1_STANDBY,Int2BCD(ils22));}

		else if (vor_active==1){
			vor22=BCD2Int(vor_std)+val*100;
			FsWriteInt(NAV2_STANDBY,Int2BCD(vor22));}

	break;

	case SMALL_ROT:

		if (rotator==1){
			rotator=0;	
			time1=startTime2_;
			FsWriteInt(RADIO_25,TRUE);
			}
		else if (rotator==0){
			rotator=1;
			time2=startTime2_;
			}
		deltaT=abs(time2-time1);
		//end of time calculation
		if (deltaT<1){
			val=val*10;}

		//printf("throttle2 is %d",BCD2Int(com1_std));
		if (vhf1_var==0){
			//FsWriteInt(COM_FREQ_25,1);
		//if(BCD2Int(com1_std)<=1800){
		//	com1_std=Int2BCD(3600);}
		//else if(BCD2Int(com1_std)>=3600){
		//	com1_std=Int2BCD(1800);}
		com11=BCD2Int(com1_std)+val*5;
		FsWriteInt(COM1_FREQ_STD,Int2BCD(com11));}

		else if (vhf2_var==0){
		//if(BCD2Int(com1_std)<=1800){
		//	com1_std=Int2BCD(3600);}
		//else if(BCD2Int(com1_std)>=3600){
		//	com1_std=Int2BCD(1800);}
		//com22=BCD2Int(com2_std)+val*5;
		  com22=BCD2Int(com2_std)+val*5;
		FsWriteInt(COM2_FREQ_STD,Int2BCD(com22));}

		else if (ils_active==1){
		  ils22=BCD2Int(ils_std)+val*5;
		FsWriteInt(VOR1_STANDBY,Int2BCD(ils22));}

		else if (vor_active==1){
		  vor22=BCD2Int(vor_std)+val*5;
		FsWriteInt(NAV2_STANDBY,Int2BCD(vor22));}

	break;


	//------------------------SWITCHING PANEL----------------------------

	case SW_ATT_HDG_1:
		if (val==0) {
			FsWriteInt(ATT_HDG_SW, 1);}
		else if (val==1){
			FsWriteInt(ATT_HDG_SW, 0);}
	break;
	
	case SW_ATT_HDG_2:
		if (val==1) {
			FsWriteInt(ATT_HDG_SW, 2);}
		else if (val==0) {
			FsWriteInt(ATT_HDG_SW, 0);}
	break;

	case SW_AIR_DATA_1:
		if (val==1) {
			FsWriteInt(AIR_DATA_SW, 1);}
		else if (val==0){
			FsWriteInt(AIR_DATA_SW, 0);}
	break;
	
	case SW_AIR_DATA_2:
		if (val==1) {
			FsWriteInt(AIR_DATA_SW, 0);}
		else if (val==0) {
			FsWriteInt(AIR_DATA_SW, 2);}
	break;

	case SW_EIS_DMC_1:
		if (val==1) {
			FsWriteInt(EIS_DMC_SW, 2);}
		else if (val==0){
			FsWriteInt(EIS_DMC_SW, 0);}
	break;
	
	case SW_EIS_DMC_2:
		if (val==0) {
			FsWriteInt(EIS_DMC_SW, 1);}
		else if (val==1) {
			FsWriteInt(EIS_DMC_SW, 0);}
	break;

	case SW_ECAM_ND_1:
		if (val==1) {
			FsWriteInt(GLASS_ND_MODE, 333);}
		else if (val==0){
			FsWriteInt(GLASS_ND_MODE, 1);}
			
	break;

	case SW_ECAM_ND_2:
		if (val==0) {
			FsWriteInt(GLASS_ND_MODE, 334);}
		else if (val==1) {
			//FsWriteInt(GLASS_ND_MODE, 101);
			//Sleep(600);
			FsWriteInt(GLASS_ND_MODE, 101);}
	break;

}}