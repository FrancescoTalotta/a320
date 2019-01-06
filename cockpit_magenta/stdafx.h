// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#ifndef __STDAFX_H
#define __STDAFX_H



// Load program headers
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlobj.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <iostream>
#include <thread>
#include "conio.h"
#include <ctime>
#include <Windows.h>


// Load FSBUS header
#include "fsbus.h"

// Global constants
#define  OBJECT_GROUPS      16                              // Number of control groups
#define  OBJECTS_PER_GROUP  (4096 / OBJECT_GROUPS)          // Calculate maximum objects per group


// Global variables
extern  bool  bSynchronised;                                // Cockpit executable <-> Flight Simulator synchronisation flag


// Global method prototypes
void EventHandler(int ObjectID, int Value, double DoubleValue);


enum  _object_groups 

{ 

	GROUP_NULL,                         // group 0 not used!

	MAIN_GROUP_OID,                     // 1

	FCU_GROUP_OID,                      // 2

	//GROUP_AFTOVERHEAD,                // 3

	//GROUP_FWDOVERHEAD,                // 4

	GROUP_PEDESTAL,                     // 5

	GROUP_SOUND,                        // 6

	//GROUP_THROTTLEQUADRANT,           // 7

	//GROUP_FLIGHTSELECTOR				// 8

	GROUP_SIDESTICK_RIGHT               // 9

};

#include "glareshield.h"
#include "mainpanel.h"
#include "pedestal.h"
#include "Sidestick_Right.h"
#include "sound.h"
#include "xplane.h"





#endif