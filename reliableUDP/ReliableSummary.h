//THIS CLASS IS SELF-CONTAINT
//WE NEED TO MODIFY UDP TO USE THIS CLASS FOR THE SUMMARY OUTPUT
//IT WILL READ THE OUTPUT FROM A FILE
//I REALLY HATE OOP

#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Summary
{
private:

public:
	Summary(){}

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
