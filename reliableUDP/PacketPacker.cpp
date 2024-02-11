// Filename: PacketPacker.cpp
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: functions used to pack the packet

#include "TransferProtocol.h"
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


//
// FUNCTION    : assemblePacket
// DESCRIPTION : Assembles a packet based on the client or server state.
// PARAMETERS  :
//      unsigned char* packetToSend : Pointer to the buffer where the assembled packet will be stored.
//      ClientState* clientState : Pointer to the client state.
//      ServerState* serverState : Pointer to the server state.
//      FileInfo* fileInfo : Pointer to the file information.
//      Mode mode : The mode of operation (Client or Server).
// RETURNS     : None
//
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




//
// FUNCTION    : createRequestPacket
// DESCRIPTION : Creates an initial request packet.
// PARAMETERS  :
//      unsigned char* packet : Pointer to the packet buffer where the request packet will be stored.
//      unsigned char* filename : Pointer to the filename buffer.
// RETURNS     : bool - True if the request packet is successfully created, false otherwise.
//
bool createRequestPacket(unsigned char* packet, unsigned char* filename)
{
	return true;
}



//
// FUNCTION    : createDataPacket
// DESCRIPTION : Creates a data packet.
// PARAMETERS  :
//      unsigned char* packet : Pointer to the packet buffer where the data packet will be stored.
//      unsigned char* transferID : Pointer to the transfer ID buffer.
// RETURNS     : int - -1 on failure, 0 on success, and 1 on last packet.
//
int createDataPacket(unsigned char* packet, unsigned char* transferID)
{
	int packetStatus = 0;
	
	return packetStatus;
}



//
// FUNCTION    : createHashPacket
// DESCRIPTION : Creates a hash packet.
// PARAMETERS  :
//      unsigned char* packet : Pointer to the packet buffer where the hash packet will be stored.
//      unsigned char* filename : Pointer to the filename buffer.
// RETURNS     : bool - True if the hash packet is successfully created, false otherwise.
//
bool createHashPacket(unsigned char* packet, unsigned char* filename)
{
	//generate hash

	//assemble packet

	return true;
}



//
// FUNCTION    : createAckResponsePacket
// DESCRIPTION : Creates an acknowledgment response packet.
// PARAMETERS  :
//      unsigned char* packet : Pointer to the packet buffer where the acknowledgment response packet will be stored.
//      unsigned char* transferID : Pointer to the transfer ID buffer.
// RETURNS     : bool - True if the acknowledgment response packet is successfully created, false otherwise.
//
bool createAckResponsePacket(unsigned char* packet, unsigned char* transferID)
{

	return true;
}



//
// FUNCTION    : createTransferConfirmationPacket
// DESCRIPTION : Creates a transfer confirmation packet.
// PARAMETERS  :
//      unsigned char* packet : Pointer to the packet buffer where the transfer confirmation packet will be stored.
//      unsigned char* transferSpeed : Pointer to the transfer speed buffer.
// RETURNS     : bool - True if the transfer confirmation packet is successfully created, false otherwise.
//
bool createTransferConfirmationPacket(unsigned char* packet, unsigned char* transferSpeed)
{
	return true;
}



//
// FUNCTION    : createErrorMessagePacket
// DESCRIPTION : Creates an error message packet.
// PARAMETERS  :
//      unsigned char* packet : Pointer to the packet buffer where the error message packet will be stored.
// RETURNS     : void
//
void createErrorMessagePacket(unsigned char* packet)
{
	return;
}