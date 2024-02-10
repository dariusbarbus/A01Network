//Packs packets

#include "TransferProtocol.h"

#define DATAPACKET_SUCCESS 0
#define DATAPACKET_FAILURE -1
#define LAST_PACKET 1


bool createRequestPacket(unsigned char* packet, unsigned char* filename, unsigned char* fileSize, int fileType);
int createDataPacket(unsigned char* packet, unsigned char* transferID);
bool createHashPacket(unsigned char* packet, unsigned char* filename, unsigned char* filesize);
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
		if (clientState->hashSent == true)
		{
			return;
		}
		//create transfer request
		if (clientState->requestSent == false)
		{
			if (!createRequestPacket(packetToSend, fileInfo->fileName, fileInfo->fileSize, fileInfo->fileType))
			{
				clientState->errorState == true; //error packet will be created next send loop
				return;
			}
			clientState->requestSent == true;
			return;
		}
		//if request was acknowledged and not sending data, send first data packet
		if (clientState->ackReceived == true && clientState->sendingData == false) 
		{
			//initialize data sending process
			//send first packet
			clientState->sendingData == true;
			return;
		}
		//if last packet hasn't been sent, continue sending data
		if (clientState->sendingData == true && clientState->lastPacketSent == false)
		{
			int packetStatus = createDataPacket(packetToSend, fileInfo->transferID);
			if (packetStatus == DATAPACKET_FAILURE)
			{
				clientState->errorState == true; //error packet will be created next send loop
			}
			else if (packetStatus == LAST_PACKET) //flip lastPacketSent based on size of packet (i.e. less than 210 bytes)
			{
				clientState->lastPacketSent == true;
			}
			return;
		}
		//if last packet has been sent, send hash
		if (clientState->lastPacketSent == true && clientState->hashSent == false) 
		{
			if (!createHashPacket(packetToSend, fileInfo->fileName, fileInfo->fileSize))
			{
				clientState->errorState == true; //error packet will be created next send loop
			}
			clientState->hashSent == true;
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
			serverState->requestAckSent == true;
			return;
		}
		if (serverState->hashReceived == true && serverState->confirmationSent == false)
		{
			//send confirmation
			createTransferConfirmationPacket(packetToSend, fileInfo->transferSpeed);
			serverState->confirmationSent == true;
			return;
		}
	}
}


//create initial request packet
bool createRequestPacket(unsigned char* packet, unsigned char* filename, unsigned char* fileSize, int fileType)
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


bool createHashPacket(unsigned char* packet, unsigned char* filename, unsigned char* filesize)
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