#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "md5.h"


using namespace std;


class HashGen 
{
private:
    string filename;
    string hash;

public:
    HashGen(string filename)
    {
        this->filename = filename;
    }

    void hashValue(const string& filename)
    {
        const size_t chunkSize = 1024 * 1024; // 1 MB
        std::ifstream inputFile(filename, std::ios::binary);

        if (!inputFile) {
            std::cerr << "Could not open file for reading." << std::endl;
            return;
        }

        MD5 md5; // Assuming you have an MD5 class like this
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



        this->hash = md5.getHash();

    }

    string getHash()
    {
        return hash;
    }


};