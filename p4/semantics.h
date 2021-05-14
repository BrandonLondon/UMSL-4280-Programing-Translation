/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: semantics.h
# Date: 4/24/2021
==================================================================================================== */
#ifndef SEMANTICS_H
#define SEMANTICS_H
#include <fstream>       
#include <iostream>      
#include <string>       
#include <vector>       
#include "node.h"
#include "token.h"
using namespace std;

typedef struct symbol_table
{
	Token identifier;
	Token value;
	//Can add more expansion later...
} SymbolTable;

class Semantics : public Language
{
private:
	ofstream file;
	string output_file_name;

	SymbolTable st_holder;
	vector<SymbolTable> st;

	unsigned int current_temp_vars_num = 0;
	unsigned int carry_labels_num = 0;
	unsigned int current_labels_num = 0;

	void checkDeclare(Token id, Token val);
	void checkDefined(Token tk);
	void validate(NodeT* node);
	void generate(NodeT* node);

public:
	Semantics(string file_name);
	void codeGeneration(NodeT* node);
};

#endif
