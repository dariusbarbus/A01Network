//Outline of protocol, client/server messages, etc.



//SENDER
//========

//Send Request
	//Filename
	//Filesize
	//Filetype


//Data packet
//Header
	//Filename
	//Filesize
	//Size of Payload (bytes)
//Payload

//Final packet with hash
//Header
	//Filename
	//Filesize
//Hash

//Error message
	//Header
	//Error message (code and/or text)




//RECEIVER
//========

//Response to Request
	//Proceed
	//Don't Proceed

//Response to final packet
	//Comparison to hash complete - file transferred successfully
	//File transfer unsuccessful

//Error message
	//Header
	//Error message (code and/or text)





