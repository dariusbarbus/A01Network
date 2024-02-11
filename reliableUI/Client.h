// Filename: Client.h
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: Class file 

#pragma once
#include <string>
#include <iostream>
#include <regex>
using namespace std;

#define MIN_PORT 1
#define MAX_PORT 65535



/* -- CLASS COMMENT --
    NAME    :   Client
    PURPOSE :   Represents a client with an IP address and a filename.
*/
class Client{
private:
    string ipAddress;
    string filename;

public:
    //
    // METHOD      : Client (constructor)
    // DESCRIPTION : Initializes a Client object with default values.
    // PARAMETERS  : None
    // RETURNS     : None
    //
    Client()
    {
        ipAddress = "";
        filename = "";
    }

    //
// METHOD      : getIpAddress
// DESCRIPTION : Retrieves the IP address.
// PARAMETERS  : None
// RETURNS     : string - The IP address.
//
    string getIpAddress()
    {
        return ipAddress;
    }


    //
// METHOD      : getFilename
// DESCRIPTION : Retrieves the filename.
// PARAMETERS  : None
// RETURNS     : string - The filename.
//
    string getFilename()
    {
        return filename;
    }


    //
    // METHOD      : setIpAddress
    // DESCRIPTION : Sets the IP address.
    // PARAMETERS  : 
    //      string ipAddress : The IP address to set.
    // RETURNS     : bool - True if the IP address is valid and set successfully, false otherwise.
    //
    bool setIpAddress(string ipAddress)
    {
        bool retValue = isIpAddressValid(ipAddress);
        if (!retValue)
        {
            return false;
        }
        this->ipAddress = ipAddress;
        return true;
    }


    //
// METHOD      : setFilename
// DESCRIPTION : Sets the filename.
// PARAMETERS  : 
//      string filename : The filename to set.
// RETURNS     : bool - True if the filename is valid and set successfully, false otherwise.
//
    bool setFilename(string filename)
    {
        bool retValue = isFilenameValid(filename);
        if (!retValue)
        {
            return false;
        }
        this->filename = filename;
        return true;
    }


    //
// METHOD      : isIpAddressValid
// DESCRIPTION : Checks if the provided IP address is valid.
// PARAMETERS  : 
//      string& ip : The IP address to validate.
// RETURNS     : bool - True if the IP address is valid, false otherwise.
//
    bool isIpAddressValid(string& ip)
    {
        std::regex ipRegex("^\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b$");
        return std::regex_match(ip, ipRegex);
    }


    //
// METHOD      : isPortValid
// DESCRIPTION : Checks if the provided port number is valid.
// PARAMETERS  : 
//      int port : The port number to validate.
// RETURNS     : bool - True if the port number is valid, false otherwise.
//
    bool isPortValid(int port)
    {
        if (port >= MIN_PORT && port <= MAX_PORT)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    //
// METHOD      : isFilenameValid
// DESCRIPTION : Checks if the provided filename is valid.
// PARAMETERS  : 
//      string& file : The filename to validate.
// RETURNS     : bool - True if the filename is valid, false otherwise.
//
    bool isFilenameValid(string& file)
    {
        std::regex filenameRegex("^[^.]+\\.[^.]+$");    //checks for an extension
        return std::regex_match(file, filenameRegex);
    }


};
