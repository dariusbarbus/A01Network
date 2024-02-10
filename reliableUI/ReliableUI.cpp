#include "Client.h"
#include <iostream>
#include <cstdlib> 


bool getClientInformation(Client& client);




int main()
{
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

		//START UDP AS CLIENT
		string reliableUDP = "reliableUDP.exe " + client.getIpAddress() + " " + client.getFilename();
		system(reliableUDP.c_str());
	}
	else if(choice == 2)
	{
		//START UDP AS SERVER
		string reliableUDP = "reliableUDP.exe";
		system(reliableUDP.c_str());
	}
	else
	{
		std::cout << "Invalid choice. Please select 1 or 2.\n";
	}

	return 0;
}




bool getClientInformation(Client& client)
{
	string ipAddress = " ";
	string filename = " ";

	cout << "What's the IP ADDRESS\n";
	cin >> ipAddress;

	cout << "What's the FILENAME\n";
	cin >> filename;

	bool isIpCorrect = client.setIpAddress(ipAddress);
	bool isFilenameCorrect = client.setFilename(filename);

	//get filesize

	if(!isIpCorrect || !isFilenameCorrect)
	{
		return false;
	}
	return true;
}
