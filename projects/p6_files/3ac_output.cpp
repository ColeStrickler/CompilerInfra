#include "ast.hpp"

namespace a_lang{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

void FnDeclNode::to3AC(IRProgram * prog){
	prog->setGlobal(myID->getSymbol());
	auto this_func = prog->makeProc(myID->getName());

	// set quads for arguments
	size_t argnum = 1;
	for (auto& formal: *myFormals)
	{
		formal->to3AC(this_func);
		auto argQuad = new GetArgQuad(argnum, new SymOpd(formal->ID()->getSymbol(), formal->getTypeNode()->getType()->getSize()));
		this_func->addQuad(argQuad);
	}


	for (auto& stmt: *myBody)
	{
		stmt->to3AC(this_func);
	}
}

void FnDeclNode::to3AC(Procedure * proc){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A function declaration
	// never occurs within another function)
	throw new InternalError("FnDecl at a local scope");
}

void FormalDeclNode::to3AC(IRProgram * prog){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A formal never
	// occurs at global scope)
	throw new InternalError("Formal at a global scope");
}

void FormalDeclNode::to3AC(Procedure * proc)
{
	proc->gatherFormal(ID()->getSymbol());	
}

Opd * IntLitNode::flatten(Procedure * proc){
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(myNum), 8);
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * TrueNode::flatten(Procedure * proc){
	return new LitOpd("1", 1);
}

Opd * FalseNode::flatten(Procedure * proc){
	
	return new LitOpd("0", 1);
}

Opd * EhNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * CallExpNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * NegNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * NotNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * PlusNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * MinusNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * TimesNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * DivideNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * AndNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * OrNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * EqualsNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * LessNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * GreaterNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * LessEqNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	TODO(Implement me)
}

void AssignStmtNode::to3AC(Procedure * proc){
	
	auto dstSymbol = myDst->getSymbol();
	auto dstType = dstSymbol->getDataType();
	auto dst = proc->getSymOpd(dstSymbol);

	auto srcOpd = mySrc->flatten(proc);

	auto assignQuad = new AssignQuad(dst, srcOpd);
	proc->addQuad(assignQuad);
}

void MaybeStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void PostIncStmtNode::to3AC(Procedure * proc){
	auto locSym = myLoc->getSymbol();
	auto locOpd = proc->getSymOpd(locSym);
	auto sumQuad = new BinOpQuad(locOpd, BinOp::ADD64, locOpd, new LitOpd("1", 8));
	proc->addQuad(sumQuad);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	auto locSym = myLoc->getSymbol();
	auto locOpd = proc->getSymOpd(locSym);
	auto sumQuad = new BinOpQuad(locOpd, BinOp::SUB64, locOpd, new LitOpd("1", 8));
	proc->addQuad(sumQuad);
}

void ToConsoleStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void FromConsoleStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfElseStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void WhileStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void CallStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void ReturnStmtNode::to3AC(Procedure * proc){
	
	auto retValQuad = myExp->flatten(proc);
	auto setRet = new SetRetQuad(retValQuad);
	proc->addQuad(setRet);

	auto leaveLabel = proc->getLeaveLabel();
	auto goToLeave = new GotoQuad(leaveLabel);
	proc->addQuad(goToLeave);
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * sym = ID()->getSymbol();
	assert(sym != nullptr);
	proc->gatherLocal(sym);

	if (myInit != nullptr)
	{
		auto initOpd = myInit->flatten(proc);
		auto type = myType->getType();
		auto dstOpd = proc->getSymOpd(sym);

		auto initAssign = new AssignQuad(dstOpd, initOpd);
		proc->addQuad(initAssign);
	}

}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sym = ID()->getSymbol();
	assert(sym != nullptr);
	prog->setGlobal(sym);
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent)
Opd * IDNode::flatten(Procedure * proc){

	auto symbol = getSymbol();
	auto type = symbol->getDataType();
	return new SymOpd(symbol, type->getSize());
}

}
