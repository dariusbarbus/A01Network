#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>



using namespace std;


//FIRST WE USE setExtension to get the file extension and properly write the following file
//need to get actual file name instead of using "newFile"



class FileIO
{
private:
	string filename;
	string extension;

public:
	FileIO()
	{
		filename = "";
		extension = "";
	}

	void setExtension(string& filename)
	{
		size_t dot = filename.rfind('.');
		if (dot != string::npos)
		{
			this->extension = filename.substr(dot);
		}
	}

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
