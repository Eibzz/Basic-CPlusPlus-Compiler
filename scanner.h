#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "StateMachine.h"
#include <iostream>
#include <fstream>

class ScannerClass {
	public:
		ScannerClass(string infile);
		~ScannerClass();
		TokenClass GetNextToken();
		TokenClass PeekNextToken();
		//TokenClass PeekNextToken();
		int mLineNumber;
		int mPos;
	private:
		ifstream mFin;
		

};

#endif