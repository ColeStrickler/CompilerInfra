#include "scanner.hpp"
#include <regex.h>
RegexScanner::RegexScanner(const std::string &regex) : m_Regex(regex), m_CurrIndex(0), m_PrevCharacter(0x0)
{
}

RegexScanner::RegexScanner() : m_CurrIndex(0), m_PrevCharacter(0x0)
{
    
}

RegexScanner::~RegexScanner()
{
}

void RegexScanner::AddRegExString(const std::string &regex)
{
    m_Regex = regex;
}

std::string RegexScanner::GetErrorString()
{
    return std::string(ScannerErrorStrings[m_ErrorCode]);
}

void RegexScanner::ScannerReset()
{
    m_CurrIndex = 0;
    m_PrevCharacter = 0x0;
    m_Regex.clear();
    m_ErrorCode = ERRORCODE::NO_ERROR;
}

std::vector<Token> RegexScanner::GetTokens()
{
    return m_Tokens;
}

bool RegexScanner::ScanRegex()
{
    while (!AtEnd())
    {
        char c = ConsumeChar();
        switch(c)
        {
            case '\\':
            {
                char next = Peek();
                switch(next)
                {
                    case '\\': AddConcatIfNeeded(); ConsumeChar(); m_Tokens.push_back(Token(TokenType::CHARACTER, "\\")); break;
                    case 't': AddConcatIfNeeded(); ConsumeChar(); m_Tokens.push_back(Token(TokenType::CHARACTER, "\t")); break;
                    case '\'': AddConcatIfNeeded(); ConsumeChar(); m_Tokens.push_back(Token(TokenType::CHARACTER, "'")); break;
                    case 'n': AddConcatIfNeeded(); ConsumeChar(); m_Tokens.push_back(Token(TokenType::CHARACTER, "\n")); break;
                    case '_': AddConcatIfNeeded(); ConsumeChar(); m_Tokens.push_back(Token(TokenType::CHARACTER, " ")); break;
                    case '"': AddConcatIfNeeded(); ConsumeChar(); m_Tokens.push_back(Token(TokenType::CHARACTER, "\"")); break;
                    default:
                    {
                        m_ErrorCode = ERRORCODE::INVALID_ESCAPE;
                        return false;
                    }
                }
                break;
            }
            case '[': AddConcatIfNeeded(); m_Tokens.push_back(Token(TokenType::BRACKET_LEFT)); break;
            case ']': m_Tokens.push_back(Token(TokenType::BRACKET_RIGHT)); break;
            case '?': m_Tokens.push_back(Token(TokenType::QUESTION_MARK)); break;
            case '+': m_Tokens.push_back(Token(TokenType::PLUS)); break;
            case '*': m_Tokens.push_back(Token(TokenType::STAR)); break;
            case '|': m_Tokens.push_back(Token(TokenType::OR)); break;
            case '(': AddConcatIfNeeded(); m_Tokens.push_back(Token(TokenType::PARENTHESES_LEFT)); break;
            case ')': m_Tokens.push_back(Token(TokenType::PARENTHESES_RIGHT)); break;
            case '.': AddConcatIfNeeded(); m_Tokens.push_back(Token(TokenType::DOT)); break;
            case '-': m_Tokens.push_back(Token(TokenType::DASH)); break;
            case '^': m_Tokens.push_back(Token(TokenType::CARROT)); break;
            default:
            {
                AddConcatIfNeeded(); 
                std::string text;
                text.push_back(c);
                m_Tokens.push_back(Token(TokenType::CHARACTER, text));
                break;
            }
        }

    }
    return true;
}

void RegexScanner::AddConcatIfNeeded()
{
    auto prevToken = GetLastToken();
    if (prevToken == nullptr)
        return;
    auto type = prevToken->m_Type;

    std::vector<TokenType> needConcats = {TokenType::CHARACTER, TokenType::BRACKET_RIGHT, TokenType::PARENTHESES_RIGHT, \
    TokenType::PLUS, TokenType::QUESTION_MARK, TokenType::DOT, TokenType::STAR};

    if (std::find(needConcats.begin(), needConcats.end(), type) != needConcats.end())
    {
        m_Tokens.push_back(Token(TokenType::CONCAT));
    }
}

Token *RegexScanner::GetLastToken()
{
    if (m_Tokens.size() == 0)
        return nullptr;
    return &m_Tokens[m_Tokens.size() - 1];
}

bool RegexScanner::AtEnd()
{
    return m_CurrIndex >= m_Regex.size();
}

char RegexScanner::ConsumeChar()
{
    if (AtEnd())
    {
        m_ErrorCode = ERRORCODE::CONSUME_AT_END;
        return 0x0;
    }

    return m_Regex[m_CurrIndex++];
}

char RegexScanner::Peek()
{
    if (AtEnd())
    {
        return 0x0;
    }

    
    return m_Regex[m_CurrIndex];
}

Token::Token(TokenType type, const std::string &text) : m_Type(type), m_Lexeme(text)
{
}

Token::Token(TokenType type) : m_Type(type)
{
}
Token::~Token()
{
}
