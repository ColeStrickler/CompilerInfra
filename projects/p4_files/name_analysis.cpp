#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "name_analysis.hpp"




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


	myType->DisableVoid();
	bool add = true;
	if(!myType->nameAnalysis(symTab))
	{
		add = false;
		nameAnalysisOk = false;
		Report::fatal(myID->pos(), "Invalid type in declaration");
	}

	std::string name = myID->getName();
	SemSymbol* varDeclSymbol = new SemSymbol(SYMBOLTYPE::VARIABLE, myID->getName());
	varDeclSymbol->AddVariableDeclType(myType);
	
	if (symTab->SymbolExistsInCurrentScope(name))
	{
		// handle already declared
		add = false;
		nameAnalysisOk = false;
		Report::fatal(myID->pos(), "Multiply declared identifier");
	}

	//myType->nameAnalysis(symTab);
	if (add)
		symTab->AddSymbolCurrentScope(varDeclSymbol);
	myID->attachSymbol(varDeclSymbol);
	

	return nameAnalysisOk;
}

bool ClassDefnNode::nameAnalysis(SymbolTable *symTab)
{
    bool nameAnalysisOk = true;


	// add class symbol to current scope
	std::string name = myID->getName();
	SemSymbol* classDeclSymbol = new SemSymbol(SYMBOLTYPE::CLASS, name);
	if (symTab->SymbolExistsInCurrentScope(name))
	{
		// handle already declared
		nameAnalysisOk = false;
		Report::fatal(pos(), "Multiply declared identifier");
	}
	myID->attachSymbol(classDeclSymbol);
	symTab->AddSymbolCurrentScope(classDeclSymbol);

	//symTab->NewScope();

	for (auto it = myMembers->begin(); it != myMembers->end(); it++)
	{
		if (!(*it)->nameAnalysis(symTab))
			nameAnalysisOk = false;
	}


	//symTab->EndScope(); 
	// DO we create a new scope for inside the class body?
	return nameAnalysisOk;
}

bool CallExpNode::nameAnalysis(SymbolTable *symTab)
{
	//printf("CallExpNode()\n");
    bool nameAnalysisOk = true;
	if(!myCallee->isDeclared(symTab))
	{
		throwUndeclared(myCallee->pos());
		nameAnalysisOk = false;// ERROR use before decl
	}


	for (auto it = myArgs->begin(); it != myArgs->end(); it++)
	{		
		(*it)->EnsureDecl();
		if (!(*it)->nameAnalysis(symTab))
		{
			nameAnalysisOk = false;
		}
	}

	//printf("Was declared callexpnode\n");
	return nameAnalysisOk;
}

bool CallStmtNode::nameAnalysis(SymbolTable *symTab)
{
    return myCallExp->nameAnalysis(symTab);
}


bool ClassTypeNode::nameAnalysis(SymbolTable *symTab)
{
    if (myID->isDeclared(symTab))
		return true;


	throwUndeclared(myID->pos());
	return false;
}

bool AssignStmtNode::nameAnalysis(SymbolTable *symTab)
{
	bool ok = true;
	
	myDst->EnsureDecl();
	
	if (!myDst->nameAnalysis(symTab))
	{
		ok = false;
		//printf("dst not decl\n");
	}

	mySrc->EnsureDecl();
	if (!mySrc->nameAnalysis(symTab))
	{
		//throwUndeclared(mySrc->pos());
		ok = false;
	}

    return ok;
}

bool MaybeStmtNode::nameAnalysis(SymbolTable *symTab)
{
	bool ok = true;
	if (!myDst->isDeclared(symTab))
	{
		throwUndeclared(myDst->pos());
		ok = false;
	}

	if (!mySrc1->nameAnalysis(symTab))
	{
		throwUndeclared(mySrc1->pos());
		ok = false;
	}
	if (!mySrc2->nameAnalysis(symTab))
	{
		throwUndeclared(mySrc2->pos());
		ok = false;
	}


    return ok;
}

/*
	Check if the symbol has been declared, and if so bind it
*/
bool MemberFieldExpNode::isDeclared(SymbolTable *symTab)
{
	bool ok = true;
	if(!myBase->isDeclared(symTab))
	{
		throwUndeclared(myBase->pos());
		ok = false;
	}

	auto name = myField->getName();
	if(!symTab->SymbolExists(name))
	{
		throwUndeclared(myField->pos());
		ok = false;
	}
	auto sym = symTab->GetSymbol(name);
	myField->attachSymbol(sym);	

	return ok;
}

bool IDNode::isDeclared(SymbolTable *symTab)
{
	
	bool ok = true;
    auto name = getName();
	if(!symTab->SymbolExists(name))
	{
		ok = false;
		if (ensureDecl)
		{
			throwUndeclared(pos());
		}
	}
		
	auto sym = symTab->GetSymbol(name);
	attachSymbol(sym);

	return ok;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	//throw new ToDoError("[DELETE ME] I'm an fnDecl."
	//	" you should add and make current a new"	
	//	" scope table for my body"
	//);


	
	std::string name = myID->getName();
	SemSymbol* fnSymbol = new SemSymbol(SYMBOLTYPE::FUNCTION, name);
	fnSymbol->AddFunctionParameters(getFormals());
	fnSymbol->AddFunctionReturnType(myRetType);
	
	if (symTab->SymbolExistsInCurrentScope(name))
	{
		// handle already declared
		nameAnalysisOk = false;
		Report::fatal(myID->pos(), "Multiply declared identifier");
	}
	myID->attachSymbol(fnSymbol);
	// Add functionDecl to current scope
	symTab->AddSymbolCurrentScope(fnSymbol);
	
	// Create new scope for function body
	symTab->NewScope();


	// add formalDecls to the new scope
	for (auto it = myFormals->begin(); it != myFormals->end(); it++)
	{
		auto f = (*it);
		auto type = f->getTypeNode();
		type->DisableVoid();
		bool add = true;
		auto formalID = f->ID();
		if(!type->nameAnalysis(symTab))
		{
			add = false;
			nameAnalysisOk = false;
			Report::fatal(formalID->pos(), "Invalid type in declaration");
		}

		
		std::string formalName = formalID->getName();
		if (formalID->isDeclared(symTab))
		{
			// throw previously declared
		}
		auto sym = new SemSymbol(SYMBOLTYPE::VARIABLE, formalName);
		sym->AddVariableDeclType(f->getTypeNode());
		if (add)
			symTab->AddSymbolCurrentScope(sym);
		formalID->attachSymbol(sym);
	}

	
	for (auto it = myBody->begin(); it != myBody->end(); it++)
	{
		auto stmt = *it;
		if(!stmt->nameAnalysis(symTab))
		{
			nameAnalysisOk = false;
			//printf("stmt not ok\n");
		}
	}

	symTab->EndScope();
	return nameAnalysisOk;
}




bool IntTypeNode::nameAnalysis(SymbolTable* symTab){
	// Name analysis may never even recurse down to IntTypeNode,
	// but if it does, just return true to indicate that 
	// name analysis has not failed, and add nothing to the symbol table
	//printf("HERE\n");
	return true;
}

bool VoidTypeNode::nameAnalysis(SymbolTable *symTab)
{
	if (disableVoid)
	{
		return false;
	}
	return true;
}
}

void a_lang::throwUndeclared(const Position* pos)
{
	//printf("here undecl %s\n", pos->span().c_str());
	Report::fatal(pos, "Undeclared identifier");
}
