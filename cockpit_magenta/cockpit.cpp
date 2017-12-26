// rvda320cockpit.cpp : Defines the entry point for the console application.
// Copyright (c) 2008..2010, Dirk Anderseck, Rob van Dijk, Rinus Groen and Rob Hamerling



#include "stdafx.h"
// Main program

using namespace std;
extern unsigned char strDatabuffer[2100];


void main()

{
	{

		// Connect to fsbus.dll and flightsim
		FSUDP *XPlaneUdp;

		CheckIn();
		FsConnect();
//--------------------------XPlane READ-------------------------//
		XPlaneUdp = MkUdpInterface(UDP_XPLANE, 49010, EventHandlerUdp); 
//--------------------------XPlane WRITE-----------------------//
		UdpDestination(XPlaneUdp, "192.168.0.2", 49001);
		UdpSend(XPlaneUdp, strDatabuffer, 41); 

		// Open the COM interface for Input/Output operations
		FsbusOpen("COM1");

		// Build all objects
		//FsbusWriteFmt3(0, 131, 0);  // battery status off all cards
		Sleep(500);
		BuildObjectsSound();
		BuildglareObjects();
		BuildMainPanelObjects();
		BuildPedestalObjects();
		FsbusMux(250);

		// Synchronise the attached hardware with flight simulator
		startsound();
		Syncroglare();
		SyncroMainpanel();
		SyncroPedestalpanel();
		SetPower(PWRTYPE_MAINBUS, 1);
		printf("press any key to exit...\r\n");
		

		while (!_kbhit())
		FsbusMux(500);

		// Close connection to the serial interface
		FsbusClose();
		// Disconnect from fsbus.dll and flight simulator 
		//FsDisconnect();
		CheckOut();	

	}
}
  
    




// Global event handler, redirects events to the appropriate group handler

void EventHandler(int ObjectID, int Value, double DoubleValue)
{
	 switch (ObjectID / OBJECTS_PER_GROUP)
    {
        case MAIN_GROUP_OID:
			{
				eventhandlermain(ObjectID, Value, DoubleValue);
			}
            break;
		case GROUP_PEDESTAL:
			{
				eventhandlermain(ObjectID, Value, DoubleValue);
			}
            break;
		case FCU_GROUP_OID:
			{
				eventhandler(ObjectID, Value, DoubleValue);
			}
            break;
		case GROUP_SOUND:
			{
				EventHandlerSound(ObjectID, Value, DoubleValue);
			}
			break;

		// Error handling
        default:
			{
            printf("%cError: Object %d (%d:%d) in unknown group.\n",
                    7, ObjectID, ObjectID / OBJECTS_PER_GROUP, ObjectID % OBJECTS_PER_GROUP);
			}
			break;
    }
}

