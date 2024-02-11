// Filename: HashGen.h
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: Class file 


#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "md5.h"
#include <cstring> 

using namespace std;



/* -- CLASS COMMENT --
    NAME    :   HashGen
    PURPOSE :   Generates hash values for files using the MD5 algorithm.
*/
class HashGen 
{
private:
    char* filename;
    char hash[33];
    char hashCopy[33];

public:
    //
// METHOD      : HashGen (constructor)
// DESCRIPTION : Initializes a HashGen object.
// PARAMETERS  : None
// RETURNS     : None
//
    HashGen(){}


    //
// METHOD      : hashValue
// DESCRIPTION : Generates the hash value for the specified file.
// PARAMETERS  : 
//      const char* filename : The name of the file to generate the hash for.
// RETURNS     : None
//
    void hashValue(const char* filename)
    {
        const size_t chunkSize = 1024 * 1024; // 1 MB
        std::ifstream inputFile(filename, std::ios::binary);

        if (!inputFile) {
            std::cerr << "Could not open file for reading." << std::endl;
            return;
        }

        MD5 md5; 
        char* buffer = new char[chunkSize];

        while (!inputFile.eof()) {
            inputFile.read(buffer, chunkSize);
            size_t bytesRead = inputFile.gcount(); // Actual bytes read, important for handling the last chunk
            if (bytesRead > 0) {
                md5.add(buffer, bytesRead);
            }
        }

        // Cleanup
        delete[] buffer;

        inputFile.close();

        string retVal = md5.getHash();

        memcpy(hash, retVal.data(), retVal.size());


    }


    //
// METHOD      : getHash
// DESCRIPTION : Retrieves the generated hash value.
// PARAMETERS  : None
// RETURNS     : const char* - The generated hash value.
//
    const char* getHash()
    {
        memcpy(hashCopy, hash, sizeof(hash));
        return hashCopy;
    }


};