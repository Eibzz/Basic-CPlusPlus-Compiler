#include "parser.h"
#include "debug.h"
using namespace std;

int main() {

	string filename = "test.txt";
	ScannerClass scanner(filename);

	SymbolTableClass symbol;

	ParserClass parser = ParserClass(&scanner, &symbol);
	StartNode *s = parser.Start();
	MSG("Starting Interpretation...");
	//s->Interpret();
	//symbol.PrintTable();

	InstructionsClass machineCode;
	s->Code(machineCode);
	machineCode.Finish();
	//machineCode.PrintAllMachineCodes();

	machineCode.Execute();
	/*
	InstructionsClass machineCode;
	machineCode.PushValue(2);
	machineCode.PushValue(3);
	machineCode.PopPopAddPush();
	machineCode.PopAndWrite();
	machineCode.Finish();
	machineCode.PrintAllMachineCodes();
	machineCode.Execute();
	*/

	delete s;

	return 0;
}
