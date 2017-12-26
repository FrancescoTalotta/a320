// sound.cpp - Header file for Sound control
#include "stdafx.h"
#include "mmsystem.h"

static int powerup=0;

// Create FSBUS and Flight Simulator objects
void BuildObjectsSound()
{
    MkSound(POWERUP, "OID", EventHandlerSound, "Sound\\AvionicsFull2.wav");
	MkSound(VENTPOWERON, "OID", EventHandlerSound, "Sound\\AvionicsVent2.wav");
	MkSound(AVIONICS, "OID", EventHandlerSound, "Sound\\Avionics2.wav");
	MkSound(APURISE, "OID", EventHandlerSound, "Sound\\Apu_Internal_1.wav");
	//MkSound(CHIME, "OID", EventHandlerSound, "Sound\\SINGLE_CHIME.wav");
	//MkLoopSound(CHIME_LOOP, "OID", EventHandlerSound, "Sound\\CHIME.wav");
	MkFsObject(APUON, "", EventHandlerSound, 0x5600, 1, TP_I8, FS_NORMAL, 0);
}

//#define SND_FILENAME 0x20000
//#define SND_LOOP 8
//#define SND_ASYNC 1
using namespace std; 
void startsound() {

}

void EventHandlerSound(int oid, int val, double dval)
{
	//Play(POWERUP);
    switch(oid) {

	case APUON:
		val==powerup;
		//printf("apu val %d", val);
		if (val==4){
		Sleep(11*1000);
		Play(APURISE);
		Sleep(13*1000);
		PlaySound(TEXT("Sound//Apu_Internal_2.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
		}
		if (val==6){
		FsbusWrite(GLARE_LIGHTS, 1);
		FsbusWrite(INT_LIGHTS, 1);
		Play(POWERUP);
		//PlaySound(TEXT("Sound\\AvionicsFull2.wav"), NULL, SND_FILENAME|NULL|SND_ASYNC);
		FsbusWriteFmt3(0, 131, 100);
		Sleep(2*1000);	
		Play(VENTPOWERON);
		Sleep(2.94*1000);
		PlaySound(TEXT("Sound//Avionics.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
		SetPower(PWRTYPE_MAINBUS, 1);
		FsbusWriteFmt3(27, 80, 0);
		FsbusWriteFmt3(27, 82, 210);
		}
		break;
	}
}