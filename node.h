#ifndef __NODE_H__
#define __NODE_H__

#include "symbol.h"
#include "instructions.h"
#include <iostream>
using namespace std;
/*
these are from Symbol.h
-----------------------
#include <vector>
#include <string>
using namespace std;
*/

class Node;
class StartNode;
class ProgramNode;
class BlockNode;
class StatementGroupNode;
class StatementNode;
class DeclarationStatementNode;
class AssignmentStatementNode;
class CoutStatementNode;
class ExpressionNode;
class IntegerNode;
class IdentifierNode;
class BinaryOperatorNode;
class PlusNode;
class OrNode;
class AndNode;
class IfNode;
class WhileNode;
class NotNode;


class Node { 
public:
	virtual ~Node();
	virtual void Interpret() = 0;
	virtual void Code(InstructionsClass &machineCode) = 0;
};

class StartNode : public Node {
public:
	StartNode(ProgramNode *program);
	~StartNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	ProgramNode * mProgram;
};

class ProgramNode : public Node {
public:
	ProgramNode(BlockNode *block);
	~ProgramNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	BlockNode * mBlock;
};

class StatementNode : public Node {
public:
	StatementNode();
};


class BlockNode : public StatementNode {
public:
	BlockNode(StatementGroupNode *statementgroup);
	~BlockNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	StatementGroupNode * mStatementGroup;
};

class IfNode : public StatementNode {
public:
	IfNode(ExpressionNode *expression, BlockNode *block);
	~IfNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	ExpressionNode * mExpression;
	BlockNode * mBlock;
};

class WhileNode : public StatementNode {
public:
	WhileNode(ExpressionNode *expression, BlockNode *block);
	~WhileNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	ExpressionNode * mExpression;
	BlockNode * mBlock;
};

class StatementGroupNode : public Node {
public:
	StatementGroupNode();
	~StatementGroupNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
	void AddStatement(StatementNode *sn);
private:
	vector<StatementNode*> mStatements;
};


class DeclarationStatementNode : public StatementNode {
public:
	DeclarationStatementNode(IdentifierNode *identifier);
	~DeclarationStatementNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	IdentifierNode * mIdentifier;
};


class CoutStatementNode : public StatementNode {
public:
	CoutStatementNode(ExpressionNode *expression);
	~CoutStatementNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	ExpressionNode * mExpression; 
};


class AssignmentStatementNode : public StatementNode {
public:
	AssignmentStatementNode(IdentifierNode *identifier, ExpressionNode *expression);
	~AssignmentStatementNode();
	void Interpret();
	virtual void Code(InstructionsClass &machineCode);
private:
	IdentifierNode * mIdentifier;
	ExpressionNode * mExpression;
};


class ExpressionNode {
public:
	virtual ~ExpressionNode();
	virtual int Evaluate() = 0;
	virtual void CodeEvaluate(InstructionsClass &machineCode) = 0;
};


class IntegerNode : public ExpressionNode {
public:
	IntegerNode(int n);
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
private:
	int mValue;
};


class IdentifierNode : public ExpressionNode {
public:
	IdentifierNode(string name, SymbolTableClass * table);
	void DeclareVariable();
	void SetValue(int v);
	int GetIndex();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
private:
	string mName;
	SymbolTableClass * mTable;
};


class BinaryOperatorNode : public ExpressionNode {
public:
	BinaryOperatorNode(ExpressionNode *left, ExpressionNode *right);
	~BinaryOperatorNode();
	ExpressionNode * mExpLeft;
	ExpressionNode * mExpRight;
};

class NotNode : public ExpressionNode {
public:
	NotNode(ExpressionNode *exp);
	~NotNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
private:
	ExpressionNode *mExpression;
};


// Math Nodes
class PlusNode : public BinaryOperatorNode {
public:
	PlusNode(ExpressionNode *left, ExpressionNode *right);
	~PlusNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class MinusNode : public BinaryOperatorNode {
public:
	MinusNode(ExpressionNode *left, ExpressionNode *right);
	~MinusNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class TimesNode : public BinaryOperatorNode {
public:
	TimesNode(ExpressionNode *left, ExpressionNode *right);
	~TimesNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};


class DivideNode : public BinaryOperatorNode {
public:
	DivideNode(ExpressionNode *left, ExpressionNode *right);
	~DivideNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

// Comparison Nodes
class LessNode : public BinaryOperatorNode {
public:
	LessNode(ExpressionNode *left, ExpressionNode *right);
	~LessNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class LessEqualNode : public BinaryOperatorNode {
public:
	LessEqualNode(ExpressionNode *left, ExpressionNode *right);
	~LessEqualNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class GreaterNode : public BinaryOperatorNode {
public:
	GreaterNode(ExpressionNode *left, ExpressionNode *right);
	~GreaterNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class GreaterEqualNode : public BinaryOperatorNode {
public:
	GreaterEqualNode(ExpressionNode *left, ExpressionNode *right);
	~GreaterEqualNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class EqualNode : public BinaryOperatorNode {
public:
	EqualNode(ExpressionNode *left, ExpressionNode *right);
	~EqualNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class NotEqualNode : public BinaryOperatorNode {
public:
	NotEqualNode(ExpressionNode *left, ExpressionNode *right);
	~NotEqualNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class OrNode : public BinaryOperatorNode {
public:
	OrNode(ExpressionNode *left, ExpressionNode *right);
	~OrNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};

class AndNode : public BinaryOperatorNode {
public:
	AndNode(ExpressionNode *left, ExpressionNode *right);
	~AndNode();
	int Evaluate();
	virtual void CodeEvaluate(InstructionsClass &machineCode);
};


#endif
