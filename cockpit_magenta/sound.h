#ifndef __SOUND_H
#define __SOUND_H

extern int powerup;

// Declare method prototypes from the code file
void BuildObjectsSound();
void startsound();
void EventHandlerSound(int oid, int val, double dval);

// Enumerate objects
enum _sound
{
	// FSBUS objects
    POWERUP = (GROUP_SOUND * OBJECTS_PER_GROUP),
    COCKPIT_FANS,
	VENTPOWERON,
	APUON,
	APURISE,
	CHIME,
	CHIME_LOOP,
	AVIONICS
};


#endif