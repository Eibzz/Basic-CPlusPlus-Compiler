#include "symbol.h"
#include "debug.h"
#include <iostream>

SymbolTableClass::SymbolTableClass() {
}

bool SymbolTableClass::Exists(string s) {
	for(unsigned int i = 0; i < mVars.size(); i++) {
		if (mVars[i].mLable == s) {
			//MSG(s << " exists")
			return true;
		}
	}
	return false;
}

void SymbolTableClass::AddEntry(string s) {
	if(!Exists(s)) {
		Variable newVar;
		newVar.mLable = s;
		newVar.mValue = 0;
		mVars.push_back(newVar);
		//MSG("Added symbol: " << s);
	}
}

int SymbolTableClass::GetValue(string s) {
	for(unsigned int i = 0; i < mVars.size(); i++) {
		if (mVars[i].mLable == s) {
			//MSG("Retrieved value of " << mVars[i].mValue << " from " << s);
			return mVars[i].mValue;
		}
	}
	//MSG("Failed to retrieve value. Variable " << s << " does not exist.");
	return -1;
}

void SymbolTableClass::SetValue(string s, int v) {
	for(unsigned int i = 0; i < mVars.size(); i++) {
		if (mVars[i].mLable == s) {
			mVars[i].mValue = v;
			//MSG("Set value of " << s << " to " << v);
			return;
		}
	}
	//MSG("Failed to set value. Variable " << s << " does not exist.");
}

//probably should've done this one first... woops.
//oh well, helps with 'item !exists cases'
int SymbolTableClass::GetIndex(string s) {
	for(unsigned int i = 0; i < mVars.size(); i++) {
		if (mVars[i].mLable == s) {
			return i;
			//MSG("Retrieved index for " << s << " at " << i);
		}
	}
	return -1;
}

int SymbolTableClass::GetCount(){
	return mVars.size();
}

void SymbolTableClass::PrintTable() {
	MSG("PRINTING SYMBOL TABLE: ");
	for(unsigned int i=0; i<mVars.size(); i++) {
		MSG(mVars[i].mLable << " " << mVars[i].mValue);
	}
}