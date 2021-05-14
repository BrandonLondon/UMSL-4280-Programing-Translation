/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: token.h
# Date: 02/24/2021
==================================================================================================== */
#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>      
#include <string>        
#include <map>          
#include <vector>        
using namespace std;

/* Enumeration */
enum token_id { idTk, keywordTk, intTk, opTk, eofTk };

/* Token Structure */
typedef struct token
{
	token_id id;        //token category
	int line_number;    //token location
	string value;       //value of the token
} Token;

/* Language Class */
class Language
{
	protected:
		//Operator Map & Keyword Map
		map<string, string> operator_map;
		map<string, string> keyword_map;

		//Token Categories (should match with token_id enum)
		const vector<string> token_names = { 
			"Identfier", "Keyword", "Integer", "Operator", "End Of File"
		};

		//Reserved Keyword
		const vector<string> keywords = {
			"begin", "end", "loop", "whole", "void", "exit", 
			"getter", "outter", "main", "if", "then", 
			"assign", "data", "proc"
		};

		//Delimiter
		const char COMMENT_DELIMITER = '$';

		//Operator (single and non-single)
		const vector<char> operators = {
			'=', '<', '>', ':',
			'+', '-', '*', '/', '%',
			'.', ',', ';', '(', ')',
			'{', '}', '[', ']'
		};
		const vector<string> ns_operators = {
			":=", "==", "=>", "=<"
		};
		
		const vector<char> identifiers = { '_' 
		};
			
		void initOperatorMap();
		void initKeywordMap();
		int isOperator(char ch);
		int isNonSingleOperator(string str);
		int getOperator(Token &tk);
		int getKeyword(Token &tk);

	public:
		void tokenToString(Token tk);
};
#endif

