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



		//send data packet




		//send final packet with has
	}
	if (mode == Server)
	{
		//send request response



		//send response



	}
}

bool createRequestPacket(char* packet, char* filename, char* fileSize, char* fileType)
{

}

//bool createDataPacket
