/* ====================================================================================================
# Author: Brandon London
# Course: Program Translation Techniques
# File Name: parser.cpp
# Date: 3/23/2021
==================================================================================================== */
#include "parser.h"

/* BNF GRAMMER
<program> ->  <vars> <block>
<block>   ->  { <vars> <stats> }
<vars>    ->  empty | declare Identifier :=  Integer ; <vars>
<expr>    ->  <N> - <expr>  | <N>
<N>       ->  <A> / <N> | <A> * <N> | <A>
<A>       ->  <M> + <A> | <M>
<M>       ->  * <M> | <R>
<R>       ->  ( <expr> ) | Identifier | Integer
<stats>   ->  <stat> <mStat>
<mStat>   ->  empty | <stat> <mStat>
<stat>    ->  <in> ; | <out> ; | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ; | <block>
<in>      ->  in Identifier
<out>     ->  out <expr>
<if>      ->  iffy [ <expr> <RO> <expr> ] then <stat>
<loop>    ->  loop [ <expr> <RO> <expr> ] <stat>
<assign>  ->  Identifier  := <expr>
<label>   ->  label Identifier
<goto>    ->  goto Identifier
<RO>      ->  < | << (two tokens) | > | >> (two tokens) | == (one token ==) | <> (two tokens)
*/

Parser::Parser(string file_name)
{
	//Check if the file is open and associated with the stream object
	string output_file_name = "filter-parser-temp.dat";
	scanner.filterScan(file_name, output_file_name);

	file.open(output_file_name.c_str());
	if (!file.is_open()) {
		cout << "CANT OPEN FILE" << endl;
		exit(EXIT_FAILURE);
	}

	EMPTY_TK.value = "EMPTY";
	scanner_status_code = 0;
	getline(file, file_input);
}


/* <program> -> <vars> <block> */
NodeT* Parser::program()
{
	NodeT* node = createNode("<program>");
	node->c1 = vars();
	if ((tk.id == keywordTk) && (keyword_map[tk.value] == "mainTk")) {
		nextScan();
		node->c2 = block();
	}
	else {
		expected_token.assign("main");
		parserError();
	}
	return node;
}

/* <block> -> { <vars> <stats> } */
NodeT* Parser::block()
{
	NodeT* node = createNode("<block>");

	if ((tk.id == keywordTk) && (keyword_map[tk.value] == "beginTk")) {	
		tk.blockNum += 1;
		nextScan();
		node->c1 = vars();
		node->c2 = stats();

		if ((tk.id == keywordTk) && (keyword_map[tk.value] == "endTk")) {
			tk.blockNum -= 1;
			nextScan();
			return node;
		}
		else {
			expected_token.assign("end");
			parserError();
		}
	}
	else {
		expected_token.assign("begin");
		parserError();
	}
	//ERROR
	exit(EXIT_FAILURE);
}

/* <vars> -> empty | declare Identifier := Integer ; <vars> */
NodeT* Parser::vars()
{
	NodeT* node = createNode("<vars>");

	if ((tk.id == keywordTk) && (keyword_map[tk.value] == "dataTk")) {
		nextScan();

		//Check if idTk
		if (tk.id == idTk) {
			node->tokens.push_back(tk);
			nextScan();

			//Check if colonEqualTk
			if ((tk.id == opTk) && (operator_map[tk.value] == "colonEqualTk")) {
				nextScan();

				//Check if intTk
				if (tk.id == intTk) {
					node->tokens.push_back(tk);
					nextScan();

					//Check if semiColonTk
					if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
						nextScan();

						node->c1 = vars();
						return node;
					}
					else {
						expected_token.assign(";");
						parserError();
					}
				}
				else {
					expected_token.assign("Integer");
					parserError();
				}
			}
			else {
				expected_token.assign(":=");
				parserError();
			}
		}
		else {
			expected_token.assign("Identifier");
			parserError();
		}
	}
	else {
		node->tokens.push_back(EMPTY_TK);
		return node;
	}
	//ERROR
	exit(EXIT_FAILURE);
}




/* <expr> -> <N> - <expr> | <N>*/
NodeT* Parser::expr()
{
	NodeT* node = createNode("<expr>");

	node->c1 = N();

	//Check if minusTk
	if ((tk.id == opTk) && (operator_map[tk.value] == "minusTk")) {
		node->tokens.push_back(tk);
		nextScan();

		node->c2 = expr();
	}

	return node;
}


/* <A> -> <M> + <A> | <M>*/
NodeT* Parser::A()
{
	//Create the node <A>
	NodeT* node = createNode("<A>");
	node->c1 = M();
	//Check if plusTk
	if ((tk.id == opTk) && (operator_map[tk.value] == "plusTk")) {
		node->tokens.push_back(tk);
		nextScan();
		node->c2 = A();
	}
	return node;
}


/* <N> -> <A> / <N> | <A> * <N> | <A> */
NodeT* Parser::N()
{
	NodeT* node = createNode("<N>");
	node->c1 = A();
	//Check if opTk
	if (tk.id == opTk) {
		if (operator_map[tk.value] == "divideTk") {
			node->tokens.push_back(tk);  //Store divideTK 
			nextScan();
			node->c2 = N();
		}
		//Check if multiplyTk
		else if (operator_map[tk.value] == "multiplyTk") {
			node->tokens.push_back(tk);  //Store multiplyTK
			nextScan();
			node->c2 = N();
		}
	}
	return node;
}


/* <M> -> * <M> | <R> */
NodeT* Parser::M()
{
	NodeT* node = createNode("<M>");

	//Check if multiplyTk
	if ((tk.id == opTk) && (operator_map[tk.value] == "multiplyTk")) {
		node->tokens.push_back(tk);
		nextScan();

		node->c1 = M();
		return node;
	}

	node->c1 = R();
	return node;
}


/* <R> -> ( <expr> ) | Identifier | Integer */
NodeT* Parser::R()
{
	NodeT* node = createNode("<R>");

	//Check if leftParenTk
	if ((tk.id == opTk) && (operator_map[tk.value] == "leftParenTk")) {
		nextScan();

		node->c1 = expr();

		//Check if rightParenTk
		if ((tk.id == opTk) && (operator_map[tk.value] == "rightParenTk")) {
			nextScan();
			return node;
		}
		else {
			expected_token.assign(")");
			parserError();
		}
	}
	//Check if idTk
	else if (tk.id == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;
	}
	//Check if intTk
	else if (tk.id == intTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;
	}
	else {
		expected_token.assign("( or Identifier or Integer");
		parserError();
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}

NodeT* Parser::stat()
{
	NodeT* node = createNode("<stat>");

	if (tk.id == keywordTk) {
		if (keyword_map[tk.value] == "getterTk") {
			nextScan();

			node->c1 = in();
			//Check if semiColonTk
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
		}
		//Check if outterTk
		else if (keyword_map[tk.value] == "outterTk") {
			nextScan();

			node->c1 = out();

			//Check if semiColonTk
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
		}
		//Check iftk
		else if (keyword_map[tk.value] == "ifTk") {
			nextScan();

			node->c1 = iffy();

			//Check if semiColonTk
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
			return node;
		}
		//Check if loopTk
		else if (keyword_map[tk.value] == "loopTk") {
			nextScan();

			node->c1 = loop();

			//Check if semiColonTk
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
			return node;
		}

		else if (keyword_map[tk.value] == "assignTk") {

			nextScan();

			node->c1 = assign();

			//Check if semiColonTk
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
			return node;



		}
		else if (keyword_map[tk.value] == "voidTk") {
			nextScan();
			node->c1 = label();
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
			return node;
		}
		else if (keyword_map[tk.value] == "procTk") {
			nextScan();
			node->c1 = goTo();
			if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
				nextScan();
				return node;
			}
			else {
				expected_token.assign(";");
				parserError();
			}
			return node;
		}

		else if (keyword_map[tk.value] == "beginTk") {
			node->c1 = block();
			return node;
		}

		else {
			expected_token.assign("in or out or iffy or loop or label or goto or Identifier");
			parserError();
		}
	}
	else if (tk.id == idTk) {
		Token temp_tk = tk;
		nextScan();

		node->c1 = assign();
		node->c1->tokens.push_back(temp_tk);
		if ((tk.id == opTk) && (operator_map[tk.value] == "semiColonTk")) {
			nextScan();
			return node;
		}
		else {
			expected_token.assign("semiColonTk");
			parserError();
		}
	}
	else {
		node->c1 = block();
		return node;
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}


/* <stats> -> <stat> <mStat> */
NodeT* Parser::stats()
{
	//Create the node <stats>
	NodeT* node = createNode("<stats>");

	node->c1 = stat();
	node->c2 = mStat();

	return node;
}


/* <mStat> -> empty | <stat> <mStat> */
NodeT* Parser::mStat()
{
	NodeT* node = createNode("<mStat>");
	if (
		((tk.id == keywordTk) && (
			(keyword_map[tk.value] == "getterTk") || (keyword_map[tk.value] == "outterTk") ||
			(keyword_map[tk.value] == "ifTk") || (keyword_map[tk.value] == "procTk") || (keyword_map[tk.value] == "loopTk") || (keyword_map[tk.value] == "beginTk") ||
			(keyword_map[tk.value] == "voidTk") || (keyword_map[tk.value] == "assignTk") || (keyword_map[tk.value] == "labelTk") || (keyword_map[tk.value] == "gotoTk")
			)) ||
		((tk.id == opTk) && (operator_map[tk.value] == "leftCurlyTk")) ||
		(tk.id == idTk)
		) {
		node->c1 = stat();
		node->c2 = mStat();
		return node;
	}
	else {
		node->tokens.push_back(EMPTY_TK);
		return node;
	}
}




/* <in> -> in Identifier*/
NodeT* Parser::in()
{
	NodeT* node = createNode("<in>");

	//Check if idTk
	if (tk.id == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;
	}
	else {
		expected_token.assign("Identifier");
		parserError();
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}


/* <out> -> out <expr> */
NodeT* Parser::out()
{
	NodeT* node = createNode("<out>");

	node->c1 = expr();
	return node;
}


/* <iffy> -> iffy [ <expr> <RO> <expr> ] then <stat>*/
NodeT* Parser::iffy()
{
	NodeT* node = createNode("<iffy>");
	if ((tk.id == opTk) && (operator_map[tk.value] == "leftBracketTk")) {
		nextScan();
		node->c1 = expr();
		node->c2 = RO();
		node->c3 = expr();
		if ((tk.id == opTk) && (operator_map[tk.value] == "rightBracketTk")) {
			nextScan();
			if ((tk.id == keywordTk) && (keyword_map[tk.value] == "thenTk")) {
				nextScan();
				node->c4 = stat();
				return node;
			}
			else {
				expected_token.assign("then");
				parserError();
			}
		}
		else {
			expected_token.assign("]");
			parserError();
		}
	}
	else {
		expected_token.assign("[");
		parserError();
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}


/* <loop> -> loop [ <expr> <RO> <expr> ] <stat> */
NodeT* Parser::loop()
{
	NodeT* node = createNode("<loop>");
	if ((tk.id == opTk) && (operator_map[tk.value] == "leftBracketTk")) {
		nextScan();
		node->c1 = expr();
		node->c2 = RO();
		node->c3 = expr();
		if ((tk.id == opTk) && (operator_map[tk.value] == "rightBracketTk")) {
			nextScan();
			node->c4 = stat();
			return node;
		}
		else {
			expected_token.assign("]");
			parserError();
		}
	}
	else {
		expected_token.assign("[");
		parserError();
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}


/* <assign> -> Identifier := <expr> */
NodeT* Parser::assign()
{
	NodeT* node = createNode("<assign>");

	//Check if colonEqualTk

	if (tk.id == idTk) {
		node->tokens.push_back(tk);
		nextScan();

		if ((tk.id == opTk) && (operator_map[tk.value] == "colonEqualTk")) {
			nextScan();

			node->c1 = expr();
			return node;
		}
		else {
			expected_token.assign(":=");
			parserError();
		}
	}
	else {
		expected_token.assign("Identifier");
		parserError();

	}
	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}


/* <label> -> label Identifier*/
NodeT* Parser::label()
{
	NodeT* node = createNode("<label>");
	if (tk.id == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;
	}
	else {
		expected_token.assign("Identifier");
		parserError();
	}
	exit(EXIT_FAILURE);
}


/* <goto> -> goto Identifier */
NodeT* Parser::goTo()
{
	NodeT* node = createNode("<goTo>");
	if (tk.id == idTk) {
		node->tokens.push_back(tk);
		nextScan();
		return node;
	}
	else {
		expected_token.assign("Identifier");
		parserError();
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}


/* <RO> -> < | << (two tokens) | > | >> (two tokens) | == (one token ==) | <> (two tokens) */
NodeT* Parser::RO()
{
	NodeT* node = createNode("<RO>");
	if (tk.id == opTk) {
		if ((tk.id == opTk) && (operator_map[tk.value] == "modulusTk")) {
			node->tokens.push_back(tk);
			nextScan();

			if ((tk.id == opTk) && ((operator_map[tk.value] != "%"))) {
				expected_token.assign("%");
				parserError();
			}
			else {
				return node;
			}
		}
		//check for ==, =>, =<
		if ((tk.id == opTk) && (operator_map[tk.value] == "equalEqualTk")) {
			node->tokens.push_back(tk);
			nextScan();
			return node;
		}
		if ((tk.id == opTk) && (operator_map[tk.value] == "equalLessTk")) {

			node->tokens.push_back(tk);
			nextScan();
			return node;
		}
		if ((tk.id == opTk) && (operator_map[tk.value] == "equalGreaterTk")) {
			node->tokens.push_back(tk);
			nextScan();
			return node;
		}
		if ((tk.id == opTk) && (operator_map[tk.value] == "leftBracketTk")) {
			node->tokens.push_back(tk);
			nextScan();
			if ((tk.id == opTk) && (operator_map[tk.value] == "equalEqualTk")) {
				node->tokens.push_back(tk);
				nextScan();
				if ((tk.id == opTk) && (operator_map[tk.value] == "rightBracketTk")) {
					node->tokens.push_back(tk);
					nextScan();
					return node;
				}
				else
				{
					expected_token.assign("] , used to finish [==]");
					parserError();
				}

			}
			else {
				expected_token.assign("==, used as part of [==]");
				parserError();
			}
		}
		else {
			expected_token.assign(" => | =< | == | [==] | %");
			parserError();
		}
	}
	else {
		expected_token.assign(" => | =< | == | [==] | % ");
		parserError();
	}

	//An error has occur if code reach here
	exit(EXIT_FAILURE);
}




/* determine whether renew the current input line or use the current input line. Also determine if end of file has reach. */
void Parser::nextScan()
{
	do {
		if (scanner_status_code == -1) {
			if (!getline(file, file_input)) {
				if (file.eof()) {
					scanner.isCommentMode();
					scanner.invokeEOF(tk);
					return;
				}
			}
		}
		scanner_status_code = scanner.scan(file_input, tk);
		/* DEBUG ONLY
		if(scanner_status_code != -1) {
			scanner.tokenToString(tk);
		} */
	} while (scanner_status_code == -1);
}


/* create an individual node based on given production name label. */
NodeT* Parser::createNode(string production_name)
{
	NodeT* node = new NodeT;
	node->label = production_name;
	node->c1 = NULL;
	node->c2 = NULL;
	node->c3 = NULL;
	node->c4 = NULL;
	return node;
}

/* constructing the parse tree from the begin symbol. */
NodeT* Parser::parse()
{
	NodeT* root = NULL;
	nextScan();
	root = program();

	//Check the last token of the program return by the scanner is eofTk
	//If not eofTk, an error occur, display error message and exit with no recovery
	if (tk.id != eofTk) {
		expected_token.assign("eofTk");
		parserError();
		exit(EXIT_FAILURE);
	}
	else {
		cout << ">>: Parseing Complete " << endl;
		file.close();
		return root;
	}
}

/* print error message of the line number cursor position */
void Parser::parserError()
{
	cout << "[ERROR] parsing error at " << scanner.getScannerPosition() << " -> expected ["
		<< expected_token << "] but received [" << tk.value << "]" << endl;
	file.close();
	exit(EXIT_FAILURE);
}
