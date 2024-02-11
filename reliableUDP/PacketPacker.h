#pragma once

#include <string.h>

#define DATAPACKET_SUCCESS 0
#define DATAPACKET_FAILURE -1
#define LAST_PACKET 1

#pragma warning(suppress : 4996) //for fopen

bool createRequestPacket(unsigned char* packet, unsigned char* filename);
int createDataPacket(unsigned char* packet, unsigned char* transferID);
bool createHashPacket(unsigned char* packet, unsigned char* filename);
bool createAckResponsePacket(unsigned char* packet, unsigned char* transferID);
bool createTransferConfirmationPacket(unsigned char* packet, unsigned char* transferSpeed);
void createErrorMessagePacket(unsigned char* packet);


//create initial request packet
bool createRequestPacket(unsigned char* packet, unsigned char* filename)
{
	return true;
}

//bool createDataPacket
//returns -1 on failure, 0 on success, and 1 on last packet
int createDataPacket(unsigned char* packet, unsigned char* transferID)
{
	int packetStatus = 0;

	return packetStatus;
}


bool createHashPacket(unsigned char* packet, unsigned char* filename)
{
	//generate hash

	//assemble packet

	return true;
}

bool createAckResponsePacket(unsigned char* packet, unsigned char* transferID)
{

	return true;
}

bool createTransferConfirmationPacket(unsigned char* packet, unsigned char* transferSpeed)
{
	return true;
}

void createErrorMessagePacket(unsigned char* packet)
{
	return;
}
