/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: scanner.h
# Date: 02/24/21
==================================================================================================== */
#ifndef SCANNER_H
#define SCANNER_H      
#include <iostream>      
#include <sstream>       
#include <string>        
#include <vector>   
#include <cctype>        
#include <cstdio>         
#include <cstdlib>       
#include <cstring>       
#include <fstream> 
#include "token.h"
using namespace std;

/* Scanner Class */
class Scanner: public Language
{
        private:
                //Scanner Tracker
                unsigned int current_scanner_pointer = 0;   
                unsigned int current_line_number = 1;       //Keep track of the current line number.
                bool is_commenting = false;                 //Keep track when the scanner encounter a comment.
                string last_comment_position = "";          //Keep track the last known open comment tag.

                //FSA TABLE
                enum {
                        ERROR_UNK = -2,    
                        ERROR_INT = -1,    
                        STATE_0 = 0,       
                        STATE_1 = 1,       
                        STATE_2 = 2,       
                        STATE_3 = 3,       
                        STATE_F = 1000,    
                        STATE_ID = 1001,   
                        STATE_INT = 1003,  
                        STATE_OP = 1004,   
                        STATE_EOF = 1005   
                };
                const int FSA_TABLE[4][6] = {
                        { STATE_2, STATE_3, STATE_0, STATE_EOF, STATE_1, ERROR_UNK },
                        { STATE_OP, STATE_OP, STATE_OP, STATE_OP, STATE_OP, ERROR_UNK },
                        { STATE_2, STATE_2, STATE_ID, STATE_ID, STATE_ID, ERROR_UNK },
                        { ERROR_INT, STATE_3, STATE_INT, STATE_INT, STATE_INT, ERROR_UNK }
                };

                const char SCANNER_DELIMITER = ' ';

                int getCategory(char ch);
                void getError(int state, char ch);
                char checkComment(char ch);

        public:
                Scanner();
                void filterScan(string input_file_name, string output_file_name);
                int scan(string &input, Token &tk);
                string getScannerPosition();
                void isCommentMode();
                void invokeEOF(Token &tk);
};
#endif
