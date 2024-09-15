#ifndef REGEX_SCANNER
#define REGEX_SCANNER

#include <string>
#include <vector>
#include <algorithm>



/*

*/
enum TokenType
{
    CHARACTER,
    CONCAT,
    STAR,
    PLUS,
    DOT,
    DASH,
    CARROT,
    QUESTION_MARK,
    BRACKET_LEFT,
    BRACKET_RIGHT,
    PARENTHESES_LEFT,
    PARENTHESES_RIGHT,
    OR,
};


static const char* TokenTypeStrings[] = 
{
    "CHARACTER",
    "CONCAT",
    "STAR",
    "PLUS",
    "DOT",
    "DASH",
    "CARROT",
    "QUESTION_MARK",
    "BRACKET_LEFT",
    "BRACKET_RIGHT",
    "PARENTHESES_LEFT",
    "PARENTHESES_RIGHT",
    "OR",
};



enum ERROR
{
    NO_ERROR,
    CONSUME_AT_END,
    INVALID_ESCAPE,
};

static const char* ScannerErrorStrings[] = {
    "CONSUME_AT_END",
    "INVALID_ESCAPE",
};



class Token
{
public:
    Token(TokenType type, const std::string& text);
    Token(TokenType type);
    ~Token();
    std::string toString() { return std::string(TokenTypeStrings[m_Type]); }
    std::string m_Lexeme;
    TokenType m_Type;
private:
    
};




class RegexScanner
{
public:
    RegexScanner(const std::string& regex);
    RegexScanner();
    ~RegexScanner();

    void AddRegExString(const std::string& regex);
    void ScannerReset();
    std::string GetErrorString();
    std::vector<Token> GetTokens();
    bool ScanRegex();
private:
    char Peek();  
    bool AtEnd();
    char ConsumeChar();
    void AddConcatIfNeeded();    
    Token* GetLastToken();

    int m_CurrIndex;
    char m_PrevCharacter;
    std::string m_Regex;
    std::vector<Token> m_Tokens;
    ERROR m_ErrorCode;
};



#endif