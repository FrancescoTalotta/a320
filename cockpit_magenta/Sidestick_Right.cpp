#include "stdafx.h"


static CALTAB tiller[] =
{
	{ 148,     16384 },
    { 182,	   0     },
    { 187,	   0     },
    { 210,	  -16384 }
};


void BuildSidestick_Right_Objects() {
	//----FSUBS OBJECTS-------//
	MkFsbusObject(BTP_A_IN, TILLER_FO, "", eventhandlerSidestickRight, 11, 73, 0);

	//----FS OBJECTS----------//
	MkFsObject(TILLER_FS, "", eventhandlerSidestickRight, 0x0BBA, 2, TP_I16, FS_NORMAL, 0);
}

void eventhandlerSidestickRight(int oid, int val, double dval)
{
	switch (oid)
	{

	case TILLER_FO: 
		int fo_tiller_cal = Calibrate(val, tiller, 4);
		FsWriteInt(TILLER_FS, fo_tiller_cal);
		break;
    }
}

