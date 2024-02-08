//LET USER SELECT MODE
	//CLIENT
		//IF CLIENT SELECTED, ASK FOR ADITIONAL INFORMATION
			//IP OF SERVER
			//FILENAME
	//SERVER
//CALL UDP BASED ON MODE
#include "Client.h"
#include <iostream>


int main()
{
	//Client client;
	string ipAddress = " ";
	int port = 0;
	string filename = " ";


	cout << "What's the IP ADDRESS\n";
	cin >> ipAddress;

	cout << "What's the PORT\n";
	cin >> port;

	cout << "What's the FILENAME\n";
	cin >> filename;



	Client client(ipAddress, port, filename);
	cout << client.getIpAddress() << endl;
	cout << client.getPort() << endl;
	cout << client.getFilename() << endl;

	return 0;
}
