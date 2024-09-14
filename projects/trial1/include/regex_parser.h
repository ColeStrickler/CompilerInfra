#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include "scanner.hpp"



class RegExprNode {
public:
    RegExprNode();
    ~RegExprNode();
protected:
    virtual void Print() = 0;

};

class OrExpr : public RegExprNode {
public:
    OrExpr();
    ~OrExpr();
    void Print() override;
};

class StarExpr : public RegExprNode {
public:
    StarExpr();
    ~StarExpr();
    void Print() override;
};

class InterMedExpr : public RegExprNode {
public:
    InterMedExpr();
    ~InterMedExpr();
    
    void Print() override;

};



class DashExpr : public RegExprNode {
public:
    DashExpr();
    ~DashExpr();
    void Print() override;
};


class ParenExpr : public RegExprNode {
public:
    ParenExpr();
    ~ParenExpr();
    void Print() override;
};

class CharExpr : public RegExprNode {
public:
    CharExpr();
    ~CharExpr();
    void Print() override;
};

class CharLitExpr : public RegExprNode {
public:
    CharLitExpr();
    ~CharLitExpr();
    void Print() override;
};

class RegexParser
{
public:
    RegexParser();
    ~RegexParser();
    



    /*
        We will get this to return a DFA

        void Parse(std::vector<Token> tokens);
    */
    

};



#endif