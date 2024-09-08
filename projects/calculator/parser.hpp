#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <vector>
#include "scanner.hpp"

struct Child;


class Node
{
public:
    virtual float translate();
    std::vector<Child> m_Children;
    void AddChild(Child child);
    void AddChildren(Child child1, Child child2);
};

struct Child
{
    Child(int op, Node* n) : op(op), node(n) {

    };
    int op;
    Node* node;
};

class RootNode : public Node
{
public:
    RootNode();
    ~RootNode();
    float translate();
};

class PlusExpr : public Node
{
public:
    PlusExpr();
    PlusExpr(Node* expr, Node* term);
    ~PlusExpr();
    float translate();
};

class DashExpr : public Node
{
public:
    DashExpr();
    DashExpr(Node* expr, Node* term);
    ~DashExpr();
    float translate();
};

class RawExpr : public Node
{
public:
    RawExpr();
    RawExpr(Node* term);
    ~RawExpr();
    float translate();
};



class StarTerm : public Node
{
public:
    StarTerm();
    StarTerm(Node* term, Node* factor);
    ~StarTerm();
    float translate();
};

class SlashTerm : public Node
{
public:
    SlashTerm();
    SlashTerm(Node* term, Node* factor);
    ~SlashTerm();
    float translate();
};

class RawTerm: public Node
{
public:
    RawTerm();
    RawTerm(Node* factor);
    ~RawTerm();
    float translate();
};


class RawFactor: public Node
{
public:
    RawFactor();
    RawFactor(const std::string& literal);
    ~RawFactor();
    std::string m_Literal;
    float translate();
};

class ParenFactor: public Node
{
public:
    ParenFactor();
    ParenFactor(Node* expr);
    ~ParenFactor();
    float translate();
};



class Parser
{
public:
    Parser();
    Parser(std::vector<Token*> tokens);
    ~Parser();
    RootNode* Parse();
private:
    Node* ParseExpr();
    Node* ParseTerm();
    Node* ParseFactor();
    Token* ConsumeToken();
    TOKENTYPE PeekToken();
    TOKENTYPE PeekNextToken();
    bool AtEnd();
    void ParseError();
    void ParseError(const std::string& msg);

    Token* m_CurrentToken;


    std::vector<Token*> m_Tokens;
    int m_Loc;
    RootNode* m_CurrentTree;
};




#endif