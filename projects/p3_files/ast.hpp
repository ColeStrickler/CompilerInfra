#ifndef A_LANG_AST_HPP
#define A_LANG_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>
#include <vector>

/* You'll probably want to add a bunch of ASTNode subclasses */

void strout(std::ostream& out, const std::string& str, int count);
static void doIndent(std::ostream& out, int indent){
	for (int k = 0 ; k < indent; k++){ out << "\t"; }
}
namespace a_lang
{

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
	class ASTNode
	{
	public:
		ASTNode(const Position *p) : myPos(p) {}
		virtual void unparse(std::ostream &out, int indent) = 0;
		const Position *pos() { return myPos; }
		std::string posStr() { return pos()->span(); }
		const Position *myPos = nullptr;
	protected:
		
	};

	/**
	 * \class ProgramNode
	 * Class that contains the entire abstract syntax tree for a program.
	 * Note the list of declarations encompasses all global declarations
	 * which includes (obviously) all global variables and struct declarations
	 * and (perhaps less obviously), all function declarations
	 **/
	class ProgramNode : public ASTNode
	{
	public:
		ProgramNode(std::list<DeclNode *> *globalsIn);
		void unparse(std::ostream &out, int indent) override;

	private:
		std::list<DeclNode *> *myGlobals;
	};

	/*
		EXP NODES
	*/
	/**  \class ExpNode
	 * Superclass for expression nodes (i.e. nodes that can be used as
	 * part of an expression).  Nodes that are part of an expression
	 * should inherit from this abstract superclass.
	 **/
	class ExpNode : public ASTNode
	{
	protected:
		ExpNode(const Position *p) : ASTNode(p) {}


	public:
		void AddExpressions(ExpNode* a, ExpNode* b)
		{
			m_Children.push_back(a);
			m_Children.push_back(b);;
		}

		void AddChild(ExpNode* child)
		{
			m_Children.push_back(child);
		}

		void begin(std::ostream &out, int indent)
		{
			if (m_InsideParentheses)
				out << "(";
		}

		void end(std::ostream &out, int indent)
		{
			if (m_InsideParentheses)
				out << ")";
		}

		std::vector<ExpNode*> m_Children;
		bool m_InsideParentheses;
	};

	/* BINARYEXP NODES */

	class BinaryExpNode : public ExpNode
	{
	protected: // protected means only available within the class hierarchy
		BinaryExpNode(const Position *p) : ExpNode(p)
		{
		}

	};

	class AndNode : public BinaryExpNode
	{
	public:
		AndNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " and ", indent);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class DivideNode : public BinaryExpNode
	{
	public:
		DivideNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " / ", indent);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class EqualsNode : public BinaryExpNode
	{
	public:
		EqualsNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " == ", indent);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class GreaterEqNode : public BinaryExpNode
	{
	public:
		GreaterEqNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " >= ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class GreaterNode : public BinaryExpNode
	{
	public:
		GreaterNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " > ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class LessEqNode : public BinaryExpNode
	{
	public:
		LessEqNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent) 
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " <= ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class LessNode : public BinaryExpNode
	{
	public:
		LessNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent) 
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " < ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class MinusNode : public BinaryExpNode
	{
	public:
		MinusNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " - ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class NotEqualsNode : public BinaryExpNode
	{
	public:
		NotEqualsNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " != ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class OrNode : public BinaryExpNode
	{
	public:
		OrNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent) 
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " or ", indent);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class PlusNode : public BinaryExpNode
	{
	public:
		PlusNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent) 
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " + ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};

	class TimesNode : public BinaryExpNode
	{
	public:
		TimesNode(const Position *p) : BinaryExpNode(p)
		{
		}

		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Children[0]->unparse(out, indent);
			strout(out, " * ", 0);
			m_Children[1]->unparse(out, indent);
			end(out, indent);
		}
	};


		/** A memory location. LocNodes subclass ExpNode
	 * because they can be used as part of an expression.
	 **/
	class LocNode : public ExpNode
	{
	public:
		LocNode(const Position* p) : ExpNode(p) {}
		LocNode(const Position *p, LocNode* body, LocNode* child)
			: ExpNode(p), m_Body(body), m_Child(child) {}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			m_Body->unparse(out, indent);
			if (m_Child)
			{
				out << "->";
				m_Child->unparse(out, indent);
			}
			end(out, indent);
		}

		LocNode* m_Body;
		LocNode* m_Child;

	};

	class CallExpNode : public ExpNode
	{
	public:
		CallExpNode(const Position *p, LocNode* loc) : ExpNode(p), m_Loc(loc)
		{
		}
		void unparse(std::ostream &out, int indent) 
		{
			doIndent(out, indent);
			begin(out, indent);
			m_Loc->unparse(out, 0);
			out << "(";
			for (uint32_t i = 0; i < m_ExprList.size(); i++)
			{
				m_ExprList[i]->unparse(out, 0);
				if (i != m_ExprList.size() - 1)
					out << ", ";
			}
			out << ")";
			end(out, indent);
		}

		void AddExpressions(const std::vector<ExpNode*>& exprs)
		{
			m_ExprList.insert(m_ExprList.end(), exprs.begin(), exprs.end());
		}

		LocNode* m_Loc;
		std::vector<ExpNode*> m_ExprList;
	};

	class EhNode : public ExpNode
	{
	public:
		EhNode(const Position *p) : ExpNode(p)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			end(out, indent);
		}
	};

	class FalseNode : public ExpNode
	{
	public:
		FalseNode(const Position *p) : ExpNode(p)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			strout(out, "false", 0);
			end(out, indent);
		}
	};

	class IntLitNode : public ExpNode
	{
	public:
		IntLitNode(const Position *p, int val) : ExpNode(p), m_Val(val)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			strout(out, std::to_string(m_Val), 0);
			end(out, indent);
		}

	private:
		int m_Val;
	};



	/** An identifier. Note that IDNodes subclass
	 * LocNode because they are a type of memory location.
	 **/
	class IDNode : public LocNode
	{
	public:
		IDNode(const Position *p, std::string nameIn)
			: LocNode(p), name(nameIn) {}
		void unparse(std::ostream &out, int indent)
		{

			strout(out, name ,indent);
		}

	private:
		/** The name of the identifier **/
		std::string name;
	};

	class MemberFieldExpNode : public LocNode
	{
	public:
		MemberFieldExpNode(const Position *p, std::string nameIn)
			: LocNode(p), name(nameIn) {}
		void unparse(std::ostream &out, int indent);

	private:
		/** The name of the identifier **/
		std::string name;
	};

	class StrLitNode : public ExpNode
	{
	public:
		StrLitNode(const Position *p, const std::string &val) : ExpNode(p), m_Val(val)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			out << m_Val;
			end(out, indent);
		}

	private:
		std::string m_Val;
	};

	class TrueNode : public ExpNode
	{
	public:
		TrueNode(const Position *p) : ExpNode(p)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			out << "true";
			end(out, indent);
		}
	};

	class UnaryExpNode : public ExpNode
	{
	public:
		UnaryExpNode(const Position *p) : ExpNode(p)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			end(out, indent);
		}

		

	};

	class NegationNode : public UnaryExpNode
	{
	public:
		NegationNode(const Position *p) : UnaryExpNode(p)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);
			end(out, indent);
		}



	};

	class NotNode : public UnaryExpNode
	{
	public:
		NotNode(const Position *p) : UnaryExpNode(p)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			begin(out, indent);

			end(out, indent);
		}
	};

	/*
		STATEMENT NODES
	*/

	class StmtNode : public ASTNode
	{
	public:
		StmtNode(const Position *p) : ASTNode(p) {}
		void unparse(std::ostream &out, int indent)
		{

		}
	};

		class StmtListNode : public ASTNode
	{
	public:
		StmtListNode(const Position *p, bool useSemicol) : ASTNode(p), m_UseSemicolon(useSemicol) {}
		void unparse(std::ostream &out, int indent)
		{
			if (myPos == nullptr)
			{
				printf("myPos == nullptr\n");
				return;
			}
			for (auto& child: m_Children)
			{
				child->unparse(out, indent);
				if (child->myPos != nullptr)
					out << ";\n";
			}
		}

		void AddChildren(const std::vector<StmtNode*> children)
		{
			m_Children.insert(m_Children.end(), children.begin(), children.end());
		}

		void AddChild(StmtNode* child)
		{
			m_Children.push_back(child);
		}

		bool m_UseSemicolon;
		std::vector<StmtNode*> m_Children;
		
	};


	class AssignStmtNode : public StmtNode
	{
	public:
		AssignStmtNode(const Position *p, LocNode* loc, ExpNode* exp) : StmtNode(p), m_Loc(loc), m_Exp(exp) {}
		void unparse(std::ostream &out, int indent) override
		{
			m_Loc->unparse(out, indent);
			out << " = ";
			m_Exp->unparse(out, 0);
		}

		LocNode* m_Loc;
		ExpNode* m_Exp;
		

	};

	class CallStmtNode : public StmtNode
	{
	public:
		CallStmtNode(const Position *p, ExpNode* callExp) : StmtNode(p), m_CallExp(callExp) {}
		void unparse(std::ostream &out, int indent)
		{
			m_CallExp->unparse(out, indent);
		}

		ExpNode* m_CallExp;
	};

	/** \class DeclNode
	 * Superclass for declarations (i.e. nodes that can be used to
	 * declare a struct, function, variable, etc).  This base class will
	 **/
	class DeclNode : public StmtNode
	{
	public:
		DeclNode(const Position *p) : StmtNode(p), m_PrintSemicolon(false) {}
		void unparse(std::ostream &out, int indent) override = 0;

		bool m_PrintSemicolon;
	};
	class TypeNode : public ASTNode
	{
	protected:
		TypeNode(const Position *p) : ASTNode(p)
		{
		}

	public:
		virtual void unparse(std::ostream &out, int indent)
		{

		}
	};

	/** A variable declaration.
	 **/
	class VarDeclNode : public DeclNode
	{
	public:
		VarDeclNode(const Position *p, IDNode *inID, TypeNode *inType) : DeclNode(p), myID(inID), myType(inType)
		{
			assert(myType != nullptr);
			assert(myID != nullptr);
		}
		void unparse(std::ostream &out, int indent);

	private:
		IDNode *myID;
		TypeNode *myType;
	};

	class FormalDeclNode : public VarDeclNode
	{
	public:
		FormalDeclNode(const Position *p, IDNode *inID, TypeNode *inType) : VarDeclNode(p, inID, inType)
		{
			assert(myType != nullptr);
			assert(myID != nullptr);
		}
		void unparse(std::ostream &out, int indent)
		{
			myID->unparse(out, indent);
			out << ": ";
			myType->unparse(out, indent);
		}

	private:
		IDNode *myID;
		TypeNode *myType;
	};

	class FnDeclNode : public DeclNode
	{
	public:
		FnDeclNode(const Position *p, IDNode *inID, TypeNode *inType)
			: DeclNode(p), myID(inID), myType(inType)
		{
			assert(myType != nullptr);
			assert(myID != nullptr);
		}


		void AddFormals(const std::vector<DeclNode *> &formals)
		{
			m_Formals.clear();
			m_Formals = formals;
		}

		void unparse(std::ostream &out, int indent)
		{
			myID->unparse(out, indent);
			out << " : (";
			for (auto& arg: m_Formals)
				arg->unparse(out, indent);
			out << ") -> ";
			myType->unparse(out, 0);
			out << " {\n";
			m_StmtList->unparse(out, indent+1);
			out << "}\n";

		}
		void AddStatements(StmtListNode* stmtList)
		{
			m_StmtList = stmtList;
		}

	private:
		IDNode *myID;
		TypeNode *myType;
		StmtListNode* m_StmtList;
		std::vector<DeclNode *> m_Formals;
	};

	class ClassDefnNode : public DeclNode
	{
	public:
		ClassDefnNode(const Position *p, IDNode *inID, TypeNode *inType)
			: DeclNode(p), myID(inID), myType(inType)
		{
			assert(myType != nullptr);
			assert(myID != nullptr);
		}
		void unparse(std::ostream &out, int indent);

	private:
		IDNode *myID;
		TypeNode *myType;
	};

	class ExitStmtNode : public StmtNode
	{
	public:
		ExitStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override
		{

		}
	};

	class FromConsoleStmtNode : public StmtNode
	{
	public:
		FromConsoleStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override 
		{

		}
	};

	class IfElseStmtNode : public StmtNode
	{
	public:
		IfElseStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override
		{

		}
	};

	class IfStmtNode : public StmtNode
	{
	public:
		IfStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override
		{

		}
	};

	class MaybeStmtNode : public StmtNode
	{
	public:
		MaybeStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override
		{

		}
	};

	class PostDecStmtNode : public StmtNode
	{
	public:
		PostDecStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override
		{

		}
	};

	class PostIncStmtNode : public StmtNode
	{
	public:
		PostIncStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override 
		{

		}
	};

	class ReturnStmtNode : public StmtNode
	{
	public:
		ReturnStmtNode(const Position *p, ExpNode* node) : StmtNode(p), m_Child(node) {}
		void unparse(std::ostream &out, int indent) override
		{
			strout(out, "return", indent);
			if (m_Child != nullptr)
			{
				out << " ";
				m_Child->unparse(out, 0);
			}

				
		}

		ExpNode* m_Child;
	};

	class ToConsoleStmtNode : public StmtNode
	{
	public:
		ToConsoleStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override 
		{

		}
	};

	class WhileStmtNode : public StmtNode
	{
	public:
		WhileStmtNode(const Position *p) : StmtNode(p) {}
		void unparse(std::ostream &out, int indent) override
		{

		}
	};

	/*
		TYPE NODES
	*/
	/**  \class TypeNode
	 * Superclass of nodes that indicate a data type. For example, in
	 * the declaration "int a", the int part is the type node (a is an IDNode
	 * and the whole thing is a DeclNode).
	 **/


	class IntTypeNode : public TypeNode
	{
	public:
		IntTypeNode(const Position *p) : TypeNode(p) {}
		void unparse(std::ostream &out, int indent)
		{
			strout(out, "int", indent);
		}
	};

	class BoolTypeNode : public TypeNode
	{
	public:
		BoolTypeNode(const Position *p) : TypeNode(p) {}
		void unparse(std::ostream &out, int indent)
		{
			strout(out, "bool", indent);
		}
	};

	class ClassTypeNode : public TypeNode
	{
	public:
		ClassTypeNode(const Position *p) : TypeNode(p) {}
		void unparse(std::ostream &out, int indent)
		{
			strout(out, "class", indent);
		}
	};

	class ImmutableTypeNode : public TypeNode
	{
	public:
		ImmutableTypeNode(const Position *p, TypeNode *type) : TypeNode(p), m_Type(type)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			strout(out, "", indent);
		}

	private:
		TypeNode *m_Type;
	};

	class RefTypeNode : public TypeNode
	{
	public:
		RefTypeNode(const Position *p, TypeNode *node) : TypeNode(p), m_Type(node)
		{
		}
		void unparse(std::ostream &out, int indent)
		{
			strout(out, "&", indent);
			m_Type->unparse(out, indent);
		}

	private:
		TypeNode *m_Type;
	};

	class VoidTypeNode : public TypeNode
	{
	public:
		VoidTypeNode(const Position *p) : TypeNode(p) {}
		void unparse(std::ostream &out, int indent)
		{
			strout(out, "void", indent);
		}
	};

} // End namespace a_lang

#endif
