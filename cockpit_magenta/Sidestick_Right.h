#ifndef __SIDESTICK_RIGHT_H
#define __SIDESTICK_RIGHT_H


void BuildSidestick_Right_Objects();
void eventhandlerSidestickRight(int oid, int val, double dval);

enum sidestick_right {
	TILLER_FO=(GROUP_SIDESTICK_RIGHT * OBJECTS_PER_GROUP),
	TILLER_FS
};

#endif