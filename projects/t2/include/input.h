#ifndef INPUT_H
#define INPUT_H
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>


enum RHSType
{
    ACTION,
    TERMINAL,
    NONTERMINAL,
    EPSILON
};

struct RHSObject
{
    RHSObject(){}
    RHSObject(RHSType t, std::string o) : type(t), obj(o)
    {

    }
    RHSObject(const RHSObject& other)
    {
        type = other.type;
        obj = other.obj;
    }

    bool operator==(const RHSObject& other)
    {
        return (other.type == type) && (obj == other.obj);
    }


    std::string toString()
    {
        std::string ret;

        return "RHSObject((RHSType)" + std::to_string(type) + ",\"" + obj + "\")";
    }

    RHSType type;
    std::string obj;
};


struct ProductionRule
{
    std::string m_LHS;
    std::vector<RHSObject> m_RHS;
    ProductionRule(){}

    ProductionRule(const ProductionRule& other)
    {
        m_LHS = other.m_LHS;
        m_RHS = other.m_RHS;
    }

    bool AddIfNeeded(const RHSObject& obj)
    {
        for (auto& o: m_RHS)
        {
            if (o == obj)
                return false;
        }
        m_RHS.push_back(obj);
        return true;
    }

    void Print()
    {
        std:: cout << m_LHS << " ::= ";
        for (auto& t: m_RHS)
            std::cout << t.obj << "(" << (t.type == RHSType::ACTION ? "action" : (t.type == RHSType::NONTERMINAL ? "NONTERMINAL" : "TERMINAL")) << ") ";
        std::cout << "\n";
    }

    std::string toString()
    {
        std::string ret;
        ret += "ProductionRule(\"" + m_LHS + "\",{";

        for (auto& obj: m_RHS)
            ret += obj.toString() + ",";

        ret += "})";
        return ret;     
    }
};


class InputSorter
{
public:
    InputSorter(const std::string& filename);
    ~InputSorter();


    bool ParseLines();
    bool HandleGrammarSection();
    bool HandleInitSection();
    bool HandleActionSection();

    void PrintActions();
    std::vector<ProductionRule> GetProdRules() {return m_ProductionRules;}
    std::string GetInitSection() { return m_InitSectionRaw; }
    std::unordered_map<std::string, std::string> GetActions() {return m_Actions;}
    std::unordered_map<std::string, RHSType> m_ProdMapping;
private:
    std::vector<std::string> SplitGrammarString(const std::string& input);
    std::vector<std::string> SplitActionString(const std::string& input);
    std::string ExtractBetweenBrackets(std::string input);
    std::string GetFirstWord(std::string input);
    

    

    std::string m_CurrentLine;
    std::string m_InitSectionRaw;
    std::vector<ProductionRule> m_ProductionRules;
    std::unordered_map<std::string, std::string> m_Actions;

    int m_LineNo;
    std::stringstream m_FileStringStream;
    std::string m_FileContents;
};


#endif