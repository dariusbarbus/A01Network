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
		//create transfer request
		if (!clientState->requestSent)
		{
			if (!createRequestPacket(packetToSend, fileInfo->fileName, fileInfo->fileSize, fileInfo->fileType))
			{
				//create error packet
				return;
			}
			clientState->requestSent == true;
			return;
		}

		//send first data packet
		if (clientState->ackReceived && !clientState->sendingData) //if request was acknowledged and not sending data
		{
			//initialize data sending process
			//send first packet
			clientState->sendingData == true;
			return;
		}

		//continue sending data
		if (clientState->sendingData)
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
		if (clientState->lastPacketSent) //send final packet with hash after last packet is sent
		{
			if (!createHashPacket(packetToSend, fileInfo->fileName, fileInfo->fileSize))
			{
				//create error packet
				clientState->errorState == true;
			}
			return;
		}
	}
	if (mode == Server)
	{
		//send request response



		//send response



	}
}

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