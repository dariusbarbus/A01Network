//Packs packets

#include "TransferProtocol.h"
#include "HashGen.h"
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

//determine what packent needs to be sent
void assemblePacket(unsigned char* packetToSend, ClientState* clientState, ServerState* serverState, FileInfo* fileInfo, Mode mode)
{
	if (mode == Client)
	{
		if (clientState->errorState == true)	//if an error state was set when receiving/parsing the last message
		{
			//send error message
			return;
		}
		//if hashSent is true
		else if (clientState->hashSent == true)
		{
			return;
		}
		//create transfer request
		else if (clientState->requestSent == false)
		{
			if (!createRequestPacket(packetToSend, fileInfo->fileName))
			{
				clientState->errorState = true; //error packet will be created next send loop
			}
			else
			{
				clientState->requestSent = true;
			}
			return;
		}
		//if request was acknowledged and not sending data, send first data packet
		if (clientState->ackReceived == true && clientState->sendingData == false) 
		{
			//initialize data sending process
			//send first packet
			clientState->sendingData = true;
			return;
		}
		//if last packet hasn't been sent, continue sending data
		if (clientState->sendingData == true && clientState->lastPacketSent == false)
		{
			int packetStatus = createDataPacket(packetToSend, fileInfo->transferID);
			if (packetStatus == DATAPACKET_FAILURE)
			{
				clientState->errorState = true; //error packet will be created next send loop
			}
			else if (packetStatus == LAST_PACKET) //flip lastPacketSent based on size of packet (i.e. less than 210 bytes)
			{
				clientState->lastPacketSent = true;
			}
			return;
		}
		//if last packet has been sent, send hash
		if (clientState->lastPacketSent == true && clientState->hashSent == false) 
		{
			if (!createHashPacket(packetToSend, fileInfo->fileName))
			{
				clientState->errorState = true; //error packet will be created next send loop
			}
			else
			{
			clientState->hashSent = true;
			}
			return;
		}
	}
	if (mode == Server)
	{

		if (serverState->errorState == true)	//if an error state was set when receiving/parsing the last message
		{
			//send error message
			return;
		}
		if (serverState->requestReceived == false || serverState->confirmationSent == true) //if request not received, or transfer is done, keep waiting
		{
			return;
		}
		if (serverState->requestReceived == true && serverState->requestAckSent == false) 
		{
			//send request ack
			createAckResponsePacket(packetToSend, fileInfo->transferID);
			serverState->requestAckSent = true;
			return;
		}
		if (serverState->hashReceived == true && serverState->confirmationSent == false)
		{
			//send confirmation
			createTransferConfirmationPacket(packetToSend, fileInfo->transferSpeed);
			serverState->confirmationSent = true;
			return;
		}
	}
}


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



	HashGen hashGen;
	hashGen.hashValue((char*)filename);



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