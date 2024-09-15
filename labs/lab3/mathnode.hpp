#ifndef MATHNODE_H
#define MATHNODE_H

#include "calc.hpp"
#include <string>
#include <vector>

namespace EECS
{
    class MathNode
    {
    public:
        MathNode();
        ~MathNode();
        void AddChild(std::string l1, MathNode* node);
        virtual std::string Print();
        virtual std::string Op() = 0;
        std::string GetNode();
        int m_NodeNo;
    protected: // allows for inherited classes to access
        std::vector<std::pair<std::string, MathNode*>> m_Children;
    };



    class MultNode : public MathNode
    {
    public:
        MultNode();
        std::string Print() override;
        std::string Op() override;
    private:
        
    };

    class DivNode : public MathNode
    {
    public:
        DivNode();
        std::string Print() override;
        std::string Op() override;
    private:

    };

    class PlusNode : public MathNode
    {
    public:
        PlusNode();
        std::string Print() override;
        std::string Op() override;
    private:  
    };


    class SubNode : public MathNode
    {
    public:
        SubNode();
        std::string Print() override;
        std::string Op() override;
    private:  
    };


    class IntNode: public MathNode {
    public:
        IntNode(int valueIn);
        std::string Print() override;
        std::string Op() override {return "";}
        int getLitValue();
    private:
        int m_Value;
    };
}






#endif