/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: testScanner.cpp
# Date: 02/24/21
==================================================================================================== */
#include "token.h"
#include "scanner.h"
#include "testScanner.h"

int testScanner(string fileName)
{
	//Init Scanner Object
	Scanner scanner;
	
	//Check if the file is open and associated with the stream object
	ifstream file(fileName.c_str());
	unsigned int currentLine = 1;  //Keep track of the current line number of the file
	Token token;                    //Token holder for displaying purpose (can be useful later in the future...)
	if(file.is_open()) {
		string input;
		while(getline(file, input)) {
			//Invoke scanner() until each token in the current line has been identified
			while(scanner.scan(currentLine, input, token) == 0) {
				scanner.tokenToString(token);
			}
			currentLine++;  //Increment the currnet line in the file when finish reading the current input
		}
		
		//Check for any open comment tag
		scanner.isCommentMode();
	}
	else {
		cout << "[ERROR] Can't open file!" << endl;
		return -1;
	}

	//Once all lines of the files has been traverse, print the EOF token
	if(file.eof()) {
		//Check if the current line is the initial line. If not, reduce by 1
		if(currentLine > 1) {
			currentLine--;
		}

		//Display EOF token
		token.line_number = currentLine;
		token.id = eofTk;
		token.value = "EOF";
		scanner.tokenToString(token);
	}

	//Close the reading file
	file.close();
	return 0;
}

