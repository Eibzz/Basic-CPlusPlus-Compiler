#include "token.h"

TokenClass::TokenClass() 
	: mType(BAD_TOKEN), mLexeme("")
{
}

TokenClass::TokenClass(TokenType type, const string & lexeme)
	: mType(type), mLexeme(lexeme)
{
}

void TokenClass::CheckReserved() {
	if(mLexeme.compare("void") == 0) { mType = VOID_TOKEN; }
	else if(mLexeme.compare("main") == 0) { mType = MAIN_TOKEN; }
	else if(mLexeme.compare("int") == 0) { mType = INT_TOKEN; }
	else if(mLexeme.compare("cout") == 0) { mType = COUT_TOKEN; }
	else if(mLexeme.compare("if") == 0) { mType = IF_TOKEN; }
	else if(mLexeme.compare("while") == 0) { mType = WHILE_TOKEN; }
}

ostream & operator<<(ostream & out, const TokenClass & tc) {
	out << tc.GetTokenTypeName() << " " << tc.GetLexeme();
	return out;
}

