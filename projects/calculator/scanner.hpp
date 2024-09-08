#ifndef SCANNER_HPP
#define SCANNER_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include <vector>
//#include "frontend.hh"
//#include "errors.hpp"

//using TokenKind = a_lang::Parser::token;



enum TOKENTYPE {
   TOKEN_EOF,
   CROSS,
   STAR,
   INT,
   SLASH,
   DASH,
   LPAREN,
   RPAREN,
   TOKEN_ERR
};

static const char* TokenStrings[] =
{
   "TOKEN_EOF",
   "CROSS",
   "STAR",
   "INT",
   "SLASH",
   "DASH",
   "LPAREN",
   "RPAREN",
   "TOKEN_ERR"
};



struct Position
{
   Position();
   Position(int lineStart, int colStart, int lineEnd, int colEnd);
   Position(const Position& other);
   ~Position();
   std::string toString();
   int m_LineStart;
   int m_ColStart;
   int m_LineEnd;
   int m_ColEnd;
};


class Token {
public:
   Token(TOKENTYPE type, Position pos, const std::string& text);
   ~Token();
   std::string toString();
   inline std::string GetLexeme() { return m_Lexeme; };
   inline TOKENTYPE GetType() {return m_Type; };
private:
   TOKENTYPE m_Type;
   Position m_Position;
   std::string m_Lexeme;
};



namespace Scanner {

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in)
   {
	   m_Line = 1;
	   m_Col = 1;
   };
   ~Scanner() {
      for (auto& tok: m_Tokens)
         delete tok;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   int produceToken();

   Token* GetLastToken();
   inline std::vector<Token*> GetTokens() {return m_Tokens;};
   void outputTokens(std::ostream& outstream);
private:
   size_t m_Line;
   size_t m_Col;
   std::vector<Token*> m_Tokens;
  // a_lang::Parser::semantic_type *yylval = nullptr;

};

} /* end namespace */

#endif /* END __DREWNO_MARS_SCANNER_HPP__ */
