#include "stdafx.h"

#define STARTOFDATA	5
#define BLOCKCOUNT 8
#define	ZERO 0x01
#define	ONE 0x02
#define	TWO 0x04
#define	TREE 0x08
#define	FOUR 0x10
#define	FIVE 0x20
#define	SIX 0x40
#define	EIGHT 0x80
#define	NINE 0x200
#define	TEN 0x800

extern int rfuelpump;

int iIndex;
float fValueBlock[BLOCKCOUNT]; 
unsigned char strDatabuffer[2100];


void EventHandlerUdp(FSUDP *UdpDatagram) 
{ 


	if (UdpDatagram->rcount > 5 && UdpDatagram->rcount < sizeof(UdpDatagram->rbuf)) 
	{
		if (memcmp(UdpDatagram->rbuf, "DATA", 4) == 0) 
		{ 
			int iCompoundPacketCount, iPacketIndex, iDataCursor, i; 
			float fPayload[BLOCKCOUNT]; 
			iCompoundPacketCount = (UdpDatagram->rcount - STARTOFDATA) / 36;
			//printf("\nTotal datagram size: %d, number of compound packets: %d\n", UdpDatagram->rcount, iCompoundPacketCount); 
			iDataCursor=STARTOFDATA;
			iPacketIndex=0;

			//for(i = 0; i < iCompoundPacketCount; i++) 
			//{  
				//int iValueBlock = 0;  
				/* Reads the X-Plane data index number */  
				//memcpy(&iPacketIndex, &UdpDatagram->rbuf[iDataCursor], 4); 
				/* Print index number to console */  
				//printf("\nData packet index: %d\n", iPacketIndex);  
				//printf("--------------------------\n");  
				/* Move the cursor to start of value blocks */  
				//iDataCursor += 4;    
				/* Read all the values and store them in an array */  
				//memcpy(fPayload, &UdpDatagram->rbuf[iDataCursor], 32);  
				/* Print all values in the array to console */  
				//while(iValueBlock < BLOCKCOUNT)  
				//{   
				//	printf("Value block %d: %f\n", iValueBlock, fPayload[iValueBlock]);   
				//	iValueBlock++;  
				//}
				 
				/* Move the cursor to start of the next data packet */  
				rfuelpump=1;

				//iDataCursor += 32; 

			//	if (iPacketIndex==55){
//					rfuelpump=fPayload[1];
	//				if (rfuelpump==1){
		//				FsWriteDirect(0x56B1,1,&rfuelpump);}}
				//	lfuel=fPayload[2]*230*24;
				//	FsWriteDirect(0x0B94,4,&lfuel);
				//}
					//printf("value of center fuel %d", cfuel);}
					

		//}
	}
}

		//if (rfuelpump==1){
			//iIndex=55;
			//memcpy(&strDatabuffer[STARTOFDATA], &iIndex, 4); 
			//fValueBlock[0]=1.000000;
			//memcpy(&strDatabuffer[STARTOFDATA + 4], &fValueBlock, 32); 
			//rfuelpump=0;

		//}

}
