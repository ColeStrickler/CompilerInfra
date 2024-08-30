#ifndef REGEX_SCANNER
#define REGEX_SCANNER

#include <string>
#include <vector>

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


enum ERROR
{
    CONSUME_AT_END,

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

    
    std::vector<Token> GetTokens();
private:
    void ScanRegex();
    bool AtEnd();
    char ConsumeChar();
    

    int m_CurrIndex;
    char m_PrevCharacter;
    std::string m_Regex;
    std::vector<Token> m_Tokens;
    ERROR m_ErrorCode;
};





#endif