/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: semantics.h
# Date: 4/24/2021
==================================================================================================== */
#ifndef SEMANTICS_H
#define SEMANTICS_H
#include <iostream>      
#include <string>        
#include <vector>        
#include "node.h"
#include "token.h"
using namespace std;

typedef struct symbol_table
{
	Token identifier;
	//Token value;
	//Can add more expansion later...
} SymbolTable;

class Semantics
{
	private:
		SymbolTable st_holder;
		vector<SymbolTable> st;

		void checkDeclare(Token tk);
		void checkDefined(Token tk);

	public:
		void validate(NodeT *node);
};

#endif

