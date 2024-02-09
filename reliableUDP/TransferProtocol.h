#pragma once

//Outline of protocol, client/server messages, etc.


//UNIVERSAL
//=========

#define HEADER_DELIM '|' //Delimeter used between message components
#define HEADER_END ':' //Marks end of header
#define MAX_MESSAGE_CHARS 255 //Max number of chars for general transfer protocol messages

//Error message
#define ERROR_PREFIX "ERROR"
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

#define REQ_PREFIX "TRANSFER REQ"
#define MAX_FILE_NAME_CHARS 100 //TBD - consider generating shortform filename ID for data packets
#define MAX_FILE_SIZE_CHARS 10 //Allows for a maximum file size of 10^9 bytes (i.e. 9.99 GB), which is large enough for our application

	//Allowable Filetypes
#define ASCII_FILE "ASCII"
#define BINARY_FILE "BIN"

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
#define FINAL_PREFIX "FINAL"
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
#define REQ_PREFIX "REQUEST ACKED"
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
#define CONF_PREFIX "TRANSFER STATUS"
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





