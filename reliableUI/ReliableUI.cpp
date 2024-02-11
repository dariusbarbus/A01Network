// Filename: ReliableUI.cpp
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: UI

#include "Client.h"
#include <iostream>
#include <cstdlib> 


bool getClientInformation(Client& client);




int main()
{
	int choice;

	// Display menu
	std::cout << "Welcome to Reliable UDP\n";

	Client client;
	bool retValue = getClientInformation(client);
	if (!retValue)
	{
		return 0;
	}

	//START UDP AS CLIENT
	string reliableUDP = "reliableUDP.exe " + client.getIpAddress() + " " + client.getFilename();
	system(reliableUDP.c_str());


	return 0;
}



//
// FUNCTION    : getClientInformation
// DESCRIPTION : Retrieves client information, IP address and filename.
// PARAMETERS  :
//      Client& client : Reference to the Client object to store the information.
// RETURNS     :
//      bool : True if the client information is successfully retrieved, false otherwise.
//

bool getClientInformation(Client& client)
{
	string ipAddress = " ";
	string filename = " ";

	cout << "What's the IP Address: ";
	cin >> ipAddress;

	cout << "What's the Filename: ";
	cin >> filename;

	bool isIpCorrect = client.setIpAddress(ipAddress);
	bool isFilenameCorrect = client.setFilename(filename);

	if(!isIpCorrect || !isFilenameCorrect)
	{
		return false;
	}
	return true;
}
