#include "parser.h"
#include "debug.h"

ParserClass::ParserClass(ScannerClass * scanner, SymbolTableClass * symbol) {
	mScanner = scanner;
	mSymbol = symbol;
}

TokenClass ParserClass::Match(TokenType expectedType)
{
	TokenClass currentToken = mScanner->GetNextToken();
	if(currentToken.GetTokenType() != expectedType)
	{
		cerr << "Error in ParserClass::Match. " << endl;
		cerr << "Expected token type " << 
			expectedType << 
			", but got type " << currentToken.GetTokenTypeName() << endl;
		exit(1);
	}
	MSG("\tSuccessfully matched Token Type: " << 
		currentToken.GetTokenTypeName() << ". Lexeme: \"" << 
		currentToken.GetLexeme() << "\"");
	return currentToken; // the one we just processed
}

StartNode * ParserClass::Start() {
	ProgramNode *p = Program();
	Match(ENDFILE_TOKEN);
	StartNode *s = new StartNode(p);
	return s;
}

ProgramNode * ParserClass::Program() {
	Match(VOID_TOKEN);
	Match(MAIN_TOKEN);
	Match(LPAREN_TOKEN);
	Match(RPAREN_TOKEN);
	BlockNode *b = Block();
	ProgramNode *p = new ProgramNode(b);
	return p;
}

BlockNode * ParserClass::Block() {
	Match(LCURLY_TOKEN);
	StatementGroupNode * sg = StatementGroup();
	Match(RCURLY_TOKEN);
	BlockNode *b = new BlockNode(sg);
	return b;
}

IfNode * ParserClass::If() {
	Match(IF_TOKEN);
	Match(LPAREN_TOKEN);
	ExpressionNode *exp = Expression();
	Match(RPAREN_TOKEN);
	BlockNode *bn = Block();
	IfNode *ifn = new IfNode(exp, bn);
	return ifn;
}

WhileNode * ParserClass::While() {
	Match(WHILE_TOKEN);
	Match(LPAREN_TOKEN);
	ExpressionNode *exp = Expression();
	Match(RPAREN_TOKEN);
	BlockNode *bn = Block();
	WhileNode *wh = new WhileNode(exp, bn);
	return wh;
}

StatementGroupNode * ParserClass::StatementGroup() { 
	TokenType ptt = mScanner->PeekNextToken().GetTokenType();
	StatementGroupNode *sg = new StatementGroupNode();
	if(ptt == INT_TOKEN) {
		DeclarationStatementNode *ds = DeclarationStatement();
		sg = StatementGroup();
		sg->AddStatement(ds);
	}
	else if(ptt == IDENTIFIER_TOKEN) {
		AssignmentStatementNode *as = AssignmentStatement();
		sg = StatementGroup();
		sg->AddStatement(as);
	}
	else if(ptt == COUT_TOKEN) {
		CoutStatementNode *cs = CoutStatement();
		sg = StatementGroup();
		sg->AddStatement(cs);
	}
	else if(ptt == LCURLY_TOKEN) {
		BlockNode *bs = Block();
		sg = StatementGroup();
		sg->AddStatement(bs);
	}
	else if(ptt == IF_TOKEN) {
		IfNode *ifn = If();
		sg = StatementGroup();
		sg->AddStatement(ifn);
	}
	else if(ptt == WHILE_TOKEN) {
		WhileNode *wh = While();
		sg = StatementGroup();
		sg->AddStatement(wh);
	}
	else {}
	return sg;
}

DeclarationStatementNode * ParserClass::DeclarationStatement() {
	Match(INT_TOKEN);
	IdentifierNode *id = Identifier();
	//IdentifierNode *id = new IdentifierNode("test",mSymbol);		//change from 'test' and delete gettokens later
	//mScanner->GetNextToken();
	Match(SEMICOLON_TOKEN);
	DeclarationStatementNode *ds = new DeclarationStatementNode(id);
	return ds;
}

CoutStatementNode * ParserClass::CoutStatement() {
	Match(COUT_TOKEN);
	Match(INSERTION_TOKEN);
	ExpressionNode *exp = Expression();
	Match(SEMICOLON_TOKEN);

	CoutStatementNode *cs = new CoutStatementNode(exp);
	return cs;
}

AssignmentStatementNode * ParserClass::AssignmentStatement() {
	IdentifierNode *id = Identifier();
	Match(ASSIGNMENT_TOKEN);
	ExpressionNode *exp = Expression();
	Match(SEMICOLON_TOKEN);

	AssignmentStatementNode *asgn = new AssignmentStatementNode(id, exp);
	return asgn;
}

ExpressionNode * ParserClass::Expression()
{
	ExpressionNode * current = And();
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(	tt == OR_TOKEN)
	{
		Match(tt);
		current = new OrNode(current, And());
	}
	return current;
}

ExpressionNode * ParserClass::And() {
	ExpressionNode * current = Relational();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == AND_TOKEN)
		{
			Match(tt);
			current = new AndNode(current, Relational());
		}
		else
		{
			return current;
		}
	}
}

ExpressionNode * ParserClass::Relational() {
	ExpressionNode * current = PlusMinus();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == LESS_TOKEN) {
			Match(tt);
			current = new LessNode(current, PlusMinus());
		}
		else if(tt == LESSEQUAL_TOKEN) {
			Match(tt);
			current = new LessEqualNode(current, PlusMinus());
		}
		else if(tt == GREATER_TOKEN) {
			Match(tt);
			current = new GreaterNode(current, PlusMinus()); 
		}
		else if(tt == GREATEREQUAL_TOKEN) {
			Match(tt);
			current = new GreaterEqualNode(current, PlusMinus());
		}
		else if(tt == EQUAL_TOKEN) {
			Match(tt);
			current = new EqualNode(current, PlusMinus());
		}
		/*else if(tt == NOTEQUAL_TOKEN) {
			Match(tt);
			current = new NotEqualNode(current, PlusMinus());
		}*/
		else {
			return current;
		}

	}
}

ExpressionNode * ParserClass::PlusMinus() {
	ExpressionNode * current = TimesDivide();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == PLUS_TOKEN) {
			Match(tt);
			current = new PlusNode(current, TimesDivide());
		}
		else if(tt == MINUS_TOKEN) {
			Match(tt);
			current = new MinusNode(current, TimesDivide());
		}
		else {
			return current;
		}

	}
}

ExpressionNode * ParserClass::TimesDivide() {
	ExpressionNode * current = Not();
	while(true)
	{
		TokenType tt = mScanner->PeekNextToken().GetTokenType();
		if(tt == TIMES_TOKEN) {
			Match(tt);
			current = new TimesNode(current, Not());
		}
		else if(tt == DIVIDE_TOKEN) {
			Match(tt);
			current = new DivideNode(current, Not());
		}
		else {
			return current;
		}

	}
}

ExpressionNode * ParserClass::Not() {
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(tt == NOT_TOKEN) {
		Match(tt);
		NotNode * current = new NotNode(Not());
		return current;
	}
	else {return Factor();}
}

ExpressionNode * ParserClass::Factor() {
	TokenType tt = mScanner->PeekNextToken().GetTokenType();
	if(tt == IDENTIFIER_TOKEN) {
		ExpressionNode * current = Identifier();
		return current;
	}
	else if(tt == INTEGER_TOKEN) {
		ExpressionNode * current = Integer();
		return current;
	}
	else if(tt == LPAREN_TOKEN) {
		Match(tt);
		ExpressionNode * current = Expression();
		Match(RPAREN_TOKEN);
		return current;
	}
	else {
		cerr << "Factor error on line " << mScanner->mLineNumber;
		cerr << "Expected 'identifier', 'integer', or '(' token";
		exit(1);
	}
}


IdentifierNode * ParserClass::Identifier() {
	TokenClass t = Match(IDENTIFIER_TOKEN);
	string name = t.GetLexeme();
	IdentifierNode *id = new IdentifierNode(name, mSymbol);
	return id;
}

IntegerNode * ParserClass::Integer() {
	TokenClass t = Match(INTEGER_TOKEN);
	string value = t.GetLexeme();
	IntegerNode *en = new IntegerNode(stoi(value));
	return en;
}

/*
void IfNode:Interpret() {
	if(mExpressionTest.Evaluate()) {
		mStatementNode->Interpret()
	}
}

Priorities:
* /
+ - 
<= < > >= != ==
&&
||

Or -> And -> PlusMinus -> TimesDivide -> Factor (Ident, Int, Expr)

*/
