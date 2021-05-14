/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: main.cpp
# Date: 3/23/2021
# Purpose:
	Display any violations of the static semantics and generate ASM.
==================================================================================================== */
#include <cstdio>        
#include <cstring>       
#include <fstream>      
#include <iostream>      
#include <string>        
#include "parser.h"
#include "semantics.h"
#include "testTree.h"
using namespace std;

/* ====================================================================================================
MAIN
==================================================================================================== */
int main(int argc, char* argv[])
{
	/* File Variable */
	string file_name;
	string output_file_name;

	/* Processing program given arguments */
	switch (argc)
	{
	case 1:/* Reading from the keyboard */
	{
		cout << ">>: Reading from the keyboard." << endl;
		string user_input;
		ofstream temp_file;

		//Set file name for temporary file
		file_name = "temp.dat";
		temp_file.open(file_name.c_str());

		//Writing user input into a temporary file and close the file when done
		cout << "Enter token (return an empty line to finish).";
		do {
			cout << endl << ">>: ";
			getline(cin, user_input);
			temp_file << user_input << endl;
		} while (!user_input.empty());

		//Close file when done
		cout << ">>: Finish inputting! Proceeding..." << endl << endl;
		temp_file.close();
		output_file_name = "kb";
		break;
	}

	case 2:/* Reading from file */
	{
		cout << "[NOTICE] Reading from file." << endl << endl;

		//Construct output file name
		file_name = argv[1];
		size_t extension_index = file_name.find(".");

		//Check if there is extension. If not, append the implicit extension. As well as grabbing the file name for the output file
		if (extension_index == string::npos) {
			output_file_name = file_name;
			file_name.append(".fs");
		}
		else {
			output_file_name = file_name.substr(0, extension_index);
		}
		break;
	}

	default:/* Invalid Usage */
	{
		cout << ">>: Too many arguments!" << endl;
		cout << "Usage: " << argv[0] << " [file]" << endl;
		return 1;
	}
	}

	/* Compiler Driver */
	Parser parser(file_name);
	Semantics semantics(output_file_name);

	/* Invoke functionality */
	NodeT* root = parser.parse();
	//traversePreorder(root);  //Temporary turn OFF
	semantics.codeGeneration(root);

	return 0;
}

