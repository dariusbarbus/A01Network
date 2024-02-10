//Packs packets

#include "TransferProtocol.h"

#define DATAPACKET_SUCCESS 0
#define DATAPACKET_FAILURE -1
#define LAST_PACKET 1


bool createRequestPacket(char* packet, char* filename, char* fileSize, int fileType);
int createDataPacket(char* packet, char* transferID);
bool createHashPacket(char* packet, char* filename, char* filesize);

//determine what packent needs to be sent
void assemblePacket(char* packetToSend, ClientState* clientState, ServerState* serverState, FileInfo* fileInfo, Mode mode)
{
	if (mode == Client)
	{
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
				//create error packet
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
				//create error packet
				clientState->errorState == true;
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
				//create error packet
				clientState->errorState == true;
			}
			clientState->hashSent == true;
			return;
		}
	}
	if (mode == Server)
	{
		if (serverState->requestReceived == false || serverState->confirmationSent == true) //if request not received, or transfer is done, keep waiting
		{
			return;
		}
		if (serverState->requestReceived == true && serverState->requestAckSent == false) 
		{
			//send request ack
			serverState->requestAckSent == true;
			return;
		}
		if (serverState->hashReceived == true && serverState->confirmationSent == false)
		{
			//send confirmation
			serverState->confirmationSent == true;
			return;
		}
	}
}


//create initial request packet
bool createRequestPacket(char* packet, char* filename, char* fileSize, int fileType)
{
	return true;
}

//bool createDataPacket
//returns -1 on failure, 0 on success, and 1 on last packet
int createDataPacket(char* packet, char* transferID)
{
	int packetStatus = 0;

	return packetStatus;
}


bool createHashPacket(char* packet, char* filename, char* filesize)
{
	//generate hash

	//assemble packet

	return true;
}