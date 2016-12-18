#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

enum TokenType {
	// Reserved Words:
	VOID_TOKEN, MAIN_TOKEN, INT_TOKEN, COUT_TOKEN,
	// Relational Operators:
	LESS_TOKEN, LESSEQUAL_TOKEN, GREATER_TOKEN, GREATEREQUAL_TOKEN, EQUAL_TOKEN, NOTEQUAL_TOKEN, OR_TOKEN, AND_TOKEN, NOT_TOKEN,
	// Other Operators:
	INSERTION_TOKEN, ASSIGNMENT_TOKEN, PLUS_TOKEN, MINUS_TOKEN, TIMES_TOKEN, DIVIDE_TOKEN, COMMENT_TOKEN,
	// Other Characters:
	SEMICOLON_TOKEN, LPAREN_TOKEN, RPAREN_TOKEN, LCURLY_TOKEN, RCURLY_TOKEN, IF_TOKEN, WHILE_TOKEN,
	// Other Token Types:
	IDENTIFIER_TOKEN, INTEGER_TOKEN,
	BAD_TOKEN, ENDFILE_TOKEN
};

const string gTokenTypeNames[] = {
	"VOID", "MAIN", "INT", "COUT",
	"LESS", "LESSEQUAL", "GREATER", "GREATEREQUAL", "EQUAL", "NOTEQUAL", "OR", "AND", "NOT",
	"INSERTION", "ASSIGNMENT", "PLUS", "MINUS", "TIMES", "DIVIDE", "COMMENT",
	"SEMICOLON", "LPAREN", "RPAREN", "LCURLY", "RCURLY", "IF", "WHILE",
	"IDENTIFIER", "INTEGER", 
	"BAD", "ENDFILE"
};

class TokenClass
{
private:
	TokenType mType;
	string mLexeme;
public:
	TokenClass();

	TokenClass(TokenType type, const string & lexeme);

	TokenType GetTokenType() const { return mType; }

	const string & GetTokenTypeName() const 
		{ return gTokenTypeNames[mType]; }

	string GetLexeme() const { return mLexeme; }

	void CheckReserved();

};

ostream & operator<<(ostream & out, const TokenClass & tc); 

#endif

// gettype, gettypestring, settoken_type/lexeme/both, checkreserved,

//initialize all spaces in legel_move array of state machine to CANTMOVE

//add PURGATORY state for letters following an integer

//when returning to start state, clear lexeme

