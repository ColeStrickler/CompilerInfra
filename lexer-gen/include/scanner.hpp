#ifndef REGEX_SCANNER
#define REGEX_SCANNER

#include <string>


enum TokenType
{
    CHARACTER,
    NUMBER,
    CONCAT,
    STAR,
    PLUS,
    CARROT,
    QUESTION_MARK,
    BRACKET_LEFT,
    BRACKET_RIGHT,
    PARENTHESES_LEFT,
    PARENTHESES_RIGHT,
    OR,
    ESCAPE,
};

class Token
{
public:
    Token();
    ~Token();
private:
    std::string m_Lexeme;
    TokenType type;
};




class RegexScanner
{
public:
    RegexScanner(const std::string& regex);
    ~RegexScanner();

private:
    int m_CurrIndex;
    std::string m_Regex;
};





#endif