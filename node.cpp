#include "node.h"
#include "debug.h"

//NODE
Node::~Node() {
	//MSG("Destroyed Node");
	//empty
}

//START NODE
StartNode::StartNode(ProgramNode *program) {
	MSG("Start node initialized");
	mProgram = program;
}

StartNode::~StartNode() {
	MSG("Destroyed Start node");
	delete mProgram;
}

void StartNode::Interpret() {
	mProgram->Interpret();
}

void StartNode::Code(InstructionsClass &machineCode) {
	mProgram->Code(machineCode);
}

//PROGRAM NODE
ProgramNode::ProgramNode(BlockNode *block) {
	MSG("Program node initialized");
	mBlock = block;
}

ProgramNode::~ProgramNode() {
	MSG("Destroyed Program node");
	delete mBlock;
}

void ProgramNode::Interpret() {
	mBlock->Interpret();
}

void ProgramNode::Code(InstructionsClass &machineCode) {
	mBlock->Code(machineCode);
}

//BLOCK NODE
BlockNode::BlockNode(StatementGroupNode *statementgroup) {
	MSG("Block node initialized");
	mStatementGroup = statementgroup;
}

BlockNode::~BlockNode() {
	MSG("Destroyed Block node");
	delete mStatementGroup;
}

void BlockNode::Interpret() {
	mStatementGroup->Interpret();
}

void BlockNode::Code(InstructionsClass &machineCode) {
	mStatementGroup->Code(machineCode);
}

//IF NODE
IfNode::IfNode(ExpressionNode *expression, BlockNode *block) {
	mExpression = expression;
	mBlock = block;
}

IfNode::~IfNode() {
	MSG("Destroyed If node");
	delete mExpression;
	delete mBlock;
}

void IfNode::Interpret() {
	if(mExpression->Evaluate()) {
		mBlock->Interpret();
	}
}

/*
if {
	code.evaluate
	skipifzero
	jumpaddr
	save addr1
	statement.code
	save addr2

	mcode.setoffset(jumpaddr, addr2-addr1)
}
*/
void IfNode::Code(InstructionsClass &machineCode) {
	mExpression->CodeEvaluate(machineCode);
	machineCode.SkipIfZeroStack();
	unsigned char * jumpaddr = machineCode.GetAddress();
	machineCode.PushValue(0);
	unsigned char * addr1 = machineCode.GetAddress();
	mBlock->Code(machineCode);
	unsigned char * addr2 = machineCode.GetAddress();
	machineCode.SetOffset(jumpaddr, addr1-addr2);
	machineCode.PrintAllMachineCodes();
}

//WHILE NODE
WhileNode::WhileNode(ExpressionNode *expression, BlockNode *block) {
	mExpression = expression;
	mBlock = block;
}

WhileNode::~WhileNode() {
	MSG("Destroyed While node");
	delete mExpression;
	delete mBlock;
}

void WhileNode::Interpret() {
	while(mExpression->Evaluate()) {
		mBlock->Interpret();
	}
}

/*
while {
	addr0 = getaddress()
	code.evaluate
	skipifzero
	jumpaddr
	save addr1
	statement.code
	jump
	jumpaddr2
	save addr 2

	mcode.setoffset(jumpaddr, addr2-addr1)
	mcode.setoffest(jumpaddr2,addr0-addr2)
}
*/
void WhileNode::Code(InstructionsClass &machineCode) {
	unsigned char * addr0 = machineCode.GetAddress();
	mExpression->CodeEvaluate(machineCode);
	machineCode.SkipIfZeroStack();
	unsigned char * jumpaddr = machineCode.GetAddress();
	machineCode.PushValue(0);
	unsigned char * addr1 = machineCode.GetAddress();
	mBlock->Code(machineCode);
	machineCode.Jump();
	unsigned char * jumpaddr2 = machineCode.GetAddress();
	machineCode.PushValue(0);
	unsigned char * addr2 = machineCode.GetAddress();

	machineCode.SetOffset(jumpaddr, addr2-addr1);
	machineCode.SetOffset(jumpaddr2, addr0-addr2);
}

//STATEMENT_GROUP NODE
StatementGroupNode::StatementGroupNode() {
	MSG("StatementGroup node initialized");
}

StatementGroupNode::~StatementGroupNode() {
	for(unsigned int i = 0; i < mStatements.size(); i++) {
		MSG("Destroyed Statement node within StatementGroup node");
		delete mStatements[i];
	}
	MSG("Destroyed StatementGroup node");
}

void StatementGroupNode::AddStatement(StatementNode *sn) {
	MSG("Added statement node to statement group");
	mStatements.push_back(sn);
}

void StatementGroupNode::Interpret() {
	for(int i = mStatements.size()-1; i > -1; i--) {
		MSG("Interpretting Statements within StatementGroup node");
		mStatements[(unsigned)i]->Interpret();
	}
}

void StatementGroupNode::Code(InstructionsClass &machineCode) {
	for(int i = mStatements.size()-1; i > -1; i--) {
		MSG("Interpretting Statements within StatementGroup node");
		mStatements[(unsigned)i]->Code(machineCode);
	}
}

//STATEMENT NODE
StatementNode::StatementNode() {
	//MSG("Initialized Statement node");
}

//DECLARATION_STATEMENT NODE
DeclarationStatementNode::DeclarationStatementNode(IdentifierNode *identifier) {
	MSG("DeclarationStatement node initialized");
	mIdentifier = identifier;
}

DeclarationStatementNode::~DeclarationStatementNode() {
	MSG("Destroyed DeclarationStatement node");
	delete mIdentifier;
}

void DeclarationStatementNode::Interpret() {
	MSG("Interpretting Declaration");
	mIdentifier->DeclareVariable();
}

void DeclarationStatementNode::Code(InstructionsClass &machineCode) {
	mIdentifier->DeclareVariable();
}

//ASSIGNMENT_STATEMENT NODE
AssignmentStatementNode::AssignmentStatementNode(IdentifierNode *identifier, ExpressionNode *expression) {
	MSG("AssignmentStatement node initialized");
	mIdentifier = identifier;
	mExpression = expression;
}

AssignmentStatementNode::~AssignmentStatementNode() {
	delete mExpression;
	delete mIdentifier;
}

void AssignmentStatementNode::Interpret() {
	MSG("Interpretting AssignmentStatement");
	int v = mExpression->Evaluate();
	mIdentifier->SetValue(v);
}

void AssignmentStatementNode::Code(InstructionsClass &machineCode) {
	mExpression->CodeEvaluate(machineCode);
	int index = mIdentifier->GetIndex();
	machineCode.PopAndStore(index);
}


//COUT_STATEMENT NODE
CoutStatementNode::CoutStatementNode(ExpressionNode *expression) {
	MSG("CoutStatement node initialized");
	mExpression = expression;
}

CoutStatementNode::~CoutStatementNode() {
	MSG("Destroyed Cout node");
	delete mExpression;
}

void CoutStatementNode::Interpret() {
	MSG("Interpretting CoutStatement as: " << mExpression->Evaluate());
}

void CoutStatementNode::Code(InstructionsClass &machineCode) {
	mExpression->CodeEvaluate(machineCode);
	machineCode.PopAndWrite();
}

//EXPRESSION NODE
ExpressionNode::~ExpressionNode() {
	//MSG("Destroyed Expression node.");
	//empty
}

//INTEGER NODE
IntegerNode::IntegerNode(int n) : mValue(n) {
	MSG("Integer node initialized");
}

int IntegerNode::Evaluate() {
	MSG("Integer node evaluates to " << mValue);
	return mValue;
}

void IntegerNode::CodeEvaluate(InstructionsClass &machineCode) {
	machineCode.PushValue(mValue);
}

//IDENTIFIER NODE
IdentifierNode::IdentifierNode(string name, SymbolTableClass * table) {
	mTable = table;
	MSG("Identifier node initialized");
	mName = name;
}

void IdentifierNode::DeclareVariable() {
	MSG("Added symbol '" << mName << "' to table");
	mTable->AddEntry(mName);
}

void IdentifierNode::SetValue(int v) {
	MSG("Set identifier node '" << mName << "' to " << v);
	mTable->SetValue(mName,v);
}

int IdentifierNode::GetIndex() {
	MSG("Identifier node '" << mName << "' is at index " << mTable->GetIndex(mName));
	return mTable->GetIndex(mName);
}

int IdentifierNode::Evaluate() {
	MSG("Identifier node '" << mName << "' evaluates to " << mTable->GetValue(mName));
	return mTable->GetValue(mName);
}

void IdentifierNode::CodeEvaluate(InstructionsClass &machineCode) {
	machineCode.PushValue(Evaluate());
}

//BINARY_OPERATOR NODE
BinaryOperatorNode::BinaryOperatorNode(ExpressionNode *left, ExpressionNode *right) {
	mExpLeft = left;
	mExpRight = right;
	MSG("BinaryOperator node initialized");
}

BinaryOperatorNode::~BinaryOperatorNode() {
	delete mExpLeft;
	delete mExpRight;
	//MSG("BinaryOperatorNode destroyed");
}

//NOT NODE
NotNode::NotNode(ExpressionNode *exp)
{
	mExpression = exp;
	MSG("Not node initialized.");
}
	
NotNode::~NotNode() {
	delete mExpression;
	MSG("Plus node destroyed.");
}

int NotNode::Evaluate() {
	int res = mExpression->Evaluate();
	if(res) {
		return 0;
	}
	else {
		return 1;
	}
	MSG("Not node evaluated as " << res);
}

void NotNode::CodeEvaluate(InstructionsClass &machineCode) {
	//nope
}

//PLUS NODE
PlusNode::PlusNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Plus node initialized.");
}
	
PlusNode::~PlusNode() {
	MSG("Plus node destroyed.");
}

int PlusNode::Evaluate() {
	int res = mExpLeft->Evaluate() + mExpRight->Evaluate();
	MSG("Plus node evaluated to " << res);
	return res;
}

void PlusNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopAddPush();
}

//MINUS NODE
MinusNode::MinusNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Minus node initialized.");
}

MinusNode::~MinusNode() {
	MSG("Minus node destroyed.");
}

int MinusNode::Evaluate() {
	int res = mExpLeft->Evaluate() - mExpRight->Evaluate();
	MSG("Minus node evaluated to " << res);
	return res;
}

void MinusNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopSubPush();
}

//TIMES NODE
TimesNode::TimesNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Times node initialized.");
}

TimesNode::~TimesNode() {
	MSG("Times node destroyed.");
}
	
int TimesNode::Evaluate() {
	int res = mExpLeft->Evaluate() * mExpRight->Evaluate();
	MSG("Times node evaluated to " << res);
	return res;
}

void TimesNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopMulPush();
}

//DIVIDE NODE
DivideNode::DivideNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Divide node initialized.");
}

DivideNode::~DivideNode() {
	MSG("Divide node destroyed.");
}
	
int DivideNode::Evaluate() {
	int res = mExpLeft->Evaluate() / mExpRight->Evaluate();
	MSG("Divide node evaluated to " << res);
	return res;
}

void DivideNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopDivPush();
}

//LESS NODE
LessNode::LessNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Less node initialized.");
}

LessNode::~LessNode() {
	MSG("Less node destroyed.");
}

int LessNode::Evaluate() {
	int res = mExpLeft->Evaluate() < mExpRight->Evaluate();
	MSG("Less node evaluated to " << res);
	return res;
}

void LessNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopLessPush();
}

//LESS_EQUAL NODE
LessEqualNode::LessEqualNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("LessEqual node initialized.");
}

LessEqualNode::~LessEqualNode() {
	MSG("LessEqual node destroyed.");
}

int LessEqualNode::Evaluate() {
	int res = mExpLeft->Evaluate() <= mExpRight->Evaluate();
	MSG("LessEqual node evaluated to " << res);
	return res;
}

void LessEqualNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopLessEqualPush();
}

//GREATER NODE
GreaterNode::GreaterNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Greater node initialized.");
}

GreaterNode::~GreaterNode() {
	MSG("Greater node destroyed.");
}

int GreaterNode::Evaluate() {
	int res = mExpLeft->Evaluate() > mExpRight->Evaluate();
	MSG("Greater node evaluated to " << res);
	return res;
}

void GreaterNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopGreaterPush();
}

//GREATER_EQUAL NODE
GreaterEqualNode::GreaterEqualNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("GreaterEqual node initialized.");
}

GreaterEqualNode::~GreaterEqualNode() {
	MSG("GreaterEqual node destroyed.");
}

int GreaterEqualNode::Evaluate() {
	int res = mExpLeft->Evaluate() >= mExpRight->Evaluate();
	MSG("GreaterEqual node evaluated to " << res);
	return res;
}

void GreaterEqualNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopGreaterEqualPush();
}

//EQUAL NODE
EqualNode::EqualNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Equal node initialized.");
}

EqualNode::~EqualNode() {
	MSG("Equal node destroyed.");
}

int EqualNode::Evaluate() {
	int res = mExpLeft->Evaluate() == mExpRight->Evaluate();
	MSG("Equal node evaluated to " << res);
	return res;
}

void EqualNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopEqualPush();
}

//NOT_EQUAL NODE
NotEqualNode::NotEqualNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("NotEqual node initialized.");
}

NotEqualNode::~NotEqualNode() {
	MSG("NotEqual node destroyed.");
}

int NotEqualNode::Evaluate() {
	int res = mExpLeft->Evaluate() != mExpRight->Evaluate();
	MSG("NotEqual node evaluated to " << res);
	return res;
}

void NotEqualNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopNotEqualPush();
}

//OR NODE
OrNode::OrNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("Or node initialized.");
}

OrNode::~OrNode() { 
	MSG("Or node destroyed.");
}

int OrNode::Evaluate() {
	int res = mExpLeft->Evaluate() || mExpRight->Evaluate();
	MSG("NotEqual node evaluated to " << res);
	return res;
}

void OrNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopOrPush();
}

//AND NODE
AndNode::AndNode(ExpressionNode *left, ExpressionNode *right)
	: BinaryOperatorNode(left, right)
{
	MSG("NotEqual node initialized.");
}

AndNode::~AndNode() {
	MSG("NotEqual node destroyed.");
}

int AndNode::Evaluate() {
	int res = mExpLeft->Evaluate() && mExpRight->Evaluate();
	MSG("NotEqual node evaluated to " << res);
	return res;
}

void AndNode::CodeEvaluate(InstructionsClass &machineCode) {
	mExpLeft->CodeEvaluate(machineCode);
	mExpRight->CodeEvaluate(machineCode);
	machineCode.PopPopAndPush();
}

/*

startnode* parser::start() {
	programnode * pn = program()
		//program makes block child and returns program
			//chain continues
	match(ENDFILE)
	startnode * sn = new startnode()
}

statementgroupnode* parser::statementgroup {
	statementgroupnode * sgn = new statementgroupnode()
	while(true) {
		statementnode * sn = statement()
		if(!sn) {
			break
		}
		sgn->addstatement(sn)
	}
	return sgn
}

statement* parser::statement {
	token tok = mScanner->peeknexttoken()
	tokentype tt = tok.gettokentype()
	if (tt == INT_TOKEN) {
		return declarationstatement()
		//return true
	}
	else if (tt == IDENTIFIER_TOKEN) {
		return assignmentstatement()
		//return true
	}
	. . .
	else { return false }
}

expressionnode* parser::plusminus() {
	expressionnode * curNode = timesdivide()
	while(true) {
		tokentype tt = mscanner->getnexttoken().gettokentype()
		if (tt == PLUS_TOKEN) {
			match(PLUS_TOKEN)
			expressionnode * rhs = timesdivide()
			curNode = new plusnode(curNode,rhs)
		}
		else if (tt == MINUS_TOKEN) {
			match(MINUS_TOKEN)
			expressionnode * rhs = timesdivide()
			curNode = new minusnode(curNode,rhs)
		}
		else {
			break
		}
	}
	return curNode
}

<Expression> -> <Identifier> | <Integer> | <Expression> + <Expression> | E - E | E * E | E > E | ...		** NO PRIORITY, DOESN'T WORK **

Priority {

expt	
/ *	
+ -	
< <= > >=
**
||

}

<Expression> -> <PlusMinus>
<PlusMinus> -> <TimesDivide> | <PlusMinus> + <TimesDivide> | <PlusMinus> - <TimesDivide>
<TimesDivide> -> <Factor> | <TimesDivide> * <Factor> | <TimesDivide> / <Factor>
// Allow more than one statement of a layer: <SameLayer> op <OneLayerDown>
// Allow one: <OneLayerDown> op <OneLayerDown>
// if adding expt layer, change TimesDivide's Factors to Exponents, and Exponents go to Factors
<Factor> -> <Integer> | <Identifier>

<Expression> -> <PlusMinus>
<PlusMinus> -> <TimesDivide><PlusMinusTail>
<PlusMinusTail> -> null | + <TimesDivide><PlusMinusTail> | - <TimesDivide><PlusMinusTail>
<TimesDivide> -> same pattern
<TimesDivideTail> -> same pattern

5*2
PlusMinus
TimesDivide null(PMTail)
Factor TDtail
Factor * TD null(TDTail)
Factor * Factor null(TDTail)
5*2

holy hell that's roundabout...



*/

