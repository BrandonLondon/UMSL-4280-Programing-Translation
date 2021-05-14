/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: scanner.cpp
# Date: 02/24/21
==================================================================================================== */
#include "scanner.h"

Scanner::Scanner()
{
	initOperatorMap();
	initKeywordMap();
}

int Scanner::getCategory(char ch)
{
	//Refer to FSA_TABLE in scanner.h
	if (isalpha(ch)) {
		return 0;

	}
	else if (ch == '_') {

		return 0;

	}
	else if (isdigit(ch)) {
		return 1;  //Digit is in column 1	
	}
	else if (isspace(ch)) {
		return 2;  //White Space (WS) is in column 2
	}
	else if (isOperator(ch) != -1) { //isOperator() is from token.h
		return 4;  //Operator is in column 4
	}
	else {
		return 5;  //No match for accepted categories, will error out
	}
}

void Scanner::getError(int state, char ch)
{
	cout << "[ERROR] at " << getScannerPosition() << " -> {" << ch << "]: ";
	if (state == ERROR_INT) {
		cout << "all integer token must contain only digits." << endl;
		cout << "[ERROR] code: " << ERROR_INT << endl;
	}
	else if (state == ERROR_UNK) {
		cout << "unknown token is not allow." << endl;
		cout << "[ERROR] code: " << ERROR_UNK << endl;
	}
}

char Scanner::checkComment(char ch)
{;
	return ch;
}

string Scanner::getScannerPosition()
{
	ostringstream temp;
	temp << "(" << current_line_number << ":" << current_scanner_pointer << ")";
	return temp.str();
}

void Scanner::isCommentMode()
{
	if (is_commenting) {
		cout << "[WARNING] at (" << last_comment_position << ") -> comment tag never close" << endl;
	}
}

void Scanner::invokeEOF(Token& tk)
{
	//Check if the current line is the initial line. If not, reduce by 1
	tk.line_number = (current_line_number > 1) ? current_line_number - 1 : current_line_number;
	tk.id = eofTk;
	tk.value = "EOF";
}

void Scanner::filterScan(string input_file_name, string output_file_name)
{
	ifstream source_file(input_file_name.c_str());
	ofstream output_file(output_file_name.c_str());
	Token token;

	//Check if the file is open and associated with the stream object
	if (source_file.is_open()) {
		string input;
		while (getline(source_file, input)) {

			string scan_input;
			while (scan(input, token) == 0) {
				scan_input.append(token.data);

				if (current_scanner_pointer < input.length()) {
					scan_input.append(" ");
				}
			}

			//NOTE: can surround this to eliminate blank line if need to
			output_file << scan_input << endl;
		}
	}
	else {
		cout << "[ERROR] Can't open file!" << endl;
		exit(EXIT_FAILURE);
	}

	//Clean up and reset cursor and line position to default
	source_file.close();
	output_file.close();
	current_scanner_pointer = 0;
	current_line_number = 1;
}

int Scanner::scan(string& input, Token& tk)
{
	//Set current line number for the current token
	tk.line_number = current_line_number;

	//Init variable for tracking
	int current_state = 0;           
	int next_state = 0;              
	int next_category = 0;           
	string read_value;               
	char next_char;                  

	//Continue looping until scanner pointer is less than or equal to the string it compare
	while (current_scanner_pointer <= input.length()) {
		if (current_scanner_pointer < input.length()) {
			next_char = checkComment(input.at(current_scanner_pointer));
		}
		else {
			next_char = SCANNER_DELIMITER;
		}

		//Look at FSA Table and obtain the next state (row)
		next_category = getCategory(next_char);
		next_state = FSA_TABLE[current_state][next_category];

		//Check to see if this were an error state. Return -1 if it is.
		if (next_state < 0) {
			getError(next_state, next_char);
			//			exit(EXIT_FAILURE);  //Uncomment this if you want the scanner to stop scanning when encounting error
		}
		//Check to see if this were the final state. Return 0 if it is.
		else if (next_state > STATE_F) {
			//Set the token	holder with the current token read value	
			tk.value = read_value;

			//Look for a specific final state and assign the token holder with appropriate id and value
			switch (next_state) {
			case STATE_ID: //Identfier
				if (getKeyword(tk) != -1) { //Keyword
					tk.id = keywordTk;
					tk.data.assign(read_value);
					tk.type.assign("");
					tk.value.assign(read_value);
				}
				else {
					tk.id = idTk;
					tk.data.assign(read_value);
					tk.type.assign("idTk");
					tk.value.assign(read_value);
				}
				break;

			case STATE_INT: //Integer
				tk.id = intTk;
				tk.data.assign(read_value);
				tk.type.assign("intTk");
				tk.value.assign(read_value);
				break;

			case STATE_OP: 
				tk.id = opTk;
				if (current_scanner_pointer < input.length()) {
					string ns_operator = read_value + input.at(current_scanner_pointer);
					if (isNonSingleOperator(ns_operator) != -1) {
						read_value = ns_operator;
						current_scanner_pointer++;
					}
				}

				tk.data.assign(read_value);

				if (read_value == "$$") {

					is_commenting = !is_commenting;

					if (is_commenting) {
						current_scanner_pointer--;
						current_scanner_pointer--;
						ostringstream temp;
						temp << current_line_number << ":" << current_scanner_pointer;
						last_comment_position = temp.str();
						is_commenting = false;
						return SCANNER_DELIMITER;

					}

					return SCANNER_DELIMITER;
				}
				else {

					tk.value.assign("  " + read_value);
					tk.type.assign("");
					tk.value.assign(read_value);


				}
				getOperator(tk);
				tk.data.assign(read_value);
				tk.type.assign("");
				tk.value.assign(read_value);
				break;
			}

			if (is_commenting) {
				current_scanner_pointer++;
			}
			return 0;
		}
		current_state = next_state;
		current_scanner_pointer++;
		if (!isspace(next_char)) {
			read_value.push_back(next_char);
		}
	}
	current_scanner_pointer = 0;
	current_line_number++;
	return -1;
}
