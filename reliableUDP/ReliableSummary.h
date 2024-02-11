// Filename: ReliableSummary.h
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: Header file

#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


/* -- CLASS COMMENT --
    NAME    :   Summary
    PURPOSE :   Provides functionality for reading and displaying summary information from files.
*/
class Summary
{
private:

public:
    //
// METHOD      : Summary (constructor)
// DESCRIPTION : Initializes a Summary object.
// PARAMETERS  : None
// RETURNS     : None
//
	Summary(){}



    //
// METHOD      : Reader
// DESCRIPTION : Reads and displays the contents of the specified file.
// PARAMETERS  : 
//      string filename : The name of the file to read.
// RETURNS     : None
//
	void Reader(string filename)
	{
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (getline(file, line)) {
                std::cout << line << std::endl;
            }
            file.close();
        }
        else {
            std::cout << "Unable to open file: " << filename << std::endl;
        }
	}
};
