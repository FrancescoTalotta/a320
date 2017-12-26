#include "stdafx.h"

static int gearlevelpos,airbone,gl,gc,gr,autobrake,brakefan,apuon,pressacc,leftbrakepress,rightbrakepress,speedgauge=0;

int rfuelpump=0;

static CALTAB table[] =
		{ 
			{32,   164},
			{3000,	 0}
	};

static CALTAB tablebrakeleft[] =
		{ 
			{0,   250},
			{2000,	48}
	};

static CALTAB tablebrakright[] =
		{ 
			{0,   0},
			{2000,	198}
	};

static CALTAB tablespeedgauge2[] =
		{ 
			{0,   0},
			{60,	16}
	};

static CALTAB tablespeedgauge[] =
		{ 
			{60,   16},
			{170,	255}
	};



#define NWSTRGOFF 0x04
#define NWSTRGON 0x01
#define HOTBRAKE_LIGHT 0x02
#define	AUTOBRAKE_MIN 0x01
#define	AUTOBRAKE_MED 0x02
#define	AUTOBRAKE_MAX 0x04

void BuildMainPanelObjects () {
	MkFsbusObject (BTP_D_OUT, L_GEAR_UNLK, "", eventhandlermain, 3, 32, 0);
    MkFsbusObject (BTP_D_OUT, C_GEAR_UNLK, "", eventhandlermain, 3, 34, 0);
	MkFsbusObject (BTP_D_OUT, R_GEAR_UNLK, "", eventhandlermain, 3, 37, 0);
	MkFsbusObject (BTP_D_OUT, L_GEAR_DOWN, "", eventhandlermain, 3, 33, 0);
    MkFsbusObject (BTP_D_OUT, C_GEAR_DOWN, "", eventhandlermain, 3, 35, 0);
	MkFsbusObject (BTP_D_OUT, R_GEAR_DOWN, "", eventhandlermain, 3, 36, 0);
	MkFsbusObject (BTP_D_OUT, BRAKELOOUT, "", eventhandlermain, 3, 44, 0);
	MkFsbusObject (BTP_D_OUT, BRAKELODECEL, "", eventhandlermain, 3, 45, 0);
	MkFsbusObject (BTP_D_OUT, BRAKEMEDDECEL, "", eventhandlermain, 3, 43, 0);
	MkFsbusObject (BTP_D_OUT, BRAKEMAXDECEL, "", eventhandlermain, 3, 40, 0);
	MkFsbusObject (BTP_D_OUT, BRAKEMEDOUT, "", eventhandlermain, 3, 42, 0);
	MkFsbusObject (BTP_D_OUT, BRAKEMAXOUT, "", eventhandlermain, 3, 41, 0);
	MkFsbusObject (BTP_D_OUT, BRAKEFANHOT, "", eventhandlermain, 3, 39, 0);
	MkFsbusObject (BTP_D_OUT, BRAKEHOTON, "", eventhandlermain, 3, 38, 0);
	MkFsbusObject (BTP_D_IN, GEARLEVELUP, "", eventhandlermain, 4, 15, 0);
	MkFsbusObject (BTP_D_IN, GEARLEVERDOWN, "", eventhandlermain, 4, 14, 0);
	MkFsbusObject (BTP_D_IN, BRAKELO, "", eventhandlermain, 4, 12, 0);
	MkFsbusObject (BTP_D_IN, BRAKEMED, "", eventhandlermain, 4, 11, 0);
	MkFsbusObject (BTP_D_IN, BRAKEMAX, "", eventhandlermain, 4, 10, 0);
	MkFsbusObject (BTP_D_IN, ANTISKIDONOFF, "", eventhandlermain, 4, 13, 0);
	MkFsbusObject (BTP_D_IN, BRAKEFAN_SWITCH, "", eventhandlermain, 4, 8, 0);
	MkFsbusObject (BTP_A_OUT, BRAKERIGHT_SERVO, "", eventhandlermain, 27, 80, 0);
	MkFsbusObject (BTP_A_OUT, BRAKECENTER_SERVO, "", eventhandlermain, 27, 82, 0);
	MkFsbusObject (BTP_A_OUT, BRAKELEFT_SERVO, "", eventhandlermain, 27, 81, 0);
	MkFsbusObject (BTP_A_OUT, SPEED_GAUGE_SERVO, "", eventhandlermain, 27, 83, 0);
	MkFsbusObject (BTP_D_OUT, INT_LIGHTS, "", eventhandlermain, 3, 14, 0);

	MkFsObject (BRAKEFAN, "", eventhandlermain, 0x5761, 2, TP_I16, FS_NORMAL, 0);
	MkFsObject (GEAR_STAT, "", eventhandlermain, 0x0BE8, 4, TP_I32, FS_NORMAL, 0);
	MkFsObject (L_GEAR_STAT, "", eventhandlermain, 0x0BF4, 4, TP_I32, FS_NORMAL, 0);
	MkFsObject (C_GEAR_STAT, "", eventhandlermain, 0x0BEC, 4, TP_I32, FS_NORMAL, 0);
	MkFsObject (R_GEAR_STAT, "", eventhandlermain, 0x0BF0, 4, TP_I32, FS_NORMAL, 0);
	MkFsObject (AUTOBRAKE, "", eventhandlermain, 0x5733, 1, TP_I8, FS_NORMAL, 0);
	MkFsObject (AUTOBRAKE_DECEL, "", eventhandlermain, 0x5734, 1, TP_I8, FS_NORMAL, 0);
	MkFsObject (ANTISKID, "", eventhandlermain, 0x5722, 1, TP_I8, FS_NORMAL, 0);
	MkFsObject (HOTBRAKE, "", eventhandlermain, 0x575F, 1, TP_I8, FS_NORMAL, 0);
	MkFsObject (ACCUPRESS, "", eventhandlermain, 0x5740, 2, TP_I16, FS_QUICK, 0);
	MkFsObject (BRAKEPCI1, "", eventhandlermain, 0x5742, 2, TP_I16, FS_QUICK, 0);
	MkFsObject (BRAKEPCI2, "", eventhandlermain, 0x5744, 2, TP_I16, FS_QUICK, 0);
    MkFsObject(FUEL_PUMPS, "", eventhandlermain, 0x56B1, 1,  TP_UI8,  FS_NORMAL, 0); 
	MkFsObject(FS_SPEED_IAS, "", eventhandlermain, 0x02BC, 4,  TP_UI32,  FS_QUICK, 0); 
	//MkFsObject(APUON2, "", eventhandlermain, 0x5600, 1, TP_I8, FS_NORMAL, 0);

	//Poweron BUS objects

	FsbusPowerOffOptions(L_GEAR_DOWN, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(C_GEAR_DOWN, PWRTYPE_MAINBUS, 0);
	FsbusPowerOffOptions(R_GEAR_DOWN, PWRTYPE_MAINBUS, 0);
	
}


void  SyncroMainpanel() {
	FsReadDirect(0x0BF4,4,&gl);
	FsReadDirect(0x0BEC,4,&gc);
	FsReadDirect(0x0BF0,4,&gr);
	FsReadDirect(0x5600,1,&apuon);
	FsReadDirect(0x5740,2,&pressacc);

	//if (apuon==6){
		if (gl==16383){
			FsbusWrite(L_GEAR_DOWN, 1);}

		if (gc==16383){
			FsbusWrite(C_GEAR_DOWN, 1);}

		if (gr==16383){
			FsbusWrite(R_GEAR_DOWN, 1);}

		FsbusWriteFmt3(27, 81, 164);
		//FsbusWrite(BRAKECENTER_SERVO, pressacc);
	}
//}

void eventhandlermain (int oid, int val, double dval)
{
	switch (oid)
	{

	case GEARLEVELUP:
		if (val==0){
			FsWriteInt(GEAR_STAT, 16383);}
	break;
	
	case GEARLEVERDOWN:
		if (val==0){
			FsWriteInt(GEAR_STAT, 0);
			FsWriteInt(AUTOBRAKE, 0	);
		}
	break;

	case L_GEAR_STAT:
		FsReadDirect(0x0BE8,4,&gearlevelpos);
		FsReadDirect(0x0366,2,&airbone);
		if (airbone==0){
			if (val>0 && val<16383){
				FsbusWrite(L_GEAR_UNLK, 1);
			}
			else if (val==0 || val==16383){
				FsbusWrite(L_GEAR_UNLK, 0);}
			if (val>13000 && val<=16383){
				FsbusWrite(L_GEAR_DOWN, 1);}
			else if (val>=0 && val<14000){
				FsbusWrite(L_GEAR_DOWN, 0);}
		}
	break;

	case C_GEAR_STAT:
		FsReadDirect(0x0BE8,4,&gearlevelpos);
		FsReadDirect(0x0366,2,&airbone);
		if (airbone==0){
			if (val>0 && val<16383){
				FsbusWrite(C_GEAR_UNLK, 1);
			}
			else if (val==0 || val==16383){
				FsbusWrite(C_GEAR_UNLK, 0);}
			if (val>13000 && val<=16383){
				FsbusWrite(C_GEAR_DOWN, 1);}
			else if (val>=0 && val<14000){
				FsbusWrite(C_GEAR_DOWN, 0);}
		}
	break;

	case R_GEAR_STAT:
		FsReadDirect(0x0BE8,4,&gearlevelpos);
		FsReadDirect(0x0366,2,&airbone);
		if (airbone==0){
			if (val>0 && val<16383){
				FsbusWrite(R_GEAR_UNLK, 1);
			}
			else if (val==0 || val==16383){
				FsbusWrite(R_GEAR_UNLK, 0);}
			if (val>13000 && val<=16383){
				FsbusWrite(R_GEAR_DOWN, 1);}
			else if (val>=0 && val<14000){
				FsbusWrite(R_GEAR_DOWN, 0);}
		}
	break;

	case BRAKELO:
		if (val==0 && autobrake!=1){
			FsWriteInt(AUTOBRAKE, 1);
			autobrake=1;
		}
		else if (val==0 && autobrake==1){
			FsWriteInt(AUTOBRAKE, 0);
			autobrake=0;}
		break;

	case BRAKEMED:
		if (val==0 && autobrake!=2){
			FsWriteInt(AUTOBRAKE, 2);
			autobrake=2;
		}
		else if (val==0 && autobrake==2){
			FsWriteInt(AUTOBRAKE, 0);
			autobrake=0;}
		break;

	case BRAKEMAX:
		if (val==0 && autobrake!=3){
			FsWriteInt(AUTOBRAKE, 3);
			autobrake=3;
		}
		else if (val==0 && autobrake==3){
			FsWriteInt(AUTOBRAKE, 0);
			autobrake=0;}
		break;

	case AUTOBRAKE:
			if (val==1){
			FsbusWrite(BRAKELOOUT, 1);
			FsbusWrite(BRAKEMEDOUT, 0);	
			FsbusWrite(BRAKEMAXOUT, 0);
		}
		else if (val==2){
			FsbusWrite(BRAKEMEDOUT, 1);
			FsbusWrite(BRAKELOOUT, 0);
			FsbusWrite(BRAKEMAXOUT, 0);
		}
		else if (val==3){
			FsbusWrite(BRAKEMAXOUT, 1);
			FsbusWrite(BRAKELOOUT, 0);
			FsbusWrite(BRAKEMEDOUT, 0);
		}
		else if (val==0){
			FsbusWrite(BRAKEMAXOUT, 0);
			FsbusWrite(BRAKELOOUT, 0);
			FsbusWrite(BRAKEMEDOUT, 0);}
		break;
	

	case ANTISKIDONOFF:
		if (val==0){
			FsWriteInt(ANTISKID, NWSTRGOFF);}
		else if (val==1){
			FsWriteInt(ANTISKID, NWSTRGON);}	
	break;
	
	case BRAKEFAN_SWITCH:
		if (val==0 && brakefan==0){
			FsWriteInt(BRAKEFAN, 1);
			brakefan=1;}
		else if (val==0 && brakefan==1){
			FsWriteInt(BRAKEFAN, 0);
			brakefan=0;}
	break;

	case AUTOBRAKE_DECEL:
		if (val & AUTOBRAKE_MIN){
			FsbusWrite(BRAKELODECEL, 1);}
		else if (val | AUTOBRAKE_MIN){
			FsbusWrite(BRAKELODECEL, 0);
		}

		if (val & AUTOBRAKE_MED){
			FsbusWrite(BRAKEMEDDECEL, 1);}
		else if (val | AUTOBRAKE_MED){
			FsbusWrite(BRAKEMEDDECEL, 0);
		}

		if (val & AUTOBRAKE_MAX){
			FsbusWrite(BRAKEMAXDECEL, 1);}
		else if (val | AUTOBRAKE_MAX){
			FsbusWrite(BRAKEMAXDECEL, 0);
		}
	break;

	case BRAKEFAN:
		if (val==1)
			FsbusWrite(BRAKEHOTON,1);
		else if (val==0)
			FsbusWrite(BRAKEHOTON,0);
	break;

	case HOTBRAKE:
			if(val & HOTBRAKE_LIGHT)
		FsbusWrite(BRAKEFANHOT,1);
	else if (val | HOTBRAKE_LIGHT)
		FsbusWrite(BRAKEFANHOT,0);
	break;

	//case FUEL_PUMPS:
	//	if (val & 0x02){
			rfuelpump=1;
	//		}
	break;

	case FS_SPEED_IAS:
		//printf("accu press is %d",val/128);
		val=val/128;
		if(val<60){
			speedgauge=Calibrate(val, tablespeedgauge2, 2);}
		else{
			speedgauge=Calibrate(val, tablespeedgauge, 2);}
		FsbusWrite(SPEED_GAUGE_SERVO, speedgauge);
	break;

	case BRAKEPCI1:
		//printf("accu press is %d",val);
		leftbrakepress=Calibrate(val, tablebrakeleft, 2);
		FsbusWrite(BRAKELEFT_SERVO, leftbrakepress);
	break;

	case BRAKEPCI2:
		//printf("accu press is %d",val);
		rightbrakepress=Calibrate(val, tablebrakright, 2);
		FsbusWrite(BRAKERIGHT_SERVO, rightbrakepress);
	break;

	case ACCUPRESS:
		int AccCalibriated=Calibrate(val, table, 2);
		FsbusWrite(BRAKECENTER_SERVO, AccCalibriated);
	break;



}

}
