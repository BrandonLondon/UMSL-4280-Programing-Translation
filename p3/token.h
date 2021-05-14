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
#include <vector>  
#include <map>                 
using namespace std;

enum token_id { idTk, keywordTk, intTk, opTk, eofTk };

/* Token Structure */
typedef struct token
{
        token_id id;        //The id that represent the token category
        int line_number;    //Line location of where the token reside
        string data;        //The actual scan value of the token
        string value;       //The description value of the token
        int blockNum = 0;
} Token;

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
                        "begin", "end", "outter", "void", "loop", "void", "data",  "getter", "if",
                        "then", "assign", "data",  "proc", "main"
                };

                //Delimiter
                const char COMMENT_DELIMITER = '#';

                //Operator (single and non-single)
                const vector<char> operators = {
                        '=',  ':', '$',
                        '+', '-', '*', '/', '%',
                        '.', ',', ';', '(', ')',
                        '{', '}', '[', ']', '_', '>', '<'
                };
                const vector<string> ns_operators = {
                        "=>", "=<", ":=", "==", "$$"
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
