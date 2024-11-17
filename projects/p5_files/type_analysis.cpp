#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"

namespace a_lang{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	//To emphasize that type analysis depends on name analysis
	// being complete, a name analysis must be supplied for 
	// type analysis to be performed.
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;	
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;
}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){

	//pass the TypeAnalysis down throughout
	// the entire tree, getting the types for
	// each element in turn and adding them
	// to the ta object's hashMap
	for (auto global : *myGlobals){
		global->typeAnalysis(ta);
	}

	//The type of the program node will never
	// be needed. We can just set it to VOID
	//(Alternatively, we could make our type 
	// be error if the DeclListNode is an error)
	ta->nodeType(this, BasicType::produce(VOID));
}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta){

	//HINT: you might want to change the signature for
	// typeAnalysis on FnBodyNode to take a second
	// argument which is the type of the current 
	// function. This will help you to know at a 
	// return statement whether the return type matches
	// the current function

	//Note: this function may need extra code
	auto fnType = getRetTypeNode()->getType();
	ta->currentFuncReturnType = fnType;


	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}

	// unset return type since we are leaving function
	ta->currentFuncReturnType = nullptr;
}
void ClassDefnNode::typeAnalysis(TypeAnalysis *ta)
{
	return;
}


void CallExpNode::typeAnalysis(TypeAnalysis * ta) 
{
	myCallee->typeAnalysis(ta);
	auto calleeType = ta->nodeType(myCallee);
	if (calleeType->asError() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	auto asFn = calleeType->asFn();
	if (asFn == nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errCallee(myCallee->pos());
		return;
	}
	
	
	
	// Ensure argument count is correct
	auto formalList = asFn->getFormalTypes()->getTypes();
	if (formalList->size() != myArgs->size())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errArgCount(myCallee->pos());
		return;
	}

	// Ensure argument types are correct
	int arg = 0;
	bool argError = false;
	auto it = formalList->begin(); auto argIt = myArgs->begin();
	for (;it != formalList->end();)
	{
		auto formalType = *it;
		auto argNode = *argIt;
		it++;
		argIt++;
		argNode->typeAnalysis(ta);
		auto argType = ta->nodeType(argNode);
		if (formalType->isBool() && !argType->isBool())
			goto argerror;
		if (formalType->isInt() && !argType->isInt())
			goto argerror;
		if (formalType->isClass() && !argType->isClass())
			goto argerror;	
		continue;
argerror:
		ta->errArgMatch(argNode->pos());
		ta->nodeType(this, ErrorType::produce());
		argError = true;
	}
	if (argError) // got error, leave nodeType as ErrorType
		return;


	// set to the return type of the function
	auto retType = asFn->getReturnType();
	ta->nodeType(this, retType);
}

void StmtNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Implement me in the subclass");
}

void ReturnStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	printf("ReturnStmtNode::typeAnalysis()\n");
	auto retType = ta->currentFuncReturnType;
	/*
		 we should never get this, return statements should only occur inside a function
	*/
	if(retType == nullptr)
		printf("[ERROR]: ReturnStmtNode::typeAnalysis() ta->currentFuncReturnType was NULL.\n");


	if (myExp == nullptr)
	{
		if (!retType->isVoid()) // no return type and it isn't a void function
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errRetEmpty(pos());
			return;
		}
	}

	

	myExp->typeAnalysis(ta);
	auto expType = ta->nodeType(myExp);
	if (expType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
		return;
	}


	// returning value in void function
	if (retType->isVoid())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->extraRetValue(myExp->pos());
		return;
	}

	if (!ta->areSameType(expType, retType))
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRetWrong(myExp->pos());
	}
}


void CallStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	myCallExp->typeAnalysis(ta);

	auto callType = ta->nodeType(myCallExp);
	if (callType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	ta->nodeType(this, callType);
}

void PostDecStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	myLoc->typeAnalysis(ta);
	auto locType = ta->nodeType(myLoc);
	if (locType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!locType->isInt() || locType->isImmutable())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(pos());
		return;
	}
}

void PostIncStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	myLoc->typeAnalysis(ta);
	auto locType = ta->nodeType(myLoc);
	if (locType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
		return;
	}

	if (!locType->isInt() || locType->isImmutable())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(pos());
		return;
	}
}

void AssignStmtNode::typeAnalysis(TypeAnalysis * ta){
	//TODO: Note that this function is incomplete. 
	// and needs additional code

	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);


	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself. It 
	// sort of serves as a way to cast the subtype
	if (tgtType->asError() || srcType->asError()){
		ta->nodeType(this, ErrorType::produce());
		// --> do we wanna return here?
	}

	if ((tgtType->isBool() && srcType->isInt()) || (srcType->isBool() && tgtType->isInt()))
	{
		//Some functions are already defined for you to
		// report type errors. Note that these functions
		// also tell the typeAnalysis object that the
		// analysis has failed, meaning that main.cpp
		// will print "Type check failed" at the end
		ta->nodeType(this, ErrorType::produce());
		ta->errAssignOpr(this->pos());
		return;
	}


	/*
		TODO: how exactly to handle these?
	*/
	if (srcType->isClass())
	{
		
	}
	if (tgtType->isClass())
	{
		
	}
	if (tgtType->isImmutable())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errAssignOpd(pos());
		return;
	}



	//While incomplete, this gives you one case for 
	// assignment: if the types are exactly the same
	// it is usually ok to do the assignment. One
	// exception is that if both types are function
	// names, it should fail type analysis
	if (tgtType == srcType){
		ta->nodeType(this, tgtType);
		return;
	}
	

	


	// Here, we set the type of the assignment
	// to void to indicate no error was found.
	// This step is optional, since you'll never
	// use the type of a statement
	ta->nodeType(this, BasicType::produce(VOID));

}



void FromConsoleStmtNode::typeAnalysis(TypeAnalysis *ta)
{
		myDst->typeAnalysis(ta);
		auto dstType = ta->nodeType(myDst);
		
		// got error type, no need to report
		if (dstType->asError())
		{
			ta->nodeType(this, ErrorType::produce());
			return;
		}

		if (dstType->asFn() != nullptr)
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errReadFn(myDst->pos());
			return;
		}

		if (dstType->isClass())
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errReadClass(myDst->pos());
			return;
		}


}


void ToConsoleStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	{
		mySrc->typeAnalysis(ta);
		auto srcType = ta->nodeType(mySrc);

		// got error type, no need to report
		if (srcType->asError())
		{
			ta->nodeType(this, ErrorType::produce());
			return;
		}
		
		// Attempt to output a function
		if (srcType->asFn() != nullptr)
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errOutputFn(mySrc->pos());
			return;
		}

		// Attempt to output a class
		if (srcType->isClass())
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errOutputClass(mySrc->pos());
			return;
		}

		// Attempt to output a void
		if (srcType->isVoid())
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errOutputVoid(mySrc->pos());
			return;
		}

	}
}





void ExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("ExpNode Override me in the subclass");
}

void PlusNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);
	// I think this is correctly handling the asFn() ??
	if (t1->isBool() || t1->asFn() != nullptr || t1->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	if (t2->isBool() || t2->asFn() != nullptr || t2->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
	return;	
}

void MinusNode::typeAnalysis(TypeAnalysis *ta)
{	
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);
	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);
	// I think this is correctly handling the asFn() ??
	if (t1->isBool() || t1->asFn() != nullptr || t1->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	if (t2->isBool() || t2->asFn() != nullptr || t2->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
	return;
}

void TimesNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);
	// I think this is correctly handling the asFn() ??
	if (t1->isBool() || t1->asFn() != nullptr || t1->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	if (t2->isBool() || t2->asFn() != nullptr || t2->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
	return;
}

void DivideNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);
	// I think this is correctly handling the asFn() ??
	if (t1->isBool() || t1->asFn() != nullptr || t1->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	if (t2->isBool() || t2->asFn() != nullptr || t2->asClass() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errMathOpd(this->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(INT));
	return;
}

void AndNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);

	if (!t1->isBool())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errLogicOpd(myExp1->pos());
		return;
	}
	if (!t2->isBool())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errLogicOpd(myExp2->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void OrNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);

	if (!t1->isBool())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errLogicOpd(myExp1->pos());
		return;
	}
	if (!t2->isBool())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errLogicOpd(myExp2->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void EqualsNode::typeAnalysis(TypeAnalysis *ta)
{

	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);


	auto exp1Type = ta->nodeType(myExp1);
	auto exp2Type = ta->nodeType(myExp2);
	

	if ((exp1Type->isInt() && exp2Type->isBool()) || (exp2Type->isInt() && exp1Type->isBool()))
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errEqOpr(pos());
		return;
	}

	if (exp1Type->isVoid() || exp1Type->isClass() || exp1Type->asFn() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errEqOpd(myExp1->pos());
		return;
	}

	if (exp2Type->isVoid() || exp2Type->isClass() || exp2Type->asFn() != nullptr)
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errEqOpd(myExp2->pos());
		return;
	}



	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void NotEqualsNode::typeAnalysis(TypeAnalysis *ta)
{

	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void LessNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);

	if (!t1->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp1->pos());
		return;
	}
	if (!t2->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp2->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void LessEqNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);

	if (!t1->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp1->pos());
		return;
	}
	if (!t2->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp2->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void GreaterNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);

	if (!t1->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp1->pos());
		return;
	}
	if (!t2->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp2->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void GreaterEqNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	auto t1 = ta->nodeType(myExp1);
	auto t2 = ta->nodeType(myExp2);

	if (!t1->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp1->pos());
		return;
	}
	if (!t2->isInt())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errRelOpd(myExp2->pos());
		return;
	}
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}

void NegNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp->typeAnalysis(ta);

	auto expType = ta->nodeType(myExp);
	if (!expType->isBool())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errLogicOpd(myExp->pos());
		return;
	}

	ta->nodeType(this, BasicType::produce(BOOL));
}

void NotNode::typeAnalysis(TypeAnalysis *ta)
{
	myExp->typeAnalysis(ta);

	auto expType = ta->nodeType(myExp);
	if (!expType->isBool())
	{
		ta->nodeType(this, ErrorType::produce());
		ta->errLogicOpd(myExp->pos());
		return;
	}

	ta->nodeType(this, BasicType::produce(BOOL));
}

void IfStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	printf("ifstmtnode\n");
	myCond->typeAnalysis(ta);
	auto condType = ta->nodeType(myCond);
	if (condType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	else
	{
		if (!condType->isBool())
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errCond(myCond->pos());
		}
	}


	for (auto stmt: *myBody)
		stmt->typeAnalysis(ta);
	

}

void IfElseStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	myCond->typeAnalysis(ta);
	auto condType = ta->nodeType(myCond);
	if (condType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	else
	{
		if (!condType->isBool())
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errCond(myCond->pos());
		}
	}
	for (auto stmt: *myBodyTrue)
		stmt->typeAnalysis(ta);

	for (auto stmt: *myBodyFalse)
		stmt->typeAnalysis(ta);
}

void WhileStmtNode::typeAnalysis(TypeAnalysis *ta)
{
	myCond->typeAnalysis(ta);
	auto condType = ta->nodeType(myCond);
	if (condType->asError())
	{
		ta->nodeType(this, ErrorType::produce());
	}
	else
	{
		if (!condType->isBool())
		{
			ta->nodeType(this, ErrorType::produce());
			ta->errCond(myCond->pos());
		}
	}

	for (auto stmt: *myBody)
		stmt->typeAnalysis(ta);
}

void DeclNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta){
	// VarDecls always pass type analysis, since they 
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class

	if (myInit != nullptr)
	{
		myInit->typeAnalysis(ta);
		auto initType = ta->nodeType(myInit);
		if (initType->asError())
			goto end;

		auto type = myType->getType();

		if ((type->isBool() && initType->isInt()) || (type->isInt() && initType->isBool()))
		{
			ta->errAssignOpr(myInit->pos());
			goto end;
		}

		if (initType->asFn() || (initType->isVoid() && !type->isVoid()))
		{
			ta->errAssignOpd(myInit->pos());
			goto end;
		}

	}

end:
	ta->nodeType(this, BasicType::produce(VOID));
}

void IDNode::typeAnalysis(TypeAnalysis * ta){
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
}



void TrueNode::typeAnalysis(TypeAnalysis *ta)
{
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}
void FalseNode::typeAnalysis(TypeAnalysis *ta)
{
	ta->nodeType(this, BasicType::produce(BOOL));
	return;
}


}
