/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: parser.h
# Date: 3/23/2021
==================================================================================================== */
#ifndef PARSER_H
#define PARSER_H      
#include <iostream> 
#include <cstdio>        
#include <cstdlib>       
#include <cstring>       
#include <fstream> 
#include <string>        
#include "node.h"
#include "scanner.h"
using namespace std;

/* Parser Class */
class Parser: public Scanner
{
        private:
                ifstream file;            //File pointer
                string file_input;        //Keep track of the current line input of the file

                Token tk;                 //Token holder
                Token EMPTY_TK;           //Empty token holder
                string expected_token;    //Keep track of what token is expected when there is an error

                Scanner scanner;          //Scanner object
                int scanner_status_code;  //Keep track of the scanner status wheter to grab a new line or use old line

                //BNF of the program
                NodeT *program();
                NodeT *block();
                NodeT *vars();
                NodeT *expr();
                NodeT *N();
                NodeT *A();
                NodeT *M();
                NodeT *R();
                NodeT *stats();
                NodeT *mStat();
                NodeT *stat();
                NodeT *in();
                NodeT *out();
                NodeT *iffy();
                NodeT *loop();
                NodeT *assign();
                NodeT *label();
                NodeT *goTo();
                NodeT *RO();

                //Parser Function
                void nextScan();
                NodeT *createNode(string production_name);
                void parserError();

        public:
                Parser(string file_name);
                NodeT *parse();
};
#endif
