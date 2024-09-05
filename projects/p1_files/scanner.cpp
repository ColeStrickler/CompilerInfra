#include <fstream>
#include "scanner.hpp"

using namespace a_lang;

using TokenKind = a_lang::Parser::token;
using Lexeme = a_lang::Parser::semantic_type;

static std::unordered_map<std::string, TokenKind::token_kind_type> keyword_Tok = {
	{"and", TokenKind::AND},
	{"bool", TokenKind::BOOL},
	{"custom", TokenKind::CUSTOM},
	{"else", TokenKind::ELSE},
	{"eh", TokenKind::EH},
	{"false", TokenKind::FALSE},
	{"fromconsole", TokenKind::FROMCONSOLE},
	{"if", TokenKind::IF},
	{"immutable", TokenKind::IMMUTABLE},
	{"int", TokenKind::INT},
	{"or", TokenKind::OR},
	{"otherwise", TokenKind::OTHERWISE},
	{"means", TokenKind::MEANS},
	{"toconsole", TokenKind::TOCONSOLE},
	{"return", TokenKind::RETURN},
	{"true", TokenKind::TRUE},
	{"void", TokenKind::VOID},
	{"while", TokenKind::WHILE}
};


void Scanner::outputTokens(std::ostream& outstream){
	Lexeme lastMatch;
	//Token * t = lex.as<Token *>();
	int tokenKind;
	while(true){
		tokenKind = this->yylex(&lastMatch);
		if (tokenKind == TokenKind::END){
			outstream << "EOF" 
			  << " [" << this->lineNum 
			  << "," << this->colNum << "]"
			  << std::endl;
			return;
		} 
		else {
			outstream << lastMatch.as<Token *>()->toString()
			  << std::endl;
		}
	}
}

TokenKind::token_kind_type a_lang::Scanner::GetKeywordTokenKind(const std::string &keyword)
{
   return keyword_Tok[keyword];
}
