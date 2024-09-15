#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include "scanner.hpp"
#include "automata.h"
#include <vector>

#define TOKEN_INVALID ((TokenType)-1)
#define NEW_NODE(type) ((type*)(AddPointer(new type())))
// Correct macro definition
#define PRINT_CALL() \
    do { \
        if (m_DebugMode) { \
            std::cout << "CALL: " << typeid(*this).name() << "::" << __func__ << std::endl; \
        } \
    } while (0)


enum REGEX_COMPILER_ERROR
{
    NO_ERROR_COMPILER,
    SCANNER_ERROR,
    PARSER_ERROR,
};

class RegExprNode;

struct ChildNode
{
public:
    ChildNode(uint16_t type , RegExprNode* node);

    uint16_t m_Type;
    RegExprNode* m_Node;
    
};


class RegExprNode {
public:
    RegExprNode();
    ~RegExprNode();
    virtual void Print();
    virtual NFA* Translate();
    void AddChild(const ChildNode& node);
    std::vector<ChildNode> m_Children;
    bool m_DebugMode;
protected:
    
};

class OrExpr : public RegExprNode {
public:
    OrExpr();
    ~OrExpr();
    NFA* Translate() override;
    void Print() override;
};

class StarExpr : public RegExprNode {
public:
    StarExpr();
    ~StarExpr();
    NFA* Translate() override;
    void Print() override;
};

class InterMedExpr : public RegExprNode {
public:
    InterMedExpr();
    ~InterMedExpr();
    NFA* Translate() override;
    void Print() override;

};

class BracketExpr : public RegExprNode {
public:
    BracketExpr();
    ~BracketExpr();
    NFA* Translate() override;
    void Print() override;
};


class DashExpr : public RegExprNode {
public:
    DashExpr();
    ~DashExpr();
    NFA* Translate() override;
    void Print() override;
    std::unordered_set<uint16_t> m_CharactersUsed;
};


class ParenExpr : public RegExprNode {
public:
    ParenExpr();
    ~ParenExpr();
    NFA* Translate() override;
    void Print() override;
};

class CharExpr : public RegExprNode {
public:
    CharExpr();
    ~CharExpr();
    NFA* Translate() override;
    NFA* TranslateBracket();
    void Print() override;
    std::unordered_set<uint16_t> m_CharactersUsed;
};

class CharLitExpr : public RegExprNode {
public:
    CharLitExpr();
    ~CharLitExpr();
    uint16_t GetChar();
    NFA* Translate() override;
    void Print() override;
};


class RegexParser {
public:
    /* Public Function */
    RegexParser();
    ~RegexParser();
    void AddTokens(std::vector<Token> tokens);
    RegExprNode* Parse();
    void SetDebugMode(bool on);
    void* AddPointer(void* ptr);
    std::string GetError() { return m_ErrorString; }
private:
    /* Parsing Function */
    RegExprNode* ParseRegEx();
    RegExprNode* ParseOrExpr();
    RegExprNode* ParseStarExpr();
    RegExprNode* ParseIntermedExpr();
    RegExprNode* ParseBracketExpr();
    RegExprNode* ParseDashExpr();
    RegExprNode* ParseParenExpr();
    RegExprNode* ParseCharExpr();
    RegExprNode* ParseCharLitExpr();

    /* Utility Functions */
    void Cleanup();
    std::string GetCurrentTokenString();
    Token* ConsumeToken();
    TokenType PeekToken();
    TokenType PeekNextToken();
    bool AtEnd();
    
    /* Private Variables */
    bool m_DebugMode;
    Token* m_CurrentToken;
    std::vector<void*> m_PointerCleanup;
    RegExprNode* m_Root;
    int m_Loc;
    std::vector<Token> m_Tokens;
    std::string m_ErrorString;
};


#endif