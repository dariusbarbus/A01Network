#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "md5.h"
#include <cstring> 

using namespace std;


class HashGen 
{
private:
    char* filename;
    char hash[33];
    char hashCopy[33];

public:
    HashGen(){}

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

    const char* getHash()
    {
        memcpy(hashCopy, hash, sizeof(hash));
        return hashCopy;
    }


};