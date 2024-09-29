#include "mathnode.hpp"
using namespace EECS;

MathNode::MathNode() : m_NodeNo(EECS::Manager::GetNodeCount())
{
    EECS::Manager::IncNodeCount();
}

MathNode::~MathNode()
{
}

void MathNode::AddChild(std::string l1, MathNode *node)
{
    m_Children.push_back({l1, node});
}

std::string MathNode::Print()
{
    return std::string("MATHNODE BASE PRINT");
}

std::string EECS::MathNode::GetNode()
{
    return "node" + std::to_string(m_NodeNo);
}

IntNode::IntNode(int valueIn) : m_Value(valueIn), MathNode() 
{
}

std::string IntNode::Print()
{
    return "\t" + GetNode() + " [shape=record label=\"" + std::to_string(m_Value) + "\"]\n";
}

MultNode::MultNode() : MathNode()
{
}

std::string MultNode::Print()
{
    std::string ret;
    std::string currNode = GetNode();
    ret += "\t" + currNode + " [shape=record label=\"" + std::string("*") + "\"]\n";
    int arm = 1;
    for (auto& child: m_Children)
    {
        ret += child.second->Print();
        ret += "\t" + currNode + " -> " + child.second->GetNode() + " [label=\"arm" + std::to_string(arm) + "\"]\n";
        arm++;
    }
    return ret;
}

std::string MultNode::Op()
{
    return m_Children.size() == 2 ? " * " : "";
}

DivNode::DivNode() : MathNode()
{
}

std::string DivNode::Print()
{
    std::string ret;
    std::string currNode = GetNode();
    ret += "\t" + currNode + " [shape=record label=\"" + std::string("/") + "\"]\n";
    int arm = 1;
    for (auto& child: m_Children)
    {
        ret += child.second->Print();
        ret += "\t" + currNode + " -> " + child.second->GetNode() + " [label=\"arm" + std::to_string(arm) + "\"]\n";
        arm++;
    }
    return ret;
}



std::string DivNode::Op()
{
    return m_Children.size() == 2 ? " / " : "";
}

PlusNode::PlusNode() : MathNode()
{
    
}

std::string PlusNode::Print()
{
    std::string ret;
    std::string currNode = GetNode();
    ret += "\t" + currNode + " [shape=record label=\"" + std::string("+") + "\"]\n";
    int arm = 1;
    for (auto& child: m_Children)
    {
        ret += child.second->Print();
        ret += "\t" + currNode + " -> " + child.second->GetNode() + " [label=\"arm" + std::to_string(arm) + "\"]\n";
        arm++;
    }
    return ret;
}

std::string PlusNode::Op()
{
    return m_Children.size() == 2 ? " + " : "";
}

SubNode::SubNode() : MathNode()
{
}

std::string SubNode::Print()
{
    std::string ret;
    std::string currNode = GetNode();
    ret += "\t" + currNode + " [shape=record label=\"" + std::string("-") + "\"]\n";
    int arm = 1;
    for (auto& child: m_Children)
    {
        ret += child.second->Print();
        ret += "\t" + currNode + " -> " + child.second->GetNode() + " [label=\"arm" + std::to_string(arm) + "\"]\n";
        arm++;
    }
    return ret;
}

std::string SubNode::Op()
{
    return m_Children.size() == 2 ? " - " : "";
}