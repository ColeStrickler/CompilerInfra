#ifndef EECS665_cool_lexer
#define EECS665_cool_lexer

#include <istream>

#if !defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

enum TokenType {
	END_OF_FILE,
	NOUN,
	VERB,
	ADJECTIVE,
	ERROR,
};


static std::string TokenTypeString(TokenType type)
{
	switch (type)
	{
		case TokenType::END_OF_FILE:
		{
			return "END_OF_FILE";
		}
		case TokenType::NOUN:
		{
			return "NOUN";
		}
		case TokenType::VERB:
		{
			return "VERB";
		}
		case TokenType::ADJECTIVE:
		{
			return "ADJECTIVE";
		}
		case TokenType::ERROR:
		{
			return "ERROR";
		}
		default:
			return "n/a";
	}
}


class Token{
public:
	Token(std::string lexeme, TokenType type) : m_Lexeme(lexeme), m_Type(type)
	{

	}
	std::string toString(){ return TokenTypeString(m_Type); }
	std::string getLexeme() {return m_Lexeme;}
	TokenType getType() {return m_Type;}
private:
	std::string m_Lexeme;
	TokenType m_Type;
};




class MyLexer : public yyFlexLexer{
public:
	MyLexer(std::istream * in) : yyFlexLexer(in){ }
	int produceToken(Token ** tok);
};

#endif
