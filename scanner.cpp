#include "scanner.h"

ScannerClass::ScannerClass(string infile) {
	mLineNumber = 0;
	mFin.open(infile);
}

ScannerClass::~ScannerClass() {
	mFin.close();
}

TokenClass ScannerClass::GetNextToken() {

	StateMachineClass stateMachine;
	TokenType curType;
	MachineState curState;
	string lexeme;
	char c;

	do {
		c = mFin.get();
		if(c == '\n') {
			mLineNumber++;
		}
		curState = stateMachine.UpdateState(c,curType);
		if(curState != START_STATE) {
			lexeme.push_back(c);
		}
	}
	while(curState != CANTMOVE_STATE);
	mFin.unget();
	lexeme.pop_back();
	if(c == '\n') {
		mLineNumber--;
	}
	TokenClass newToken(curType,lexeme);
	newToken.CheckReserved();
	if(curType == BAD_TOKEN) {
		cerr << "BAD_TOKEN Error on line " << mLineNumber << endl << 
			"Lexeme was :" << lexeme << endl;
		exit(1);
	}
	while(newToken.GetTokenType() == COMMENT_TOKEN) {
		lexeme = "";
		newToken = GetNextToken();
	}
	return newToken;
}

TokenClass ScannerClass::PeekNextToken() {

	streampos startPos = mFin.tellg();
	int startLine = mLineNumber;

	TokenClass newToken = GetNextToken();

	if(!mFin) {
		mFin.clear();
	}

	mFin.seekg(startPos);
	mLineNumber = startLine;

	return newToken;
}

/*

PeekNextToken
	int line = mLineNumber;
	streampos pos = mFin.tellg();
	Token tok = getnexttoken();
	mLineNumber = line;
	mFin.seekg(pos);
	return tok

bool Parser::Statement() {
	Token token = scanner.PeekNextToken();
	TokenType tt = token.GetTokenType();
	if (tt == INT_TOKEN) {
		DeclarationStatement();
		return true;
	}
	else if (tt == IDENTIFIER_TOKEN) {
		AssignmentStatement();
		return true;
	}
	else if (tt == COUT) {
		CoutStatement();
		return true;
	}
	else {
		return false;
	}
}

Parser::StatementGroup() {
	will call Statement until it returns false
}

<rule> -> TOKEN <Method> TOKEN
Match(TOKEN);
Method();
Match(TOKEN);

void Parser Term() {
	Term()	**causes infinite, left recursion**
	...
}

*** init linenumber to 1 ***

scanner::getnexttoken() {
	while(true) {
		c = mFin.get()
		if(c == '\n') {
			mLineNumber++;
			.
			.
			.
		}
		if(c == '\n') {
			mLineNumber--;
		}
		mFin.unget();
	}
}

How to handle b~tchy block comments:
[S] -- / --> [/] -- * --> [BLOCK1] -- * --> [BLOCK2] -- / --> [S]
[/] -- / --> [LINECOMMENT] -- '\n' --> [S]
[LINECOMMENT] -- 'EOF' --> [EOF]  and [LINECOMMENT] spins on 'not EOF or \n'
[S] spins on 'whitespace'
[BLOCK1] spins on 'not *'
[BLOCK2] goes to [BLOCK1] on 'not * or /' AND spins on '*'
*** lexeme = "" upon returning to [S] or going to [EOF] ***

*/