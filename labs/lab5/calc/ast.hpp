#ifndef EECS665_AST
#define EECS665_AST
#include <list>
#include <vector>

namespace EECS {



class OpNode;
class MathNode;


enum OpTypes{
	CALCULATE, ORATE
};

class OpNode{
public:
	OpNode()
	{

	}
	std::vector<OpNode*> m_Children;
	virtual void print()
	{
	}
};

class ProgramNode{
public:
	ProgramNode(std::vector<OpNode*>& op) : ops(op)
	{

	}


	std::vector<OpNode *> ops;
	void print()
	{ 
		for (auto& node: ops)
		{
			node->print();
			std::cout << "\n";
		}
		
	}
};






class MathNode{
public:
	MathNode(const std::string& op, const std::vector<MathNode*>& children) : m_Children(children),  m_Op(op)
	{

	}
	MathNode(const std::string& op) : m_Op(op)
	{

	}
	MathNode()
	{

	}
	virtual void print()
	{
		if (m_Children.size() < 1)
			return;

		auto child = m_Children[0];
		child->print();


		for (int i = 1; i < m_Children.size(); i++)
		{
			std::cout << m_Op;
			m_Children[i]->print();
		}
	}


	std::vector<MathNode*> m_Children;
	std::string m_Op;
};

class OrateNode : public OpNode
{
public:
	OrateNode(const char* id_val, MathNode* pNode) : m_ID(id_val), m_Child(pNode)
	{

	}

	OrateNode(const std::string& id_val) : m_ID(id_val), m_Child(nullptr)
	{

	}

	void print() override 
	{
		std::cout << "orate ";
		if (m_ID.size())
			std::cout << m_ID << " ";


		if (m_Child == nullptr)
			return;
		m_Child->print();
	}

	MathNode* m_Child;
	std::string m_ID;

};


class CalculateNode: public OpNode
{
public:
	CalculateNode(const std::string& id_val, MathNode* pNode) : m_ID(id_val), m_Child(pNode)
	{

	}

	void print() override 
	{
		std::cout << "calculate " << m_ID << " ";
		if (m_Child == nullptr)
			return;
		m_Child->print();
	}

	MathNode* m_Child;
	std::string m_ID;
};



class MultNode : public MathNode
{
public: 
	MultNode(const std::vector<MathNode*> children) : MathNode("*",  children)
	{

	}
};


class MinusNode : public MathNode
{
public:
	MinusNode(const std::vector<MathNode*> children) : MathNode("-", children)
	{

	}
};

class PlusNode : public MathNode
{
public:
	PlusNode(const std::vector<MathNode*> children) : MathNode("+", children)
	{

	}
};

class DivNode : public MathNode
{
public:
	DivNode(const std::vector<MathNode*> children) : MathNode("/", children)
	{

	}


};




class IDNode : public MathNode
{
public:
	IDNode(const std::string& id) : m_ID(id)
	{

	}


	void print() override 
	{
		std::cout << m_ID;
	}

	std::string m_ID;
};


class NumNode : public MathNode
{
public:
	NumNode(int num) : m_Num(num)
	{

	}

	void print() override
	{
		std::cout << m_Num;
	}

	int m_Num;
};

class ParenNode : public MathNode
{
public:
	ParenNode(MathNode* node) : m_Child(node)
	{

	}


	void print() override 
	{
		std::cout << "(";
		m_Child->print();
		std::cout << ")";
	}



	MathNode* m_Child;
};


} //End namespace
#endif


