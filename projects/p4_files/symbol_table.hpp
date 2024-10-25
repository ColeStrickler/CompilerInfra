#ifndef A_LANG_SYMBOL_TABLE_HPP
#define A_LANG_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include "ast.hpp"
#include <iostream>


using namespace std;

namespace a_lang {


enum SYMBOLTYPE
{
	VARIABLE,
	FUNCTION,
	CLASS
};

enum VARTYPE
{
	BOOL,
	INT
};


//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols 
// exist for the lifetime of a scope in the 
// symbol table. 
class SemSymbol {

public:
	SemSymbol(SYMBOLTYPE type, const std::string& symbolName) : m_SymbolName(symbolName), m_Type(type) 
	{

	}
	~SemSymbol();
	void AddFunctionParameters(const std::list<FormalDeclNode*>* formals);
	void AddFunctionReturnType(TypeNode* retType);
	void AddVariableDeclType(TypeNode* varType);
	//TODO add the fields that 
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
	std::string toString();
	std::string m_SymbolName;
private:
	SYMBOLTYPE m_Type;
	std::vector<std::string> m_FunctionParamFormals;
	std::string m_xType;
};

//A single scope. The symbol table is broken down into a 
// chain of scope tables, and each scope table holds 
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.

		void AddSymbol(SemSymbol* symbol);
		bool SymbolExists(const std::string& symbolName);
		
		SemSymbol* GetSymbol(const std::string& symbolName);

	private:
		std::unordered_map<std::string, SemSymbol *> symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc. 
		bool SymbolExistsInCurrentScope(const std::string& symbolName);
		bool AddSymbolCurrentScope(SemSymbol* sym);
		void NewScope();
		void EndScope();
		SemSymbol* GetSymbol(const std::string& symbolName);
		bool SymbolExists(const std::string& symbolName);
	private:
		std::vector<ScopeTable *> scopeTableChain;
};

	
}

#endif
