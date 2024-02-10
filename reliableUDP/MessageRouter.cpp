#include "TransferProtocol.h"
#include <string.h>
#include <stdlib.h>



#define PREFIX_MATCH 0

bool parseHeader(unsigned char* headerBuffer, unsigned char** headerItems);

//review packet contents
void reviewPacketContents(unsigned char* packetReceived, ClientState* clientState, ServerState* serverState, FileInfo* fileInfo, Mode mode)
{
	unsigned char headerBuffer[MAX_HEADER_CHARS] = { 0 };
	unsigned char* lastHeaderElemPtr = NULL; //used to mark the end of the header

	unsigned char *headerItems[MAX_HEADER_ITEMS]; //allocate memory
	for (int i = 0; i < MAX_HEADER_ITEMS; i++)
	{
		headerItems[i] = (unsigned char*)malloc(MAX_FILE_NAME_CHARS + 1);
		if (headerItems[i] != NULL && i < MAX_HEADER_ITEMS)
		{
			memset(headerItems[i], '\0', MAX_HEADER_CHARS);
		}
		
	}

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

	parseHeader(headerBuffer, headerItems);

	if (mode == Client)
	{
		if (memcmp(headerBuffer, ACK_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse acknowledgement of transfer request
			clientState->ackReceived = true;
		}
		else if (memcmp(headerBuffer, CONF_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse transfer confirmation message
			clientState->confirmationReceived = true;
		}
		else
		{
			clientState->errorState = true; //if there's no match, message was not consistent with protocol
		}

	}
	if (mode == Server)
	{
		if (memcmp(headerBuffer, REQ_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse file transfer request message
			serverState->requestReceived = true;
		}
		else if (memcmp(headerBuffer, DATA_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse data package request message
			serverState->receivingData = true;
		}
		else if (memcmp(headerBuffer, HASH_PREFIX, PREFIX_CHARS) == PREFIX_MATCH)
		{
			//parse hash packet message

			serverState->hashReceived = true;
		}
		else
		{
			serverState->errorState = true; //if there's no match, message was not consistent with protocol
		}
	}
	for (int i = 0; i < MAX_HEADER_ITEMS; i++) //free dynamically allocated memory before exiting
	{
		free(headerItems[i]);
	}


	return;
}

//takes the buffer where the header is and the 2d array where the parsed items will be stored
bool parseHeader(unsigned char* headerBuffer, unsigned char** headerItems)
{
	unsigned char* scanPtr = headerBuffer;
	for (int i = 0; i < MAX_HEADER_ITEMS; i++)
	{
		unsigned char* printPtr = headerItems[i]; //set print pointer to first element of the ith array
		while (*scanPtr != HEADER_DELIM && *scanPtr != '\0')
		{
			*printPtr = *scanPtr;
			printPtr++;
			scanPtr++;
		}
		*printPtr = '\0'; //set last element of string to null pointer

		if (*scanPtr == '\0')
		{
			i = MAX_HEADER_ITEMS; //if end of header buffer is reached, set above limit to break out of loop
		}
	}
	return true;
}



