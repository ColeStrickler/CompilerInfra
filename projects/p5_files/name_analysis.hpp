#ifndef A_LONG_NAME_ANALYSIS
#define A_LONG_NAME_ANALYSIS

#include "ast.hpp"

namespace a_lang{

class NameAnalysis{
public:

	static NameAnalysis * build(ProgramNode * astIn){
		NameAnalysis * nameAnalysis = new NameAnalysis;
		SymbolTable * symTab = new SymbolTable();
		bool res = astIn->nameAnalysis(symTab);
		
		if (!res){ return nullptr; }

		nameAnalysis->ast = astIn;
		nameAnalysis->m_SymTab = symTab;
		return nameAnalysis;
	}
	ProgramNode * ast;
	SymbolTable* m_SymTab;

private:
	NameAnalysis(){
	}
};

}

#endif
