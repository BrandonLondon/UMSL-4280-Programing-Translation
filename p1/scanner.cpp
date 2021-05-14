/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: scanner.cpp
# Date: 02/24/21
==================================================================================================== */
#include "scanner.h"

/* Initialize operator and keyword maps*/
Scanner::Scanner()
{
	initOperatorMap();
	initKeywordMap();
}

/* check and return column location */

int Scanner::getCategory(char ch)
{
	//Refer to FSA_TABLE in scanner.h
	if(isalpha(ch)) {
		return 0;  //Letter is in column 0
	}
	else if(ch == '_') {
		return 0;
	}
	else if(isdigit(ch)) {
		return 1;  //Digit is in column 1	
	}
	else if(isspace(ch)) {
		return 2;  //White Space (WS) is in column 2
	}
	else if(isOperator(ch) != -1) { //isOperator() is from token.h
		return 4;  //Operator is in column 4
	}
	else {
		return 5;  //No match for accepted categories, will error out
	}
}

/* Print error message*/
void Scanner::getError(int currentLine, int state, char ch)
{
	cout << ">>:[ERROR] at (" << currentLine << ":" << scannerPointer << ") -> {" << ch << "]: ";
	if(state == ERROR_INT) {
		cout << ">>:all integer token must contain only digits." << endl;
		cout << ">>:[ERROR] code: " << ERROR_INT << endl;
	}
	else if(state == ERROR_UNK) {
		cout << ">>:unknown token is not allow." << endl;
		cout << ">>:[ERROR] code: " << ERROR_UNK << endl;
	}
}

/* check for comments */
char Scanner::checkComment(int currentLine, char ch)
{
	if(ch == COMMENT_DELIMITER) {
		isCommenting = !isCommenting;
		
		if(isCommenting) {
			ostringstream temp;
			temp << currentLine << ":" << scannerPointer;
			lastCommentPosition = temp.str();
		}
		return SCANNER_DELIMITER;
	}

	if(isCommenting) {
		return SCANNER_DELIMITER;
	}
	else {
		return ch;
	}
}

/* Check to see if comment is still open if not send out Warning Message */
void Scanner::isCommentMode()
{
	if(isCommenting) {
		cout << ">>:[WARNING] at (" << lastCommentPosition << ") -> comment tag never close" << endl;
	}
}

/* main driver function that determine lexical using FSA table */ 
int Scanner::scan(int currentLine, string &input, Token &tk)
{
	//Set current line number for the current token
	tk.line_number = currentLine;

	//Init variable for tracking
	int state = 0;           //Keep track of the current FSA state
	int nextState = 0;              //Refer to the row of FSA Table
	int nextCategory = 0;           //Refer to the column of FSA Table
	string currentTokenValue;               //The current reading value of the token
	char nextCharacter;                  //Keep track of the current token of the input

	//Continue looping until scanner pointer is less than or equal to the string it compare
	while(scannerPointer <= input.length()) {
		if(scannerPointer < input.length()) {
			nextCharacter = checkComment(currentLine, input.at(scannerPointer));
		} 
		else {
			nextCharacter = SCANNER_DELIMITER;
		}

		//Look at FSA Table and obtain the next state (row)
		nextCategory = getCategory(nextCharacter);
		nextState = FSA_TABLE[state][nextCategory];

		//Check to see if this were an error state. Return -1 if it is.
		if(nextState < 0) {
			getError(currentLine, nextState, nextCharacter);
			//exit(EXIT_FAILURE);  //Uncomment this if you want the scanner to stop scanning when encounting error
		}
		//Check to see if this were the final state. Return 0 if it is.
		else if(nextState > STATE_F) {
			//Set the token	holder with the current token read value	
			tk.value = currentTokenValue;

			//Look for a specific final state and assign the token holder with appropriate id and value
			switch(nextState) {
				case STATE_ID: //Identfier
					if(getKeyword(tk) != -1) { //Keyword?
						tk.id = keywordTk;
						tk.value.append(" " + currentTokenValue);
					}
					else {
						tk.id = idTk;
						tk.value.assign("idTk " + currentTokenValue);
					}
					break;

				case STATE_INT: //Integer
					tk.id = intTk;
					tk.value.assign("intTk " + currentTokenValue);
					break;

				case STATE_OP: //Operator
					tk.id = opTk;
					
					//Check to see if it non-single operator
					if(scannerPointer < input.length()) {
						string ns_operator = currentTokenValue + input.at(scannerPointer);
						if(isNonSingleOperator(ns_operator) != -1) {
							currentTokenValue = ns_operator;
							scannerPointer++;
						}
					}

					tk.value.assign(currentTokenValue);
					getOperator(tk);
					tk.value.append(" " + currentTokenValue);
					break;
			}

			//Increment scanner pointer when open comment tag is found
			if(isCommenting) {
				scannerPointer++;
			}
			return 0;
		}

		//Update state and scanner pointer
		state = nextState;
		scannerPointer++;

		//If not white space, update reading value of the token
		if(!isspace(nextCharacter)) {
			currentTokenValue.push_back(nextCharacter);
		}
	}

	//When scanner pointer is looking at the end of the input, end scanner.
	scannerPointer = 0;
	return -1;
}

