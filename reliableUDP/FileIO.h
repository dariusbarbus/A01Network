// Filename: FileIO.h 
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: Class file 

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/* -- CLASS COMMENT --
	NAME    :   FileIO
	PURPOSE :   Provides functionality for file input and output operations.
*/
class FileIO
{
private:
	string filename;
	string extension;

public:
	//
// METHOD      : FileIO (constructor)
// DESCRIPTION : Initializes a FileIO object with default values.
// PARAMETERS  : None
// RETURNS     : None
//
	FileIO()
	{
		filename = "";
		extension = "";
	}


	//
// METHOD      : setExtension
// DESCRIPTION : Sets the extension of the filename.
// PARAMETERS  : 
//      string& filename : The filename to extract the extension from.
// RETURNS     : None
//
	void setExtension(string& filename)
	{
		size_t dot = filename.rfind('.');
		if (dot != string::npos)
		{
			this->extension = filename.substr(dot);
		}
	}



	//
// METHOD      : processFile
// DESCRIPTION : Processes a file by reading it in chunks and writing to a new file.
// PARAMETERS  : 
//      const string& filename : The name of the file to be processed.
// RETURNS     : None
//
	void processFile(const string& filename)
	{
		ifstream inFile(filename, ios::binary);
		string newFilename = "newFile" + extension;		//MODIFY HERE TO GET THE CORRECT NAME

		vector<char> buffer;

		ofstream outFile(newFilename, ios::binary | ios::app);

		while (!inFile.eof())
		{
			const size_t chuckSize = 256;
			const size_t bufferSize = 1024;

			char chunk[chuckSize];
			inFile.read(chunk, chuckSize);
			size_t bytesRead = inFile.gcount();		//counts the amount of bytes processed

			buffer.insert(buffer.end(), chunk, chunk + bytesRead);	//adds all

			if (buffer.size() >= bufferSize)
			{
				outFile.write(&buffer[0], bufferSize);
				buffer.erase(buffer.begin(), buffer.begin() + bufferSize);	//writes the file in 1mb chunks
			}
		}

		if (!buffer.empty())
		{
			outFile.write(&buffer[0], buffer.size());		//writes the last bytes if its less than 1mb
		}


	}
};
