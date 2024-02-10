#include "TransferProtocol.h"
#include <string.h>



#define PREFIX_MATCH 0

//review packet contents
void reviewPacketContents(unsigned char* packetReceived, ClientState* clientState, ServerState* serverState, FileInfo* fileInfo, Mode mode)
{
	unsigned char headerBuffer[MAX_HEADER_CHARS] = { 0 };
	unsigned char* lastHeaderElemPtr = NULL; //used to mark the end of the header

	//extract header from packet
	memcpy(headerBuffer, packetReceived, MAX_HEADER_CHARS); //copy packet to header buffer
	//find the ':' delimeter marking the end of the header and replace with null char to facilitate parsing
	for (int i = 0; i < MAX_HEADER_CHARS; i++)
	{
		if (headerBuffer[i] == HEADER_END)
		{
			headerBuffer[i] = '\0';
			lastHeaderElemPtr = &headerBuffer[i]; //set ptr at location of end of header (used when extracting data payload from packet)
			break;
		}
	}

	if (mode == Client)
	{
		if (memcmp(headerBuffer, ACK_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse acknowledgement of transfer request
			clientState->ackReceived == true;
			return;
		}
		else if (memcmp(headerBuffer, CONF_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse transfer confirmation message
			clientState->confirmationReceived == true;
			return;
		}
		else
		{
			//if there's no match, send error message?
			clientState->errorState == true;
			return;
		}

	}
	if (mode == Server)
	{
		if (memcmp(headerBuffer, REQ_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse file transfer request message
			serverState->requestReceived == true;
			return;
		}
		else if (memcmp(headerBuffer, DATA_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse data package request message
			serverState->receivingData == true;
			return;
		}
		else if (memcmp(headerBuffer, HASH_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse hash packet message

			serverState->hashReceived == true;
		}
		else
		{
			serverState->errorState == true;
			return;
		}
	}
}



