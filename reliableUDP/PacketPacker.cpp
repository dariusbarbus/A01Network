//Packs packets

#include "TransferProtocol.h"




//determine what packent needs to be sent
void assemblePacket(char* packetToSend, ClientState* clientState, ServerState* serverState, Mode mode)
{
	if (mode == Client)
	{
		//create transfer request
		if (!clientState->requestSent)
		{
			
			clientState->requestSent == true;
			return;
		}



		//send first data packet
		if (clientState->ackReceived && !clientState->sendingData) //if request was acknowledged and not sending data
		{
			//initialize data sending
			//send first packet
			clientState->sendingData == true;
			return;
		}

		//continue sending data
		if (clientState->sendingData)
		{
			//send data

			//flip lastPacketSent based on size of packet (i.e. less than 210 bytes)
			return;
		}


		//send final packet with hash



	}
	if (mode == Server)
	{
		//send request response



		//send response



	}
}

bool createRequestPacket(char* packet, char* filename, char* fileSize, char* fileType)
{
	return true;
}

//bool createDataPacket
