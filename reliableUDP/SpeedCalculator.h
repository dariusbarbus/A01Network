// Filename: SpeedCalculator.h
// Project: SENG2040 – ASSIGNMENT 1
// By: Dario Simpson & Francis Knowles
// Date: February 10, 2024
// Description: Class file

#pragma once



//
// CLASS       : SpeedCalculator
// DESCRIPTION : Calculates the speed based on file size and transfer time.
//
class SpeedCalculator{
private:
    double fileSizeMB; // File size in megabytes
    double transferTimeSeconds; // Transfer time in seconds

public:

    //
// METHOD      : SpeedCalculator (constructor)
// DESCRIPTION : Initializes a SpeedCalculator object with default values.
// PARAMETERS  : None
// RETURNS     : None
//
    SpeedCalculator()
    {
        fileSizeMB = 0;
        transferTimeSeconds = 0;
    }


    //
// METHOD      : setFileSizeMB
// DESCRIPTION : Sets the file size in megabytes.
// PARAMETERS  :
//      double size : Size of the file in megabytes.
// RETURNS     : void
//
    void setFileSizeMB(double size)
	{
        fileSizeMB = size;
    }



    //
// METHOD      : setTransferTimeSeconds
// DESCRIPTION : Sets the transfer time in seconds.
// PARAMETERS  :
//      double time : Transfer time in seconds.
// RETURNS     : void
//
    void setTransferTimeSeconds(double time)
	{
        transferTimeSeconds = time;
    }



    //
// METHOD      : calculate
// DESCRIPTION : Calculates the transfer speed.
// RETURNS     : double - Transfer speed.
//
    double calculate() {
        // 1 Megabyte (MB) = 8 Megabits (Mb)
        double fileSizeMb = fileSizeMB * 8;

    	return fileSizeMb / transferTimeSeconds;
    }
};
