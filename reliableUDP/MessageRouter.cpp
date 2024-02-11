// Filename: MessageRouter.cpp
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: functions used for the protocol


#include "TransferProtocol.h"
#include <string.h>
#include <stdlib.h>



#define PREFIX_MATCH 0

bool parseHeader(unsigned char* headerBuffer, unsigned char** headerItems);



//
// FUNCTION    : reviewPacketContents
// DESCRIPTION : Reviews the contents of a packet and updates the client or server state accordingly.
// PARAMETERS  :
//      unsigned char* packetReceived : Pointer to the received packet buffer.
//      ClientState* clientState : Pointer to the client state.
//      ServerState* serverState : Pointer to the server state.
//      FileInfo* fileInfo : Pointer to the file information.
//      Mode mode : The mode of operation (Client or Server).
// RETURNS     : None
//
void reviewPacketContents(unsigned char* packetReceived, ClientState* clientState, ServerState* serverState, FileInfo* fileInfo, Mode mode)
{
	unsigned char headerBuffer[MAX_HEADER_CHARS] = { 0 };
	unsigned char* lastHeaderElemPtr = NULL; //used to mark the end of the header

	unsigned char *headerItems[MAX_HEADER_ITEMS]; //allocate memory
	for (int i = 0; i < MAX_HEADER_ITEMS; i++)
	{
		headerItems[i] = (unsigned char*)malloc(MAX_FILE_NAME_CHARS + 1);	
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



//
// FUNCTION    : parseTransferReqest
// DESCRIPTION : Parses the transfer request packet contents.
// PARAMETERS  :
//      unsigned char** headerItems : Pointer to the parsed header items.
//      FileInfo* fileinfo : Pointer to the file information.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
bool parseTransferReqest(unsigned char** headerItems, FileInfo* fileinfo)
{
	//set filename
	//set filesize
	return true;
}



//
// FUNCTION    : parseDataPackage
// DESCRIPTION : Parses the data package contents.
// PARAMETERS  :
//      unsigned char** headerItems : Pointer to the parsed header items.
//      unsigned char* packetReceived : Pointer to the received packet buffer.
//      FileInfo* fileInfo : Pointer to the file information.
//      unsigned char* lastHeaderElement : Pointer to the last header element.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
bool parseDataPackage(unsigned char** headerItems, unsigned char* packetReceived, FileInfo* fileInfo, unsigned char* lastHeaderElement)
{
	//convert payload size to int
	//write to file
	//update filesize counter

	//if not all successful
		//notify user, set error state

	return true;
}




//
// FUNCTION    : parseHash
// DESCRIPTION : Parses the hash packet contents.
// PARAMETERS  :
//      unsigned char** headerItems : Pointer to the parsed header items.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
bool parseHash(unsigned char** headerItems)
{
	//compare hash sent with calculated hash
	//if they match
		//calculate file size for confirmation packet


	return true;
}



//
// FUNCTION    : parseRequestAck
// DESCRIPTION : Parses the request acknowledgment packet contents.
// PARAMETERS  :
//      unsigned char** headerItems : Pointer to the parsed header items.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
bool parseRequestAck(unsigned char** headerItems)
{
	//if request has been approved
		//start transfer
	//else
		//notify user, shut down program
	
	return true;
}



//
// FUNCTION    : parseConfirmation
// DESCRIPTION : Parses the transfer confirmation packet contents.
// PARAMETERS  :
//      unsigned char** headerItems : Pointer to the parsed header items.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
bool parseConfirmation(unsigned char** headerItems)
{
	//If hash confirmed
		//transfer successful
	//Else
		//transfer unsuccessful

	//notify user, shut down program
	return true;
}



//
// FUNCTION    : parseError
// DESCRIPTION : Parses the error packet contents.
// PARAMETERS  :
//      unsigned char** headerItems : Pointer to the parsed header items.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
bool parseError(unsigned char** headerItems)
{
	//log error and exit program
	return true;
}


//
// FUNCTION    : parseHeader
// DESCRIPTION : Parses the header of a packet.
// PARAMETERS  :
//      unsigned char* headerBuffer : Pointer to the header buffer.
//      unsigned char** headerItems : Pointer to the parsed header items.
// RETURNS     : bool - True if parsing is successful, false otherwise.
//
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
