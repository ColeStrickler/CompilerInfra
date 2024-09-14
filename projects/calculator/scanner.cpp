#include "scanner.hpp"


Position::Position()
{
}

Position::Position(int lineStart, int colStart, int lineEnd, int colEnd) : m_LineStart(lineStart), m_ColStart(colStart), m_LineEnd(lineEnd), m_ColEnd(colEnd)
{
}

Position::Position(const Position &other): m_LineStart(other.m_LineStart), m_ColStart(other.m_ColStart), m_LineEnd(other.m_LineEnd), m_ColEnd(other.m_ColEnd)
{

}

Position::~Position()
{
}

std::string Position::toString()
{
    return "[(" + std::to_string(m_LineStart) + "," + std::to_string(m_ColStart) + ") - (" + std::to_string(m_LineEnd) + "," + std::to_string(m_ColEnd) + ")]"; 
}

Token::Token(TOKENTYPE type, Position pos, const std::string& text) : m_Type(type), m_Position(pos), m_Lexeme(text)
{
}

Token::Token(TokenType type, std::string text)
{

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

std::string Token::toString()
{
    return m_Position.toString() + ": " + TokenStrings[m_Type]; 
}

Token *Scanner::Scanner::GetLastToken()
{
    return m_Tokens.size() == 0 ? nullptr : m_Tokens[m_Tokens.size() - 1];
}
