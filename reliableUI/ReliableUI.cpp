//LET USER SELECT MODE
	//CLIENT
		//IF CLIENT SELECTED, ASK FOR ADITIONAL INFORMATION
			//IP OF SERVER
			//FILENAME
	//SERVER
//CALL UDP BASED ON MODE
#include "Client.h"
#include <iostream>


bool getClientInformation(Client& client);




int main()
{
	//Client client;
	//string ipAddress = " ";
	//int port = 0;
	//string filename = " ";


	//cout << "What's the IP ADDRESS\n";
	//cin >> ipAddress;

	//cout << "What's the PORT\n";
	//cin >> port;

	//cout << "What's the FILENAME\n";
	//cin >> filename;

	//client.setIpAddress(ipAddress);
	//client.setPort(port);
	//client.setFilename(filename);


	//select client or server
	//if client is selected
		//get values
		//pass values to reliable udp
	//if server is selected
		//start reliable udp with no parameters


	int choice;

	// Display menu
	std::cout << "Select an option:\n";
	std::cout << "1. Client\n";
	std::cout << "2. Server\n";
	std::cout << "Enter your choice: ";
	std::cin >> choice;

	if(choice == 1)
	{
		Client client;
		bool retValue = getClientInformation(client);
		if(!retValue)
		{
			return 0;
		}
		//get values from client class
		//call server with values
	}
	else if(choice == 2)
	{
		//start server
	}
	else
	{
		//error 
	}

	// Process user choice using switch statement
	//switch (choice) {
	//case 1:
	//	Client client;
	//	bool retValue = getClientInformation(client);
	//	if(!retValue)
	//	{
	//		break;
	//	}
	//	break;
	//case 2:
	//	//CALL RELIABLE UDP
	//	break;
	//default:
	//	std::cout << "Invalid choice. Please select 1 or 2.\n";
	//	break;
	//}

	return 0;
}




bool getClientInformation(Client& client)
{
	string ipAddress = " ";
	int port = 0;
	string filename = " ";


	cout << "What's the IP ADDRESS\n";
	cin >> ipAddress;

	cout << "What's the PORT\n";
	cin >> port;

	cout << "What's the FILENAME\n";
	cin >> filename;

	//RETURN VALUE IS BOOL
	//CHECK FOR ERRORS AND DISPLAY
	bool isIpCorrect = client.setIpAddress(ipAddress);
	bool isPortCorrect = client.setPort(port);
	bool isFilenameCorrect = client.setFilename(filename);

	if(!isIpCorrect || !isPortCorrect || !isFilenameCorrect)
	{
		return false;
	}
	return true;
}
