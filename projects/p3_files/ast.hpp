#ifndef A_LANG_AST_HPP
#define A_LANG_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>
#include <vector>

/* You'll probably want to add a bunch of ASTNode subclasses */

namespace a_lang{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;

/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(const Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	const Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	const Position * myPos = nullptr;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};




 

/*
	EXP NODES
*/
/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
	ExpNode(const Position * p) : ASTNode(p){ }
};


/* BINARYEXP NODES */

class BinaryExpNode: public ExpNode {
protected: // protected means only available within the class hierarchy
	BinaryExpNode(const Position* p) : ExpNode(p)
	{

	}
};

class AndNode: public BinaryExpNode {
public:
	AndNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class DivideNode: public BinaryExpNode {
public:
	DivideNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class EqualsNode: public BinaryExpNode {
public:
	EqualsNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};


class GreaterEqNode: public BinaryExpNode {
public:
	GreaterEqNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class GreaterNode: public BinaryExpNode {
public:
	GreaterNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class LessEqNode: public BinaryExpNode {
public:
	LessEqNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class LessNode: public BinaryExpNode {
public:
	LessNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class MinusNode: public BinaryExpNode {
public:
	MinusNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};


class NotEqualsNode: public BinaryExpNode {
public:
	NotEqualsNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class OrNode: public BinaryExpNode {
public:
	OrNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class PlusNode: public BinaryExpNode {
public:
	PlusNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};

class TimesNode: public BinaryExpNode {
public:
	TimesNode(const Position* p) : BinaryExpNode(p)
	{

	}

	void unparse(std::ostream& out, int indent) = 0;
};



class CallExpNode : public ExpNode{
public:
	CallExpNode(const Position * p): ExpNode(p) 
	{

	}
	void unparse(std::ostream& out, int indent) = 0;
};

class EhNode : public ExpNode{
public:
	EhNode(const Position * p): ExpNode(p) 
	{
		
	}
	void unparse(std::ostream& out, int indent) = 0;
};

class FalseNode : public ExpNode{
public:
	FalseNode(const Position * p): ExpNode(p) 
	{
		
	}
	void unparse(std::ostream& out, int indent)
	{
		out << "false";
	}
};

class IntLitNode : public ExpNode{
public:
	IntLitNode(const Position * p, int val): ExpNode(p), m_Val(val)
	{
		
	}
	void unparse(std::ostream& out, int indent)
	{
		out << m_Val;
	}
private:
	int m_Val;

};


/** A memory location. LocNodes subclass ExpNode
 * because they can be used as part of an expression. 
**/
class LocNode : public ExpNode{
public:
	LocNode(const Position * p)
	: ExpNode(p) {}
	void unparse(std::ostream& out, int indent) = 0;
};

/** An identifier. Note that IDNodes subclass
 * LocNode because they are a type of memory location. 
**/
class IDNode : public LocNode{
public:
	IDNode(const Position * p, std::string nameIn) 
	: LocNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string name;
};

class MemberFieldExpNode : public LocNode{
public:
	MemberFieldExpNode(const Position * p, std::string nameIn) 
	: LocNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string name;
};

class StrLitNode : public ExpNode{
public:
	StrLitNode(const Position * p, const std::string& val): ExpNode(p), m_Val(val)
	{
		
	}
	void unparse(std::ostream& out, int indent)
	{
		out << m_Val;
	}
private:
	std::string m_Val;
};

class TrueNode : public ExpNode{
public:
	TrueNode(const Position * p): ExpNode(p) 
	{
		
	}
	void unparse(std::ostream& out, int indent)
	{
		out << "true";
	}
};

class UnaryExpNode : public ExpNode{
public:
	UnaryExpNode(const Position * p): ExpNode(p) 
	{
		
	}
	void unparse(std::ostream& out, int indent) = 0;
};

class NegationNode : public UnaryExpNode{
public:
	NegationNode(const Position * p): UnaryExpNode(p) 
	{
		
	}
	void unparse(std::ostream& out, int indent) = 0;
};


class NotNode : public UnaryExpNode{
public:
	NotNode(const Position * p): UnaryExpNode(p) 
	{
		
	}
	void unparse(std::ostream& out, int indent) = 0;
};



/*
	STATEMENT NODES
*/

class StmtNode : public ASTNode{
public:
	StmtNode(const Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
};


class AssignStmtNode : public StmtNode{
public:
	AssignStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(const Position * p, IDNode * inID, TypeNode * inType) : DeclNode(p), myID(inID), myType(inType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
	void unparse(std::ostream& out, int indent);
private:
	IDNode * myID;
	TypeNode * myType;
};

class FormalDeclNode : public VarDeclNode {
public:
	FormalDeclNode(const Position * p, IDNode * inID, TypeNode * inType) : VarDeclNode(p, inID, inType) {
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
	void unparse(std::ostream& out, int indent)
	{

	}
private:
	IDNode * myID;
	TypeNode * myType;
};

class FnDeclNode : public DeclNode{
public:
	FnDeclNode(const Position * p, IDNode * inID, TypeNode * inType) 
	: DeclNode(p), myID(inID), myType(inType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
	

	void AddFormals(const std::vector<DeclNode*>& formals)
	{
		m_Formals.clear();
		m_Formals = formals;
	}

	void unparse(std::ostream& out, int indent)
	{
		
	}
	void AddStatements(const std::vector<StmtNode*>& statements)
	{
		m_FnStatements.clear();
		m_FnStatements = statements;
	}
private:
	IDNode * myID;
	TypeNode * myType;
	std::vector<StmtNode*> m_FnStatements;
	std::vector<DeclNode*> m_Formals;
};

class ClassDefnNode : public DeclNode{
public:
	ClassDefnNode(const Position * p, IDNode * inID, TypeNode * inType) 
	: DeclNode(p), myID(inID), myType(inType){
		assert (myType != nullptr);
		assert (myID != nullptr);
	}
	void unparse(std::ostream& out, int indent);
private:
	IDNode * myID;
	TypeNode * myType;
};

class ExitStmtNode : public StmtNode{
public:
	ExitStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class FromConsoleStmtNode : public StmtNode{
public:
	FromConsoleStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class IfElseStmtNode : public StmtNode{
public:
	IfElseStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class IfStmtNode : public StmtNode{
public:
	IfStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class MaybeStmtNode : public StmtNode{
public:
	MaybeStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class ReturnStmtNode : public StmtNode{
public:
	ReturnStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class ToConsoleStmtNode : public StmtNode{
public:
	ToConsoleStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class WhileStmtNode : public StmtNode{
public:
	WhileStmtNode(const Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};


/*
	TYPE NODES
*/
/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(const Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};


class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent)
	{

	}
};

class ClassTypeNode : public TypeNode{
public:
	ClassTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class ImmutableTypeNode : public TypeNode{
public:
	ImmutableTypeNode(const Position * p, TypeNode* type) : TypeNode(p), m_Type(type)
	{ 

	}
	void unparse(std::ostream& out, int indent)
	{

	}

private:
	TypeNode* m_Type;
};

class RefTypeNode : public TypeNode{
public:
	RefTypeNode(const Position * p, TypeNode* node) : TypeNode(p), m_Type(node)
	{

	}
	void unparse(std::ostream& out, int indent)
	{

	}

private:
	TypeNode* m_Type;
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(const Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent)
	{

	}
};


} //End namespace a_lang





#endif
