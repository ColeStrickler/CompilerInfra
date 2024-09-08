%{
#include <string>
#include <limits.h>
#include <iostream>
#include <algorithm> // For std::count
#include <cstdlib>   // For std::exit

#include "scanner.hpp"


/* define yyterminate as this instead of NULL */

/* Exclude unistd.h for Visual Studio compatability. */
#define YY_NO_UNISTD_H

/* typedef to make the reference to token kinds shorter */
#undef YY_DECL
#define YY_DECL int Scanner::Scanner::produceToken()
%}

%option debug
%option nodefault
%option yyclass="Scanner::Scanner"
%option noyywrap
%option c++



INTEGER [0-9]+
PLUS \+
DASH \-
LPAREN \(
RPAREN \)
STAR \*
SLASH \/



%%

{INTEGER} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::INT, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::INT;
}

{PLUS} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::CROSS, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::CROSS;
}

{DASH} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::DASH, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::DASH;
}

{LPAREN} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::LPAREN, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::LPAREN;
}


{RPAREN} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::RPAREN, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::RPAREN;
}

{STAR} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::STAR, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::STAR;
}

{SLASH} {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::SLASH, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::SLASH;
}

\n {

}

. {
    Position pos(m_Line, m_Col, m_Line, m_Col+strlen(yytext));
    auto tok = new Token(TOKENTYPE::SLASH, pos, yytext);
    m_Col += strlen(yytext);
    m_Tokens.push_back(tok);
    return TOKENTYPE::TOKEN_ERR;
}

%%