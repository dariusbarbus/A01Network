#pragma once
#include <string>
#include <iostream>
using namespace std;

class Client{
private:
    string ipAddress;
    int port;
    string filename;

public:
    //CONSTRUCTOR
    Client();
	Client(string ipAddress, int port, string filename);


    //GETTERS
    string getIpAddress();
    int getPort();
    string getFilename();

	//SETTERS
    void setIpAddress(string ipAddress);
    void setPort(int port);
    void setFilename(string filename);

    //CHECKER
    bool isIpAddressValid(string& ip);


    void printData();
    //void getInput();

    //string getIPAddress();

    //int getPort();

    //string getFilename();
};


