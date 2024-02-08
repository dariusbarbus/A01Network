//THIS CLASS WILL HANDLE LOCAL FILE IO
//IT WILL LOAD AND SAVE THE FILE TO THE DRIVE

#include <string>

class LocalFile
{
private:
	std::string filename;


public:
	//CONSTRUCTOR
	LocalFile(){}

	//destructor
	~LocalFile(){}

	void LoadFile(std::string filename)
	{
		//THIS METHOD SHOULD LOAD THE FILE FROM THE DRIVE CREATING A FILE STREAM
	}

	void SaveFile(FILE *filestream)
	{
		//THIS METHOD SHOULD SAVE THE FILE TO THE DRIVE, THIS MIGHT ALSO MEAN CREATING A FILE
	}



};
