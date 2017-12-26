#include "stdafx.h"
static int Value;
static int Airspeed,Hdg,Alt,Vs,Fpa=0;
static int led,terr_cpt=0;
static int ap1,ap2,count,led_value_0,led_value_1,led_value_delta,dash,rotator,deltaT,time1,time2=0;
static int count2,led_value_02,led_value_12,led_value_delta2,prior_left,af,bf,cf,df,ef,qnh_switch_fo=0;
static int spd_mach,tkr,Qnh,i,altit,apuon,track,n,a,b,c,d,e,f,g,h,fd,mach,vel,qnh_switch,in_hpa_cpt,terr,fo_qnh=0;
static float vs=0;
using namespace std;

unsigned long startTime_;

#define SPD_DASH 0x01
#define HDG_DASH 0x02
#define VS_DASH 0x04
#define QNH_DASH 0x08
#define LS_LEFT 0x800
#define FD_LEFT 0x200
#define TKR_FPA 0x80
#define FOURTEEN 0x4000
#define WARNTIMER 2570

#define	AP1 0x01
#define	AP2 0x02
#define	VS 0x04
#define	ALT_HDL 0x08
#define	APP 0x10
#define	LOC 0x20
#define	LAT 0x40
#define	HDG 0x80
#define	SPD 0x200
#define	THR 0x800
#define	SPD_M 0x2000
#define	HDG_M 0x4000
#define MACH 0x8000
#define CAUT 0x01
#define WARN 0x04
#define AUTOLAND 0x01
#define QNH_MINUS 0x40000
#define QNH_PLUS 0x80000
#define PRIOR11 0x04
#define PRIOR12 0x02
#define PRIOR21 0x10
#define PRIOR22 0x08

void BuildglareObjects ()
{
	MkFsbusObject (BTP_DISPLAY, SPEED_DISPLAY, "speed display fsbus",eventhandler, 7, 0,0);
	MkFsbusObject (BTP_DISPLAY, HDG_DISPLAY, "",eventhandler, 5, 0, 0);
	MkFsbusObject (BTP_DISPLAY, ALT_DISPLAY, "",eventhandler, 6, 0, 0);
	MkFsbusObject (BTP_DISPLAY, VS_DISPLAY, "",eventhandler, 2, 0, 0);
	MkFsbusObject (BTP_DISPLAY, QNH_CPT_DISPLAY, "",eventhandler, 9, 0, 0);
	MkFsbusObject (BTP_DISPLAY, QNH_FO_DISPLAY, "",eventhandler, 8, 0, 0);
	MkFsbusObject (BTP_ROTARY,SPEED_ROT,"OID", eventhandler, 4, 36, 0);
	MkFsbusObject (BTP_ROTARY,HDG_ROT,"OID", eventhandler, 4, 32, 0);
	MkFsbusObject (BTP_ROTARY,ALT_ROT,"OID", eventhandler, 4, 4, 0);
	MkFsbusObject (BTP_ROTARY,VS_ROT,"OID", eventhandler, 4, 0, 0);
	MkFsbusObject (BTP_D_OUT, ATH_LED, "", eventhandler, 4, 10, 0);
	MkFsbusObject (BTP_D_OUT, AP1_LED, "", eventhandler, 4, 8, 0);
	MkFsbusObject (BTP_D_OUT, AP2_LED, "", eventhandler, 4, 9, 0);
	MkFsbusObject (BTP_D_OUT, LOC_LED, "", eventhandler, 4, 13, 0);
	MkFsbusObject (BTP_D_OUT, EXPED_LED, "", eventhandler, 4, 11, 0);
	MkFsbusObject (BTP_D_OUT, APPR_LED, "", eventhandler, 4, 12, 0);
	MkFsbusObject (BTP_D_OUT, HDG_DOT, "", eventhandler, 4, 14, 0);
	MkFsbusObject (BTP_D_OUT, SPD_DOT, "", eventhandler, 4, 15, 0);
	MkFsbusObject (BTP_D_OUT, ALT_DOT, "", eventhandler, 4, 7, 0);
	MkFsbusObject (BTP_D_OUT, HDG_VS, "", eventhandler, 4, 6, 0);
	MkFsbusObject (BTP_D_OUT, TRK_FPA, "", eventhandler, 4, 1, 0);
	MkFsbusObject (BTP_D_OUT, HDG_LED, "", eventhandler, 4, 4, 0);
	MkFsbusObject (BTP_D_OUT, TRK_LED, "", eventhandler, 4, 3, 0);
	MkFsbusObject (BTP_D_OUT, LAT_LED, "", eventhandler, 4, 2, 0);
	MkFsbusObject (BTP_D_OUT, SPD_LED, "", eventhandler, 3, 6, 0);
	MkFsbusObject (BTP_D_OUT, FPA_LED, "", eventhandler, 3, 13, 0);
	MkFsbusObject (BTP_D_OUT, MACH_LED, "", eventhandler, 4, 5, 0);
	MkFsbusObject (BTP_D_OUT, VS_LED, "", eventhandler, 3, 7, 0);
	MkFsbusObject (BTP_D_OUT, ALT_LED, "", eventhandler, 4, 0, 0);
	MkFsbusObject (BTP_D_OUT, ARPT_LED, "", eventhandler, 3, 48, 0);
	MkFsbusObject (BTP_D_OUT, NDB_LED, "", eventhandler, 3, 49, 0);
	MkFsbusObject (BTP_D_OUT, VOR_LED, "", eventhandler, 3, 50, 0);
	MkFsbusObject (BTP_D_OUT, WPT_LED, "", eventhandler, 3, 51, 0);
	MkFsbusObject (BTP_D_OUT, CSTR_LED, "", eventhandler, 3, 52, 0);
	MkFsbusObject (BTP_D_OUT, LS_LED, "", eventhandler, 3, 53, 0);
	MkFsbusObject (BTP_D_OUT, FD_LED, "", eventhandler, 3, 54, 0);
	MkFsbusObject (BTP_D_OUT, GLARE_LIGHTS, "", eventhandler, 3, 55, 0);
	MkFsbusObject (BTP_D_IN, ATH_SWCH, "", eventhandler, 4, 21, 0);
	MkFsbusObject (BTP_D_IN, AP1_SWCH, "", eventhandler, 4, 16, 0);
	MkFsbusObject (BTP_D_IN, AP2_SWCH, "", eventhandler, 4, 18, 0);
	MkFsbusObject (BTP_D_IN, LOC_SWCH, "", eventhandler, 4, 19, 0);
	MkFsbusObject (BTP_D_IN, EXPED_SWCH, "", eventhandler, 4, 20, 0);
	MkFsbusObject (BTP_D_IN, APPR_SWCH, "", eventhandler, 4, 23, 0);
	MkFsbusObject (BTP_D_IN, PUSH_HDG, "", eventhandler, 4, 52, 0);
	MkFsbusObject (BTP_D_IN, PULL_HDG, "", eventhandler, 4, 53, 0);
	MkFsbusObject (BTP_D_IN, PUSH_SPD, "", eventhandler, 4, 54, 0);
	MkFsbusObject (BTP_D_IN, PULL_SPD, "", eventhandler, 4, 55, 0);
	MkFsbusObject (BTP_D_IN, PUSH_ALT, "", eventhandler, 4, 50, 0);
	MkFsbusObject (BTP_D_IN, PULL_ALT, "", eventhandler, 4, 51, 0);
	MkFsbusObject (BTP_D_IN, PUSH_VS, "", eventhandler, 4, 49, 0);
	MkFsbusObject (BTP_D_IN, PULL_VS, "", eventhandler, 4, 48, 0);
	MkFsbusObject (BTP_D_IN, SPD_MACH, "", eventhandler, 4, 17, 0);
	MkFsbusObject (BTP_D_IN, TRACK, "", eventhandler, 4, 22, 0);
	MkFsbusObject (BTP_D_IN, ALTSELECTOR, "", eventhandler, 11, 47, 0);
	
//--------------------------POWER ON BUS OBJECTS------------------//
	FsbusPowerOffOptions(HDG_VS, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(HDG_LED, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(LAT_LED, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(SPD_LED, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(VS_LED, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(ALT_LED, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(GLARE_LIGHTS, PWRTYPE_MAINBUS, 0);

//---------------------------CPT EFIS----------------------------//
	MkFsbusObject (BTP_D_IN, CPTFD, "", eventhandler, 4, 43, 0);
	MkFsbusObject (BTP_D_IN, CPTVOR, "", eventhandler, 4, 31, 0);
	MkFsbusObject (BTP_D_IN, CPTCSTR, "", eventhandler, 4, 47, 0);
	MkFsbusObject (BTP_D_IN, CPTWPT, "", eventhandler, 4, 30, 0);
	MkFsbusObject (BTP_D_IN, CPTNDB, "", eventhandler, 4, 45, 0);
	MkFsbusObject (BTP_D_IN, CPTARPT, "", eventhandler, 4, 46, 0);
	MkFsbusObject (BTP_D_IN, CPTLS, "", eventhandler, 4, 44, 0);
	MkFsbusObject (BTP_D_IN, QNH_CPT_ROT, "", eventhandler, 4, 40, 0);
	MkFsbusObject (BTP_D_IN, MODE_CPT_1, "", eventhandler, 4,29 , 0);
	MkFsbusObject (BTP_D_IN, MODE_CPT_2, "", eventhandler, 4, 24, 0);
	MkFsbusObject (BTP_D_IN, MODE_CPT_3, "", eventhandler, 4, 26, 0);
	MkFsbusObject (BTP_D_IN, MODE_CPT_4, "", eventhandler, 4, 25, 0);
	MkFsbusObject (BTP_D_IN, QNH_PUSH_CPT, "", eventhandler, 4, 42, 0);
	MkFsbusObject (BTP_D_IN, QNH_IN_PHA_CPT, "", eventhandler, 11, 41, 0);
	MkFsbusObject (BTP_D_IN, RANGE_CPT_1, "", eventhandler, 4,56, 0);
	MkFsbusObject (BTP_D_IN, RANGE_CPT_2, "", eventhandler, 4,57, 0);
	MkFsbusObject (BTP_D_IN, RANGE_CPT_3, "", eventhandler, 4,58, 0);
	MkFsbusObject (BTP_D_IN, RANGE_CPT_4, "", eventhandler, 4,59, 0);
	MkFsbusObject (BTP_D_IN, RANGE_CPT_5, "", eventhandler, 4,60 , 0);
	MkFsbusObject (BTP_D_IN, RANGE_CPT_6, "", eventhandler, 4,61 , 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_1_1, "", eventhandler, 4,62 , 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_1_2, "", eventhandler, 4,63 , 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_2_1, "", eventhandler, 11,43 , 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_2_2, "", eventhandler, 11,46 , 0);
	MkFsbusObject (BTP_D_IN, TERR_ND_CPT, "", eventhandler, 11, 16, 0);
	MkFsbusObject (BTP_D_OUT, TERR_LED_CPT, "", eventhandler, 3, 23, 0);


//----------------------------FO EFIS-----------------------------//
	MkFsbusObject (BTP_D_IN, FOFD, "", eventhandler, 11, 63, 0);
	MkFsbusObject (BTP_D_IN, FOVOR, "", eventhandler, 11, 58, 0);
	MkFsbusObject (BTP_D_IN, FOCSTR, "", eventhandler, 11, 60, 0);
	MkFsbusObject (BTP_D_IN, FOWPT, "", eventhandler, 11, 59, 0);
	MkFsbusObject (BTP_D_IN, FONDB, "", eventhandler, 11, 57, 0);
	MkFsbusObject (BTP_D_IN, FOARPT, "", eventhandler, 11, 56, 0);
	MkFsbusObject (BTP_D_IN, TERR_ND_FO, "", eventhandler, 4, 9, 0);
	MkFsbusObject (BTP_D_IN, FOLS, "", eventhandler, 11, 61, 0);
	MkFsbusObject (BTP_D_IN, QNH_FO_ROT, "", eventhandler, 11, 8, 0);
	MkFsbusObject (BTP_D_IN, MODE_FO_1, "", eventhandler, 11, 11 , 0);
	MkFsbusObject (BTP_D_IN, MODE_FO_2, "", eventhandler, 11, 12, 0);
	MkFsbusObject (BTP_D_IN, MODE_FO_3, "", eventhandler, 11, 15, 0);
	//MkFsbusObject (BTP_D_IN, MODE_FO_4, "", eventhandler, 11, 14, 0);
	MkFsbusObject (BTP_D_IN, QNH_PUSH_FO, "", eventhandler, 11, 10, 0);
	MkFsbusObject (BTP_D_IN, QNH_IN_PHA_FO, "", eventhandler, 11, 62, 0);
	MkFsbusObject (BTP_D_IN, RANGE_FO_1, "", eventhandler, 11, 24, 0);
	MkFsbusObject (BTP_D_IN, RANGE_FO_2, "", eventhandler, 11, 25, 0);
	MkFsbusObject (BTP_D_IN, RANGE_FO_3, "", eventhandler, 11, 26, 0);
	MkFsbusObject (BTP_D_IN, RANGE_FO_4, "", eventhandler, 11, 27, 0);
	MkFsbusObject (BTP_D_IN, RANGE_FO_5, "", eventhandler, 11, 28 , 0);
	MkFsbusObject (BTP_D_IN, RANGE_FO_6, "", eventhandler, 11, 29 , 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_1_1_FO, "", eventhandler, 11, 53, 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_1_2_FO, "", eventhandler, 11, 52, 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_2_1_FO, "", eventhandler, 11, 30, 0);
	MkFsbusObject (BTP_D_IN, VORADF_L_2_2_FO, "", eventhandler, 11, 31, 0);
//----------------------OUT LEDS------------------------------------//
	MkFsbusObject (BTP_D_OUT, ARPT_LED_FO, "", eventhandler, 3, 59, 0);
	MkFsbusObject (BTP_D_OUT, NDB_LED_FO, "", eventhandler, 3, 60, 0);
	MkFsbusObject (BTP_D_OUT, VOR_LED_FO, "", eventhandler, 3, 61, 0);
	MkFsbusObject (BTP_D_OUT, WPT_LED_FO, "", eventhandler, 3, 62, 0);
	MkFsbusObject (BTP_D_OUT, CSTR_LED_FO, "", eventhandler, 3, 63, 0);
	MkFsbusObject (BTP_D_OUT, LS_LED_FO, "", eventhandler, 3, 57, 0);
	MkFsbusObject (BTP_D_OUT, FD_LED_FO, "", eventhandler, 3, 58, 0);
	MkFsbusObject (BTP_D_OUT, TERR_LED_FO, "", eventhandler, 3, 46, 0);


//----------------------WARN SYSTEM------------------------------//
	MkFsbusObject (BTP_D_OUT, MWARNCPT, "", eventhandler, 3, 3, 0);
	MkFsbusObject (BTP_D_OUT, MWARNFO, "", eventhandler, 3, 12, 0);
	MkFsbusObject (BTP_D_OUT, MCAUTCPT, "", eventhandler, 3, 4, 0);
	MkFsbusObject (BTP_D_OUT, MCAUTFO, "", eventhandler, 3, 11, 0);
	MkFsbusObject (BTP_D_OUT, AUTOLANDCPT, "", eventhandler, 3, 2, 0);
	MkFsbusObject (BTP_D_OUT, AUTOLANDFO, "", eventhandler, 3, 9, 0);
	MkFsbusObject (BTP_D_OUT, CAPT, "", eventhandler, 3, 0, 0);
	MkFsbusObject (BTP_D_OUT, FO, "", eventhandler, 3, 10, 0);
	MkFsbusObject (BTP_D_OUT, PRIORLEFT, "", eventhandler, 3, 8, 0);
	MkFsbusObject (BTP_D_OUT, PRIORRIGHT, "", eventhandler, 3, 1, 0);
	MkFsbusObject (BTP_D_IN, MWARNCPT_SWITCH, "", eventhandler, 11, 45, 0);
	MkFsbusObject (BTP_D_IN, MCAUTCPT_SWITCH, "", eventhandler, 11, 40, 0);
	MkFsbusObject (BTP_D_IN, CHRONO_CPT, "", eventhandler, 11, 42, 0);
	MkFsbusObject (BTP_D_IN, CHRONO_FO, "", eventhandler, 11, 48, 0);
	MkFsbusObject (BTP_D_IN, PRIORLEFT_SWITCH, "", eventhandler, 11, 44, 0);

	
	MkFsObject (SPD_D, "", eventhandler, 0x04E0, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (HDG_D, "", eventhandler, 0x04E2, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (ALT_D, "", eventhandler, 0x04E4, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (VS_D, "", eventhandler, 0x04E6, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (MAC_D, "", eventhandler, 0x04E8, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (SPD_W, "", eventhandler, 0x5406, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (HDG_W, "", eventhandler, 0x5408, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (ALT_W, "", eventhandler, 0x540A, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (VS_W, "", eventhandler, 0x540C, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (MAC_W, "", eventhandler, 0x540E, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (PM_FCU_LED, "", eventhandler, 0x04F0, 2, TP_UI16, FS_QUICK, 0);
	MkFsObject (GLASS_ND_MODE, "", eventhandler, 0x04F4, 2, TP_UI16, FS_QUICK, 0);
	MkFsObject (JH_LED2, "", eventhandler,0x7391, 1, TP_UI8, FS_NORMAL, 0); 
	MkFsObject (JH_FCU_CONTROLS, "", eventhandler, 0x5410, 4, TP_UI32, FS_NORMAL, 0);
	MkFsObject (FCU_LED, "", eventhandler, 0x5528, 2, TP_UI16, FS_NORMAL, 0);
	MkFsObject (PM_FCU_2, "", eventhandler, 0x04F2, 2, TP_UI16, FS_QUICK, 0);
	MkFsObject(PM_KNOBS, "OID", eventhandler, 0x5418, 4,  TP_UI32,  FS_NORMAL, 0);
	MkFsObject(CPT_QNH, "OID", eventhandler, 0x5536, 2,  TP_UI16,  FS_NORMAL, 0);
	MkFsObject(FO_QNH, "OID", eventhandler, 0x552E, 2,  TP_UI16,  FS_NORMAL, 0);
	MkFsObject(FS_QNH, "OID", eventhandler, 0x0330, 2,  TP_UI16,  FS_NORMAL, 0);
	MkFsObject(WARNCAUTFIRE, "OID", eventhandler, 0x56F7, 1,  TP_UI8,  FS_NORMAL, 0);
	MkFsObject(WARNCAUTFIRE_SWITCH, "OID", eventhandler, 0x56F8, 1,  TP_UI8,  FS_NORMAL, 0);
	MkFsObject(PRIORITY, "OID", eventhandler, 0x5718, 1,  TP_UI8,  FS_NORMAL, 0);
	MkFsObject(AUTOLANDFAIL, "OID", eventhandler, 0x5719, 1,  TP_UI8,  FS_NORMAL, 0);
	MkFsbusObject (BTP_D_IN, ATHR_DISCONNECT, "", eventhandler, 11, 54, 0);
	//MkFsObject(WARNCAUTFIRE, "OID", eventhandler, 0x56F7, 1,  TP_UI8,  FS_NORMAL, 0);
	//MkFsObject(APUON3, "", eventhandler, 0x5600, 1, TP_I8, FS_QUICK, 0);

	DisplayOptions(SPEED_DISPLAY, 3, 0, true, 0);
	DisplayOptions(HDG_DISPLAY, 3, 0, true, 0);
	DisplayOptions(ALT_DISPLAY, 5, 0, true, 0);
	DisplayOptions(VS_DISPLAY, 5, 0, false, 0);
	DisplayOptions(QNH_CPT_DISPLAY, 4, 0, true, 0);
	DisplayOptions(QNH_FO_DISPLAY, 4, 0, true, 0);

	MkTimer(WARNTIMER, "", eventhandler, 0, 0); 

	}

void Syncroglare() {
	FsReadDirect(0x04E0, 2, &Airspeed); 
	FsReadDirect(0x04E2, 2, &Hdg);
	FsbusWrite(HDG_DISPLAY, Hdg);
	FsReadDirect(0x04E4, 2, &Alt);
	FsReadDirect(0x04E6, 2, &Vs);
	FsReadDirect(0x0330, 2, &Qnh);
	FsReadDirect(0x552E, 2, &fo_qnh);
	FsReadDirect(0x04E8, 2, &mach);
	
	//FsReadDirect(0x40F0, 2, &led);
	//FsbusWrite(ATH_LED, led);
	FsbusWriteFmt3(5, 133, 200); //settaggio brithness display hdg	
	FsbusWrite(QNH_CPT_DISPLAY, Qnh/16);
	FsbusWrite(QNH_FO_DISPLAY, fo_qnh/16);
	FsbusWrite(SPEED_DISPLAY, Airspeed);
	FsReadDirect(0x5600,1,&apuon);
	//if (apuon==6){
	FsbusWrite(HDG_VS, 1);
	FsbusWrite(HDG_LED, 1);
	FsbusWrite(LAT_LED, 1);
	FsbusWrite(SPD_LED, 1);
	FsbusWrite(VS_LED, 1);//}
	FsbusWrite(ALT_LED, 1);
	//FsbusWrite(GLARE_LIGHTS, 1);
}

void eventhandler (int oid, int val, double dval)
{

//------------TIME CALCULATION-------------------
	//const long double now = time(0);

	startTime_ = GetTickCount();


	switch (oid)
	{

	case SPEED_ROT:
		//two click mutual condition
		if (rotator==1){
			rotator=0;
			time1=startTime_;
			}
		else if (rotator==0){
			rotator=1;
			time2=startTime_;
			}
		deltaT=abs(time2-time1);
		//end of time calculation
		if (deltaT<20){
			val=val*10;}
		//printf("the rotation is %d",deltaT);
		if(f==0){
		if(spd_mach==0){
		Airspeed+=val;
		if (Airspeed<100){
			Airspeed=100;}
		FsWriteInt(SPD_W, Airspeed);}
		else if(spd_mach==1){
		mach+=val;
		FsWriteInt(MAC_W, mach);}}
	break;

	case HDG_ROT:
		//two click mutual condition
		if (rotator==1){
			rotator=0;
			time1=startTime_;
			}
		else if (rotator==0){
			rotator=1;
			time2=startTime_;
			}
		deltaT=abs(time2-time1);
		//end of time calculation
		if (deltaT<20){
			val=val*10;}
		if (g==0){
		Hdg+=val;
		if (Hdg > 359)
			Hdg -= 360;                          
		if (Hdg < 0)                   
			Hdg += 360; 
		FsWriteInt(HDG_W, Hdg);}
		break;

	case ALTSELECTOR:
		if (val==1 && i==0){
			altit=1;
			i=1;}
		else if (val==1 && i==1){
			altit=0;
			i=0;
		}
	break;

	case ALT_ROT:
		if (altit==0){
			Alt-=val;}
		else if (altit==1){
			Alt-=val*10;}
		FsWriteInt(ALT_W, Alt);
	break;

	case VS_ROT:
		if(h==0){
		if (track==0){
			Vs+=val*100;
			FsWriteInt(VS_W, Vs);}
		else if (track==1){
			Fpa+=val*10;
			FsWriteInt(VS_W, Fpa);}
		}
	break;

// FCU Display activity

	case HDG_D:
			FsbusWrite(HDG_DISPLAY, val);
		break;

	case SPD_D:
			if (f==0){
			FsbusWrite(SPEED_DISPLAY, val); 	
			DisplayOptions(SPEED_DISPLAY, 3, 0, false, 0);
			Airspeed=val;}
		break;

	case MAC_D:
			if (f==0){
			DisplayOptions(SPEED_DISPLAY, 3, 0, false, 3);
			FsbusWrite(SPEED_DISPLAY, val); 
			mach=val;}
	break;

	case ALT_D:
			val=val*100;
			FsbusWrite(ALT_DISPLAY, val);
		break;

	case VS_D:
		if (h==0){
		if (track==0){
			DisplayOptions(VS_DISPLAY, 5, 0, false, 0);
			if (val>8000){
				val=val-65536;}
			else if (val==65536)
				val=0;
			FsbusWrite(VS_DISPLAY, val);
			Vs=val;
			}
		else if (track==1){
			if (val>8000){
				val=val-65536;}
			else if (val==65536)
				val=0;
			if (val<100 && val>0){
				DisplayOptions(VS_DISPLAY, 2, 0, true, 2);}
			//else if (val>-100 && val<0){
			//	DisplayOptions(VS_DISPLAY, 3, 0, true, 2);}
			else 
				DisplayOptions(VS_DISPLAY, 3, 0, false, 2);
				
				FsbusWrite(VS_DISPLAY, val/10);
			Fpa=val;
			}}
		else if (h==1)
			FsbusWrite(VS_DISPLAY, DISPLAY_BAR);
		break;

	//case CPT_QNH:
	//	FsbusWrite(QNH_CPT_DISPLAY, val/16);
	//	//FsbusWrite(QNH_FO_DISPLAY, val/16);
	//break;

	case FS_QNH:
		FsbusWrite(QNH_CPT_DISPLAY, val/16);
	break;

	case FO_QNH:
		FsbusWrite(QNH_FO_DISPLAY, val/16);
	break;


//------------------FCU Leds and Dashed Display----------------//

	case FCU_LED:

//		printf("value variable is, %d", val);
		
		if (val & SPD_DASH){
			FsbusWrite(SPEED_DISPLAY, DISPLAY_BAR);
			f=1;		
			}
		else if (val | SPD_DASH){
			if (spd_mach==0){
				FsbusWrite(SPEED_DISPLAY, Airspeed);}
			else if (spd_mach==1){
				FsbusWrite(SPEED_DISPLAY, mach);}
			f=0;
			}

		if (val & HDG_DASH){
			FsbusWrite(HDG_DISPLAY, DISPLAY_BAR);
			g=1;
		}
			//FsbusWrite(HDG_DOT, 1);}
		else if (val | HDG_DASH){
			FsbusWrite(HDG_DISPLAY, Hdg);
			g=0;
		}
			//FsbusWrite(HDG_DOT, 0);}

		if (val & VS_DASH){
			h=1;
			FsbusWrite(VS_DISPLAY, DISPLAY_BAR);
			}
		else if (val | VS_DASH){
			FsReadDirect(0x04E6, 2, &Vs);
			FsbusWrite(VS_DISPLAY, Vs);
			h=0;
		}

		if (val & QNH_DASH){
			FsbusWrite(QNH_CPT_DISPLAY, DISPLAY_STD);}
		else if (val | QNH_DASH){
			FsbusWrite(QNH_CPT_DISPLAY, Qnh/16);}

	//The standby fo qnh not works maybe for a pm bug!!! 

		if (val & 0x4000){
			FsbusWrite(QNH_FO_DISPLAY, DISPLAY_STD);}
		else if (val | 0x4000){
			FsbusWrite(QNH_FO_DISPLAY, Qnh/16);}

		if (val & FD_LEFT){
			FsbusWrite(FD_LED, 1);}
		else if (val | FD_LEFT){
			FsbusWrite(FD_LED, 0);}

	//----FO FS LED -----/

		if (val & 0x400){
			FsbusWrite(FD_LED_FO, 1);}
		else if (val | 0x400){
			FsbusWrite(FD_LED_FO, 0);}
		
		if (val & TKR_FPA){
			FsbusWrite(HDG_VS,0);
			FsbusWrite(TRK_FPA,1);
			FsbusWrite(HDG_LED, 0);
			FsbusWrite(TRK_LED, 1);
			FsbusWrite(VS_LED, 0);
			FsbusWrite(FPA_LED, 1);
			FsReadDirect(0x04E6,2, &Fpa);
			//FsbusWrite(VS_DISPLAY, Fpa/10);
			DisplayOptions(VS_DISPLAY, 3, 0, false, 2);
			track=1;}
		else if (val | TKR_FPA){
			FsbusWrite(HDG_VS,1);
			FsbusWrite(TRK_FPA,0);
			FsbusWrite(HDG_LED, 1);
			FsbusWrite(VS_LED, 1);
			FsbusWrite(TRK_LED, 0);
			FsbusWrite(FPA_LED, 0);
			FsReadDirect(0x04E6,2, &Vs);
			//FsbusWrite(VS_DISPLAY, Vs);
			DisplayOptions(VS_DISPLAY, 5, 0, false, 0);
			track=0;}
		
		if (val & LS_LEFT){
			FsbusWrite(LS_LED, 1);}
		else if (val | LS_LEFT){
			FsbusWrite(LS_LED, 0);}

	//---FO LS LED---//

		if (val & 0x1000){
			FsbusWrite(LS_LED_FO, 1);}
		else if (val | 0x1000 ){
			FsbusWrite(LS_LED_FO, 0);}

		break;

	

	case PM_FCU_LED:

		if (val & AP1){
			FsbusWrite(AP1_LED, 1);}
		else if (val | AP1){
			FsbusWrite(AP1_LED, 0);}

		if (val & AP2){
			FsbusWrite(AP2_LED, 1);}
		else if (val | AP2){
			FsbusWrite(AP2_LED, 0);}

		if (val & THR){
			FsbusWrite(ATH_LED, 1);}
		else if (val | THR){
			FsbusWrite(ATH_LED, 0);}
				
		if (val & APP){
			FsbusWrite(APPR_LED, 1);}
		else if (val | APP){
			FsbusWrite(APPR_LED, 0);}
				
		if (val & LOC){
			FsbusWrite(LOC_LED, 1);}
		else if (val | LOC){
			FsbusWrite(LOC_LED, 0);}
				
		if (val & LAT){
			FsbusWrite(HDG_DOT, 1);}
		else if (val | LAT){
			FsbusWrite(HDG_DOT, 0);}

		if (val & SPD_M){
			FsbusWrite(SPD_DOT, 1);}
		else if (val | SPD_M){
			FsbusWrite(SPD_DOT, 0);}

		//if (val & SPD){
		//	FsbusWrite(SPD_LED, 1);}
		//else if (val | SPD){
		//	FsbusWrite(SPD_LED, 0);}

		if (val & MACH){
			spd_mach=1;
			FsbusWrite(MACH_LED, 1);
			FsbusWrite(SPD_LED, 0);
			}
		else if (val | MACH){
			spd_mach=0;
			FsbusWrite(MACH_LED, 0);
			FsbusWrite(SPD_LED, 1);
		}

		if (val & ALT_HDL){
			FsbusWrite(HDG_DOT, 1);}
		else if (val | ALT_HDL){
			FsbusWrite(HDG_DOT, 0);}

		if (val & HDG_M){
			FsbusWrite(ALT_DOT, 1);}
		else if (val | HDG_M){
			FsbusWrite(ALT_DOT, 0);}

		break;

// FCU push-pull activity

	case PULL_HDG:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 54);}
		break;

	case PUSH_HDG:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 55);}
		break;
	
	case PULL_SPD:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 52);}
		break;

	case PUSH_SPD:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 53);}
		break;
			
	case PULL_ALT:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 57);
		 }
		break;

	case PUSH_ALT:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 56);
			 //Sleep(300);
			 FsReadDirect(0x04E6, 2, &Vs);
			 //FsbusWrite(VS_DISPLAY, Vs);
		 }
		break;

	case PULL_VS:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 58);}
	break;

	case PUSH_VS:
		 if (val==0){
			 FsWriteInt(PM_FCU_2, 59);}
		break;

	//case PM_FCU_2:
		//printf("value variable is, %d", val);
	//		break;


// FCU switch and led activity
	
	case PM_FCU_2:
	//	if (val==37 && fd==0){
	//		fd=1;
	//		FsbusWrite(FD_LED, 1);}
	//	else if (val==37 && fd==1){
	//		FsbusWrite(FD_LED, 0);
	//		fd=0;}
	break;


	
		


//------------------FCU switch activity--------------------------//

	case ATH_SWCH:
		if (val==0){
			FsWriteInt(PM_FCU_2, 19);}
		break;

	case AP1_SWCH:
		if (val==1){
			FsWriteInt(PM_FCU_2, 32);}
		break;

	case AP2_SWCH:
		if (val==1){
			FsWriteInt(PM_FCU_2, 33);}
		break;

	case LOC_SWCH:
		if (val==1)
			FsWriteInt(PM_FCU_2, 28);
			break;
		
	case EXPED_SWCH:
		if (val==0)
			FsWriteInt(PM_FCU_2, 60);
			break;

	case APPR_SWCH:
		if (val==1)
			FsWriteInt(PM_FCU_2, 29);
			break;

	case SPD_MACH:
		if (val==1){
			if (spd_mach==1 && f==0){
			FsReadDirect(0x04E8, 2, &mach);
			DisplayOptions(SPEED_DISPLAY, 3, 0, false, 3);
			FsbusWrite(SPEED_DISPLAY, mach); 
			}
			else if (spd_mach==0 && f==0){
				FsReadDirect(0x04E0, 2, &Airspeed); 
				FsbusWrite(SPEED_DISPLAY, Airspeed); 
				DisplayOptions(SPEED_DISPLAY, 3, 0, false, 0);	
			}
			FsWriteInt(PM_FCU_2, 23);}
		break;

	case TRACK:
		if (val==0){
			FsWriteInt(PM_FCU_2, 61);}
		break;

//--------------------EFIS LOGICS CPT------------------------//

	case CPTFD:
		if (val==1)	{
		FsWriteInt(PM_FCU_2, 37);
	   }
	break;

	case CPTCSTR:
		if (val==1)	{
			if(a==0){
				FsbusWrite(CSTR_LED, 1);
				a=1;}
		else if(a==1){
				FsbusWrite(CSTR_LED, 0);
				a=0;}
		FsWriteInt(PM_FCU_2, 305);
	   }
	break;

	case CPTWPT:
		if (val==1)	{
		if(b==0){
				FsbusWrite(WPT_LED, 1);
				b=1;}
		else if(b==1){
				FsbusWrite(WPT_LED, 0);
				b=0;}
		FsWriteInt(PM_FCU_2, 303);
	   }
	break;

	case CPTVOR:
		if (val==1)	{
				if(c==0){
				FsbusWrite(VOR_LED, 1);
				c=1;}
		else if(c==1){
				FsbusWrite(VOR_LED, 0);
				c=0;}
		FsWriteInt(PM_FCU_2, 301);
	   }
	break;

	case CPTNDB:
		if (val==1)	{
				if(d==0){
				FsbusWrite(NDB_LED, 1);
				d=1;}
		else if(d==1){
				FsbusWrite(NDB_LED, 0);
				d=0;}
		FsWriteInt(PM_FCU_2, 302);
	   }
	break;

	case CPTARPT:
		if (val==1)	{
				if(e==0){
				FsbusWrite(ARPT_LED, 1);
				e=1;}
		else if(e==1){
				FsbusWrite(ARPT_LED, 0);
				e=0;}
		FsWriteInt(PM_FCU_2, 304);
	   }
	break;

	case CPTLS:
		if (val==1)	{
			FsWriteInt(PM_FCU_2, 49);
	   }
	break;

	case QNH_CPT_ROT:
		if (val==1){
			FsWriteInt(PM_KNOBS, QNH_PLUS);}
		else if (val!=1){
			FsWriteInt(PM_KNOBS, QNH_MINUS);}
	break;


	case QNH_PUSH_CPT:
		if (val==1 && qnh_switch==0){
			FsWriteInt(PM_FCU_2, 51);
			qnh_switch=1;}
		else if (val==1 && qnh_switch==1){
			FsWriteInt(PM_FCU_2, 50);
			FsReadDirect(0x0330, 2, &Qnh);
			FsbusWrite(QNH_CPT_DISPLAY, Qnh/16);
			qnh_switch=0;
			}
	break;

	case QNH_IN_PHA_CPT:
		if (val==1 && in_hpa_cpt==0){
			FsWriteInt(PM_FCU_2, 62);
			in_hpa_cpt=1;}
		else if (val==1 && in_hpa_cpt==1){
			FsWriteInt(PM_FCU_2, 63);
			in_hpa_cpt=0;}
	break;

//-------------------ND MODES------------------//

	case MODE_CPT_1:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 5);}
	break;
	
	case MODE_CPT_2:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 3);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 2);}
	break;

	case MODE_CPT_3:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 1);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 2);}
	break;
	
	case MODE_CPT_4:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 4);}
	break;

	case RANGE_CPT_1:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 41);}
	break;

	case RANGE_CPT_2:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 42);}
	break;

	case RANGE_CPT_3:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 43);}
	break;

	case RANGE_CPT_4:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 44);}
	break;

	case RANGE_CPT_5:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 45);}
	break;

	case RANGE_CPT_6:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 46);}
	break;

	
	case VORADF_L_2_1:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 74);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 73);}
	break;

	case VORADF_L_2_2:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 75);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 73);}
	break;

	case VORADF_L_1_1:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 77);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 76);}
	break;

	case VORADF_L_1_2:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 78);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 76);}
	break;

//--------------------EFIS LOGICS FO------------------------//

	case FOFD:
		if (val==1)	{
		FsWriteInt(PM_FCU_2, 17);
	   }
	break;

	case FOCSTR:
		if (val==1)	{
			if(af==0){
				FsbusWrite(CSTR_LED_FO, 1);
				af=1;}
		else if(af==1){
				FsbusWrite(CSTR_LED_FO, 0);
				af=0;}
		FsWriteInt(PM_FCU_2, 315);
	   }
	break;

	case FOWPT:
		if (val==1)	{
		if(bf==0){
				FsbusWrite(WPT_LED_FO, 1);
				bf=1;}
		else if(bf==1){
				FsbusWrite(WPT_LED_FO, 0);
				bf=0;}
		FsWriteInt(PM_FCU_2, 313);
	   }
	break;

	case FOVOR:
		if (val==1)	{
				if(cf==0){
				FsbusWrite(VOR_LED_FO, 1);
				cf=1;}
		else if(cf==1){
				FsbusWrite(VOR_LED_FO, 0);
				cf=0;}
		FsWriteInt(PM_FCU_2, 311);
	   }
	break;

	case FONDB:
		if (val==1)	{
				if(df==0){
				FsbusWrite(NDB_LED_FO, 1);
				df=1;}
		else if(df==1){
				FsbusWrite(NDB_LED_FO, 0);
				df=0;}
		FsWriteInt(PM_FCU_2, 312);
	   }
	break;

	case FOARPT:
		if (val==1)	{
				if(e==0){
				FsbusWrite(ARPT_LED_FO, 1);
				e=1;}
		else if(e==1){
				FsbusWrite(ARPT_LED_FO, 0);
				e=0;}
		FsWriteInt(PM_FCU_2, 314);
	   }
	break;

	case FOLS:
		if (val==1)	{
			FsWriteInt(PM_FCU_2, 149);
	   }
	break;

	case QNH_FO_ROT:
		if (val==1 && qnh_switch_fo==0){
			FsReadDirect(0x552E,2,&fo_qnh);
			FsWriteInt(FO_QNH, fo_qnh+1);}
		else if (val!=1 && qnh_switch_fo==0){
			FsReadDirect(0x552E,2,&fo_qnh);
			FsWriteInt(FO_QNH, fo_qnh-1);}
	break;


	case QNH_PUSH_FO:
		if (val==0 && qnh_switch_fo==0){
			FsWriteInt(PM_FCU_2, 151);
			qnh_switch_fo=1;}
			//FsbusWrite(QNH_FO_DISPLAY, DISPLAY_STD);}
		else if (val==1 && qnh_switch_fo==1){
			qnh_switch_fo=0;
			FsWriteInt(PM_FCU_2, 150);
			FsReadDirect(0x552E, 2, &Qnh);
			FsbusWrite(QNH_CPT_DISPLAY, Qnh/16);	
			}
	break;

	case QNH_IN_PHA_FO:
		if (val==1 && in_hpa_cpt==0){
			FsWriteInt(PM_FCU_2, 162);
			in_hpa_cpt=1;}
		else if (val==1 && in_hpa_cpt==1){
			FsWriteInt(PM_FCU_2, 163);
			in_hpa_cpt=0;}
	break;

//-------------------ND MODES------------------//

	case MODE_FO_1:
		if (val==1){
			FsWriteInt(GLASS_ND_MODE, 105);}
		else if (val==0){
			FsWriteInt(GLASS_ND_MODE, 103);}
	break;
	
	case MODE_FO_2:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 102);}
	break;

	case MODE_FO_3:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 101);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 104);}
	break;
	
	case MODE_FO_4:
			if (val==0){
			FsWriteInt(GLASS_ND_MODE, 14);}
	break;


	case RANGE_FO_1:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 141);}
	break;

	case RANGE_FO_2:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 142);}
	break;

	case RANGE_FO_3:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 143);}
	break;

	case RANGE_FO_4:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 144);}
	break;

	case RANGE_FO_5:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 145);}
	break;

	case RANGE_FO_6:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 146);}
	break;

	
	case VORADF_L_2_1_FO:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 175);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 173);}
	break;

	case VORADF_L_2_2_FO:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 174);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 173);}
	break;

	case VORADF_L_1_1_FO:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 177);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 176);}
	break;

	case VORADF_L_1_2_FO:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 178);}
		else if (val==1){
			FsWriteInt(GLASS_ND_MODE, 176);}
	break;

// Terrain EGPWS 
	
	case TERR_ND_CPT:
		if (val==0 && terr_cpt==0){
			FsWriteInt(GLASS_ND_MODE, 82);
			FsbusWrite(TERR_LED_CPT, 1);
			terr_cpt=1;
		}
		else if (val==0 && terr_cpt==1){
			FsWriteInt(GLASS_ND_MODE, 82);
			FsbusWrite(TERR_LED_CPT, 0);
			terr_cpt=0;
		}
	break;

	case TERR_ND_FO:
		if (val==0 && terr==0){
			FsWriteInt(GLASS_ND_MODE, 182);
			FsbusWrite(TERR_LED_FO, 1);
			terr=1;
		}
		else if (val==0 && terr==1){
			FsWriteInt(GLASS_ND_MODE, 182);
			FsbusWrite(TERR_LED_FO, 0);
			terr=0;
		}
	break;

// Glass ND Lights:

	//case GLASS_ND_MODE:
		//printf("warning system %d", val);
		//if (val==182){
		//	FsbusWrite(TERR_LED_FO, 1);
		//	terr=1;}
	//break;
//--------------------WARNING PANEL--------------------//
	case WARNCAUTFIRE:
		//printf("warning system %d", val);
		if(val & CAUT){
			//Vol(CHIME,70);
			//Play(CHIME);
			FsbusWrite(MCAUTCPT, 1);
			FsbusWrite(MCAUTFO, 1);}
		else if (val | CAUT){
			FsbusWrite(MCAUTCPT, 0);
			FsbusWrite(MCAUTFO, 0);}

		if (val & WARN){
			FsbusWrite(MWARNCPT, 1);
			FsbusWrite(MWARNFO, 1);}
			//Vol(CHIME_LOOP,70);
			//Play(CHIME_LOOP);}
		else if (val | WARN){
			FsbusWrite(MWARNCPT, 0);
			FsbusWrite(MWARNFO, 0);
			//Stop(CHIME_LOOP);}
		break;

	case AUTOLANDFAIL:
		if (val & AUTOLAND){
			FsbusWrite(AUTOLANDCPT, 1);
			FsbusWrite(AUTOLANDFO, 1);}
		else if (val | AUTOLAND){
			FsbusWrite(AUTOLANDCPT, 0);
			FsbusWrite(AUTOLANDFO, 0);}
	break;

	case MWARNCPT_SWITCH:
		if (val==0){
			FsWriteInt(WARNCAUTFIRE_SWITCH, 0x04);}
	break;

	case MCAUTCPT_SWITCH:
		if (val==0){
	 	FsWriteInt(WARNCAUTFIRE_SWITCH, 0x01);}
	break;

	case PRIORLEFT_SWITCH:
		if (val==0 && prior_left==0){
		FsWriteInt(PRIORITY, PRIOR11);
		prior_left=1;}
		else if (val==0 && prior_left==1){
			FsWriteInt(PRIORITY, NULL);
			prior_left=0;}
	break;
	
	case PRIORITY:
		if (val & PRIOR11){
			FsbusWrite(CAPT, 1);}
		else if (val | PRIOR11){
			FsbusWrite(CAPT, 0);}
	break;

	case CHRONO_CPT:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 62);}
	break;

	case CHRONO_FO:
		if (val==0){
			FsWriteInt(GLASS_ND_MODE, 162);}
	break;

//-------AUTOTHROTTLE_DISCONNECT_BUTTON_POWER_PLANT

	case ATHR_DISCONNECT:
		if (val==0){
			FsWriteInt(PM_FCU_2, 20);}
	break;
}
}

//void Warninglights()
//{
//	while(1){
//	    FsbusWrite(MWARNCPT, 1);
//		FsbusWrite(MWARNFO, 1);	
//		SetTimer(NULL,1,1000,(TIMERPROC) NULL);
//		FsbusWrite(MWARNCPT, 0);
//		FsbusWrite(MWARNFO, 0);	
//		Sleep(1000);
//		}
}