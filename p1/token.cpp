/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: token.cpp
# Date: 02/21/2021
==================================================================================================== */
#include "token.h"

/*Assigns the Operator to the Operator Map. */
void Language::initOperatorMap()
{
	operator_map.insert(make_pair(":=", "colonEqualTk"));
	operator_map.insert(make_pair("==", "equalEqualTk"));
	operator_map.insert(make_pair("=", "equalTk"));
	operator_map.insert(make_pair("<", "lessThanTk"));
	operator_map.insert(make_pair(">", "greaterThanTk"));
	operator_map.insert(make_pair(":", "colonTk"));
	operator_map.insert(make_pair("+", "plusTk"));
	operator_map.insert(make_pair("-", "minusTk"));
	operator_map.insert(make_pair("*", "multiplyTk"));
	operator_map.insert(make_pair("/", "divideTk"));
	operator_map.insert(make_pair("%", "modulusTk"));
	operator_map.insert(make_pair(".", "dotTk"));
	operator_map.insert(make_pair(",", "commaTk"));
	operator_map.insert(make_pair(";", "semiColonTk"));
	operator_map.insert(make_pair("(", "leftParenTk"));
	operator_map.insert(make_pair(")", "rightParenTk"));
	operator_map.insert(make_pair("{", "leftCurlyTk"));
	operator_map.insert(make_pair("}", "rightCurlyTk"));
	operator_map.insert(make_pair("[", "leftBracketTk"));
	operator_map.insert(make_pair("]", "rightBracketTk"));
}


/* Assigns keyword to keymap */
void Language::initKeywordMap()
{
	keyword_map.insert(make_pair("begin", "beginTk"));
	keyword_map.insert(make_pair("end", "endTk"));
	keyword_map.insert(make_pair("loop", "loopTk"));
	keyword_map.insert(make_pair("whole", "wholeTk"));
	keyword_map.insert(make_pair("void", "voidTk"));
	keyword_map.insert(make_pair("exit", "exitTk"));
	keyword_map.insert(make_pair("getter", "getterTk"));
	keyword_map.insert(make_pair("outter", "outterTk"));
	keyword_map.insert(make_pair("main", "mainTk"));
	keyword_map.insert(make_pair("if", "ifTk"));
	keyword_map.insert(make_pair("then", "thenTk"));
	keyword_map.insert(make_pair("assign", "assignTk"));
	keyword_map.insert(make_pair("data", "dataTk"));
	keyword_map.insert(make_pair("proc", "procTk"));
}


/* display the line number, category of the token (Identifier, Integer, Operator, etc), token name (IDtk, THENtk, etc), and the token description (value of the token).*/
void Language::tokenToString(Token tk)
{
	cout << "Line #" << tk.line_number << ": " << token_names[tk.id] << " | " << tk.value << endl; 
}


/* checks to see if given char is operator, then returns true or false*/
int Language::isOperator(char ch)
{
	//Return 1 when input character is an operator
	for(unsigned int i = 0; i < operators.size(); i++) {
		if(ch == operators[i]) {
			return 1;
		}
	}
	
	//Return -1 when input character is not an operator
	return -1;
}


/* return true (1) or false (-1) if the input char is a non-single operator.*/
int Language::isNonSingleOperator(string str)
{
	//Return 1 when input string is a non-single operator
	for(unsigned int i = 0; i < ns_operators.size(); i++) {
		if(str.compare(ns_operators[i]) == 0) {
			return 1;
		}
	}

	//Return -1 when input character is not a non-single operator
	return -1;
}


/* return single or non-single operator key (assuming it already been verify it's an operator). */
int Language::getOperator(Token &tk)
{
	//Check single character operator
	for(unsigned int i = 0; i < operators.size(); i++) {
		string op(1, operators[i]);
		if(tk.value.compare(op) == 0) {
			tk.value = operator_map[tk.value];
			return i;
		}
	}

	//Check non-single character operator
	for(unsigned int i = 0; i < ns_operators.size(); i++) {
		if(tk.value.compare(ns_operators[i]) == 0) {
			tk.value = operator_map[tk.value];
			return i;
		}
	}

	//Return -1 when input token is not a keyword
	return -1;
}


/* return keyword key if it's a keyword. */
int Language::getKeyword(Token &tk)
{
	//Return keyword key when input token is a keyword
	for(unsigned int i = 0; i < keywords.size(); i++) {
		if(tk.value.compare(keywords[i]) == 0) {
			tk.value = keyword_map[tk.value];
			return i;
		}
	}

	//Return -1 when input token is not a keyword
	return -1;
}

