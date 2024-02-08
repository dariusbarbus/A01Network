#pragma once
#include <string>
#include <iostream>
#include <regex>
using namespace std;

#define MIN_PORT 1
#define MAX_PORT 65535


class Client{
private:
    string ipAddress;
    int port;
    string filename;

public:
    Client()
    {
        ipAddress = "";
        port = 1;
        filename = "";
    }

    string getIpAddress()
    {
        return ipAddress;
    }

    int getPort()
    {
        return port;
    }

    string getFilename()
    {
        return filename;
    }



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

    bool setPort(int port)
    {
        bool retValue = isPortValid(port);
        if (!retValue)
        {
            return false;
        }
        this->port = port;
        return true;
    }

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



    bool isIpAddressValid(string& ip)
    {
        std::regex ipRegex("^\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b$");
        return std::regex_match(ip, ipRegex);
    }


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


    bool isFilenameValid(string& file)
    {
        std::regex filenameRegex("^[^.]+\\.[^.]+$");    //checks for an extension
        return std::regex_match(file, filenameRegex);
    }


};
