#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace a_lang{

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){

	std::cout << "ASTNode::nameAnalysis()\n";
	//throw new ToDoError("This function should have"
		//"been overriden in the subclass!");
	return false;
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;

	// Create a global scope
	symTab->NewScope();

	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	//std::cerr << "I'm in VarDeclNode throwing an error."
	//	<< " use GDB or this will look like a segfault!" << std::endl;
	//std::cerr << std::flush;
	//throw new ToDoError("[DELETE ME] I'm a varDecl"
	//	" you should add the information from my"	
	//	" subtree to the symbolTable as a new"	
	//	" entry in the current scope table"
	//);
	std::string name = myID->getName();
	SemSymbol* varDeclSymbol = new SemSymbol(SYMBOLTYPE::VARIABLE, myID->getName());
	varDeclSymbol->AddVariableDeclType(myType);
	if (symTab->SymbolExists(name))
	{
		// handle the fact that a symbol with this name is already declared
	}
	myID->attachSymbol(varDeclSymbol);
	symTab->AddSymbolCurrentScope(varDeclSymbol);

	return nameAnalysisOk;
}

bool ClassDefnNode::nameAnalysis(SymbolTable *symTab)
{
    bool nameAnalysisOk = true;

	// DO we create a new scope for inside the class body?
	return nameAnalysisOk;
}

bool CallExpNode::nameAnalysis(SymbolTable *symTab)
{
	printf("CallExpNode()\n");
    bool nameAnalysisOk = true;
	if(!myCallee->isDeclared(symTab))
	{
		printf("was not declared\n");
		return false; // ERROR use before decl
	}
	printf("Was declared callexpnode\n");
	return true;
}


/*
	Check if the symbol has been declared, and if so bind it
*/
bool MemberFieldExpNode::isDeclared(SymbolTable *symTab)
{
	auto name = myField->getName();
	if(!symTab->SymbolExists(name))
		return false;
	auto sym = symTab->GetSymbol(name);
	myField->attachSymbol(sym);	
	return true;
}

bool IDNode::isDeclared(SymbolTable *symTab)
{
    auto name = getName();
	if(!symTab->SymbolExists(name))
		return false;
	auto sym = symTab->GetSymbol(name);
	attachSymbol(sym);
	return true;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	//throw new ToDoError("[DELETE ME] I'm an fnDecl."
	//	" you should add and make current a new"	
	//	" scope table for my body"
	//);
	std::cout << "FnDeclNode::nameAnalysis()\n";

	
	std::string name = myID->getName();
	SemSymbol* fnSymbol = new SemSymbol(SYMBOLTYPE::FUNCTION, name);
	fnSymbol->AddFunctionParameters(getFormals());
	fnSymbol->AddFunctionReturnType(myRetType);
	
	if (symTab->SymbolExists(name))
	{
		// handle the fact that a symbol with this name already exists
	}
	myID->attachSymbol(fnSymbol);
	// Add functionDecl to current scope
	symTab->AddSymbolCurrentScope(fnSymbol);
	
	// Create new scope for function body
	symTab->NewScope();
	
	for (auto it = myBody->begin(); it != myBody->end(); it++)
	{
		auto& stmt = *it;
		stmt->nameAnalysis(symTab);
	}

	symTab->EndScope();
	return nameAnalysisOk;
}

bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to IntTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	printf("HERE\n");
	return true;
}


}
