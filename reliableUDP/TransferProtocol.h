#pragma once

//Outline of protocol, client/server messages, etc.


//UNIVERSAL
//=========

#define HEADER_DELIM '|' //Delimeter used between message components
#define HEADER_END ':' //Marks end of header
#define MAX_HEADER_CHARS 256 //Max number of chars for general transfer protocol messages
#define PREFIX_CHARS 4

//Error message
#define ERROR_PREFIX "ERRR"
#define MAX_ERROR_CODE_CHARS 2 
//Error message (code)

//Sample error message
//
// ERROR|04:


//SENDER
//========

//Send Request for transfer
	//Filename
	//Filesize
	//Filetype

#define REQ_PREFIX "TREQ"
#define MAX_FILE_NAME_CHARS 100 //TBD - consider generating shortform filename ID for data packets
#define MAX_FILE_SIZE_CHARS 10 //Allows for a maximum file size of 10^9 bytes (i.e. 9.99 GB), which is large enough for our application

	//Allowable Filetypes
#define ASCII_FILE "ASCII"
#define ASCII 0
#define BINARY_FILE "BIN"
#define BINARY 1

//Sample request for transferring the following file
// Filename: myFile.jpg
// Size: 2MB
// Filetype: Binary
// 
// TRANSFER REQ|myFile.jpg|2000000|BIN:


//Data packet
//Header
#define DATA_PREFIX "DATA"
#define MAX_DATA_HEADER_BYTES 30 //Max header length for data transfer packets
#define SIZE_OF_PAYLOAD 3 //Max payload chars (10^2 bytes)
	//Transfer Identifier
	//Size of Payload (bytes)
//Payload

//Sample data packet
//
// DATA|123456|210:...210 bytes of file data goes here...



//Final packet with hash
#define HASH_PREFIX "HASH"
//Header
	//Filename
	//Filesize
#define MAX_HASH_CHARS 36 //TBD based on method chosen
//Hash

//Sample final packet
//
// FINAL|myFile.jpg|2000000|this-is-my-whole-file-hash:



//RECEIVER
//========

//Response to Request
#define ACK_PREFIX "RACK"
	//Proceed
#define START_TRANSFER "TRANSFER APPROVED"
	//Don't Proceed
#define DENY_TRANSFER "TRANSFER DENIED"
#define REQ_CODE_MAX_CHARS 1 //allows for up to 10 request codes
#define MAX_TRANSFER_ID_BYTES 5 //Assume randomly generatd uint16 ID for transfer

//Sample request acknowledgement packet
//
// REQUEST ACKED|TRANSFER APPROVED|0|123456:


//Response to final packet
#define CONF_PREFIX "CONF"
	//Comparison to hash complete - file transferred successfully
#define CONF_SUCCESS "SUCCESS"
	//File transfer unsuccessful
#define CONF_FAILURE "FAILURE"
#define CONF_CODE_MAX_CHARS 1 //allows for up to 10 confirmation codes
	//Transfer speed
#define SPEED_MAX_CHARS 5 //allows for up to 5 sig figs for transfer speed

//Sample final packet received
//
// TRANSFER STATUS|SUCCESS|0|10.534:


typedef struct ClientState {
	bool requestSent;
	bool ackReceived;
	bool sendingData;
	bool lastPacketSent;
	bool hashSent;
	bool confirmationReceived;
	bool errorState;
} ClientState;

typedef struct ServerState {
	bool requestReceived;
	bool requestAckSent;
	bool receivingData;
	bool hashReceived;
	bool confirmationSent;
	bool errorState;
} ServerState;


typedef struct FileInfo {
	unsigned char fileName[MAX_FILE_NAME_CHARS];
	unsigned char fileSize[MAX_FILE_SIZE_CHARS];
	int fileType;
	unsigned char transferID[MAX_TRANSFER_ID_BYTES];
	unsigned char wholeFileHash[MAX_HASH_CHARS];
	unsigned char transferSpeed[SPEED_MAX_CHARS];
} FileInfo;


//This exists in reliableUDP, may have to move it here if issues are encountered
enum Mode
{
	Client,
	Server
};

