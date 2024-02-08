#include "Client.h"
#include <iostream>
#include <regex>



Client::Client(string ipAddress, int port, string filename)
{
    this->ipAddress = ipAddress;
    this->port = port;
    this->filename = filename;
}

string Client::getIpAddress()
{
    return ipAddress;
}

int Client::getPort()
{
    return port;
}

string Client::getFilename()
{
    return filename;
}

void Client::setIpAddress(string ipAddress)
{
    this->ipAddress = ipAddress;
}

void Client::setPort(int port)
{
    this->port = port;
}

void Client::setFilename(string filename)
{
    this->filename = filename;
}


bool Client::isIpAddressValid(string& ip)
{
    std::regex ipRegex("^((\\d{1,3}\\.){3}\\d{1,3})$");     //checks only the format of the string
    return regex_match(ip, ipRegex);
}


void Client::printData()
{
	
}



//void getInput() {
//    std::cout << "Enter IP Address: ";
//    std::cin >> Client::ipAddress;
//
//    std::cout << "Enter Port: ";
//    std::cin >> port;
//
//    std::cout << "Enter Filename: ";
//    std::cin >> filename;
//}

