#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <string>
#include <vector>
using namespace std;

class SymbolTableClass {

	struct Variable {
		string mLable;
		int mValue;
	};

	public:
		SymbolTableClass();
		bool Exists(string s);
		void AddEntry(string s);
		int GetValue(string s);
		void SetValue(string s, int v);
		int GetIndex(string s);
		int GetCount();
		void PrintTable();

	private:
		vector<Variable> mVars;

};

#endif