/* Directives */
%option c++ 
%option noyywrap
%option yyclass="MyScanner"


%{
using namespace std;

#include "mylexer.hpp"

#undef YY_DECL
#define YY_DECL int MyLexer::produceToken(Token ** tok)
%}
NOUN ([mM]an)|([wW]oman)|([dD]og)
VERB bites|kicks|loves
ADJECTIVE (lo*ng)|(tall)|(short)
%%
{NOUN} {*tok = new Token(yytext, TokenType::NOUN); return TokenType::NOUN;}
{VERB} {*tok = new Token(yytext, TokenType::VERB); return TokenType::VERB;}
{ADJECTIVE} {*tok = new Token(yytext, TokenType::ADJECTIVE); return TokenType::ADJECTIVE;}
[ \t\n\r]+ { }


. {printf("%s\n", yytext); return TokenType::ERROR;}

%%
/* added to the end of the generated file */
