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
		argnum++;
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

Opd * CallExpNode::flatten(Procedure * proc){

	size_t argIndex = 1;
	for (auto& arg: *myArgs)
	{
		auto argOpd = arg->flatten(proc);
		auto setArgQuad = new SetArgQuad(argIndex, argOpd);
		proc->addQuad(setArgQuad);
		argIndex += 1;
	}


	auto callFnQuad = new CallQuad(myCallee->getSymbol());
	proc->addQuad(callFnQuad);


	auto typeSize = myCallee->getSymbol()->getDataType()->asFn()->getReturnType()->getSize();
	auto tmpQuad = proc->makeTmp(typeSize);


	auto getRetOpd = new GetRetQuad(tmpQuad);
	proc->addQuad(getRetOpd);
	return tmpQuad;
}

Opd * NegNode::flatten(Procedure * proc){
	
	auto expOpd = myExp->flatten(proc);
	auto tmpDst = proc->makeTmp(8);
	auto unaryNegQuad = new UnaryOpQuad(tmpDst, UnaryOp::NEG64, expOpd);
	proc->addQuad(unaryNegQuad);
	return tmpDst;
}

Opd * NotNode::flatten(Procedure * proc){
	auto expOpd = myExp->flatten(proc);
	auto tmpDst = proc->makeTmp(8);
	auto unaryNegQuad = new UnaryOpQuad(tmpDst, UnaryOp::NOT64, expOpd);
	proc->addQuad(unaryNegQuad);
	return tmpDst;
}

Opd * PlusNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::ADD64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * MinusNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::SUB64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * TimesNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::MULT64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * DivideNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::DIV64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * AndNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::AND64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * OrNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::OR64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * EqualsNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::EQ64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::NEQ64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * LessNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::LT64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * GreaterNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::GT64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * LessEqNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::LTE64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	auto opDa = myExp1->flatten(proc);
	auto opDb = myExp2->flatten(proc);

	auto tmpOpd = proc->makeTmp(8);
	auto binQuad = new BinOpQuad(tmpOpd, BinOp::GTE64, opDa, opDb);
	proc->addQuad(binQuad);
	return tmpOpd;
}

void AssignStmtNode::to3AC(Procedure * proc){
	
	auto dstSymbol = myDst->getSymbol();
	auto dstType = dstSymbol->getDataType();
	auto dst = proc->getSymOpd(dstSymbol);

	auto srcOpd = mySrc->flatten(proc);

	auto assignQuad = new AssignQuad(dst, srcOpd);
	proc->addQuad(assignQuad);
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

void ToConsoleStmtNode::to3AC(Procedure * proc)
{
	auto srcOpd = mySrc->flatten(proc);
	auto prog = proc->getProg();
	auto type = prog->nodeType(mySrc);
	auto toConQuad = new WriteQuad(srcOpd, type);
	proc->addQuad(toConQuad);
}

void FromConsoleStmtNode::to3AC(Procedure * proc)
{
	auto dstOpd = myDst->flatten(proc);
	auto prog = proc->getProg();
	auto type = prog->nodeType(myDst);
	auto fromConQuad = new ReadQuad(dstOpd, type);
	proc->addQuad(fromConQuad);
}


void MaybeStmtNode::to3AC(Procedure * proc){
	auto src1Opd = mySrc1->flatten(proc);
	auto src2Opd = mySrc2->flatten(proc);
	auto dstOpd = myDst->flatten(proc);
	// set up sources


	// call random
	auto randBoolSym = new FnSymbol("randBool", FnType::produce(nullptr, BasicType::produce(BOOL)));
	auto callRandBool = new CallQuad(randBoolSym);
	proc->addQuad(callRandBool);
	
	// get return of the random
	auto tmp = proc->makeTmp(8);
	auto getRet = new GetRetQuad(tmp);
	proc->addQuad(getRet);

	auto AfterTrueLabel = proc->makeLabel();
	auto AfterFalseLabel = proc->makeLabel();

	// if zero go to the assign src2
	auto ifZ = new IfzQuad(tmp, AfterTrueLabel);
	proc->addQuad(ifZ);
	
	// assign src1
	auto assignSrc1Quad = new AssignQuad(dstOpd, src1Opd);
	proc->addQuad(assignSrc1Quad);

	auto goToEndQuad = new GotoQuad(AfterFalseLabel);
	proc->addQuad(goToEndQuad);

	auto nopQ1 = new NopQuad();
	nopQ1->addLabel(AfterTrueLabel);
	proc->addQuad(nopQ1);
	auto assignSrc2Quad = new AssignQuad(dstOpd, src2Opd);
	proc->addQuad(assignSrc2Quad);

	auto nopQ2 = new NopQuad();
	nopQ2->addLabel(AfterFalseLabel);
	proc->addQuad(nopQ2);


}

Opd * EhNode::flatten(Procedure * proc){
	auto tmp = proc->makeTmp(8);
	auto randBoolSym = new FnSymbol("randBool", FnType::produce(nullptr, BasicType::produce(BOOL)));
	auto callRandBool = new CallQuad(randBoolSym);
	proc->addQuad(callRandBool);

	auto getRet = new GetRetQuad(tmp);
	proc->addQuad(getRet);

	return tmp;
}

void IfStmtNode::to3AC(Procedure * proc){
	
	auto condOpd = myCond->flatten(proc);
	auto afterLabel = proc->makeLabel();
	auto testTrue = new IfzQuad(condOpd, afterLabel);
	proc->addQuad(testTrue);

	for (auto& stmt: *myBody)
		stmt->to3AC(proc);

	auto nopQ = new NopQuad();
	nopQ->addLabel(afterLabel);
	proc->addQuad(nopQ);
	
}

void IfElseStmtNode::to3AC(Procedure * proc){
	auto condOpd = myCond->flatten(proc);
	auto afterLabel = proc->makeLabel();
	auto afterLabel2 = proc->makeLabel();
	auto testTrue = new IfzQuad(condOpd, afterLabel);
	proc->addQuad(testTrue);

	for (auto& stmt: *myBodyTrue)
		stmt->to3AC(proc);
	
	auto jQuad = new GotoQuad(afterLabel2);
	proc->addQuad(jQuad);

	auto nopQ = new NopQuad();
	nopQ->addLabel(afterLabel);
	proc->addQuad(nopQ);

	auto nopQ2 = new NopQuad();
	
	nopQ2->addLabel(afterLabel2);
	for (auto& stmt: *myBodyFalse)
		stmt->to3AC(proc);
	proc->addQuad(nopQ2);
	
}

void WhileStmtNode::to3AC(Procedure * proc){
	
	
	auto startLabel = proc->makeLabel();
	auto afterLabel = proc->makeLabel();
	auto nopQuadStart = new NopQuad();
	auto nopQuadEnd = new NopQuad();
	nopQuadStart->addLabel(startLabel);
	proc->addQuad(nopQuadStart);

	auto condOpd = myCond->flatten(proc);
	auto testCondQuad = new IfzQuad(condOpd, afterLabel);
	proc->addQuad(testCondQuad);

	for (auto& stmt: *myBody)
		stmt->to3AC(proc);
	
	auto reloopQuad = new GotoQuad(startLabel);
	proc->addQuad(reloopQuad);
	nopQuadEnd->addLabel(afterLabel);
	proc->addQuad(nopQuadEnd);
}

void CallStmtNode::to3AC(Procedure * proc){
	myCallExp->flatten(proc);
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
