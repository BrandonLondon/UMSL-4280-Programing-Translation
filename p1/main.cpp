/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: main.cpp
# Date: 02/24/21
==================================================================================================== */
#include <iostream>      
#include <string>        
#include <cstdio>       
#include <fstream>  
#include <cstring>       
#include "testScanner.h"
using namespace std;


/* ====================================================================================================
MAIN
==================================================================================================== */
int main(int argc, char *argv[])
{
	/* File Variable */
	string fileName;
	string nameOfOutput;
		
	/* Processing program given arguments */
	switch(argc)
	{
		case 1:/* Reading from the keyboard */
		{
			cout << ">>:Reading from the keyboard." << endl;
			string userInput;
			ofstream tempFile;

			//Set file name for temporary file
			fileName = "temp.dat";
			tempFile.open(fileName.c_str());

			//Writing user input into a temporary file and close the file when done
			cout << ">>:Enter token (return an empty line to finish).";
			do
			{
				cout << endl << ">>: ";
				getline(cin, userInput);
				tempFile << userInput << endl;
			}
			while(!userInput.empty());	

			//Close file when done
			cout << ">>:Finish inputting! Proceeding..." << endl << endl;
			tempFile.close();
			nameOfOutput = "output";
			break;
		}

		case 2:/* Reading from file */
		{
			cout << ">>:Reading file." << endl << endl;

			//Construct output file name
			fileName = argv[1];
			size_t extension = fileName.find(".");

			//Check if there is extension. If not, append the implicit extension. As well as grabbing the file name for the output file
			if(extension == string::npos)
			{
				nameOfOutput = fileName;
				fileName.append(".fs");
			}
			else
			{
				nameOfOutput = fileName.substr(0, extension);
			}
			break;
		}
		
		default:/* Invalid Usage */
		{
			cout << ">>:ERROR! Too many arguments!" << endl;
			cout << "Usage: " << argv[0] << " [file]" << endl;
			return 1;
		}
	}

	/* Invoking Test Program */
	testScanner(fileName);

	return 0;
}

