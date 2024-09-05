%{
#include <string>
#include <limits.h>

/* Get our custom yyFlexScanner subclass */
#include "scanner.hpp"
#include "tokens.hpp"
#undef  YY_DECL
#define YY_DECL int a_lang::Scanner::yylex( a_lang::Parser::semantic_type * const lval )

/* define yyterminate as this instead of NULL */
#define yyterminate() return( TokenKind::END )

/* Exclude unistd.h for Visual Studio compatability. */
#define YY_NO_UNISTD_H

/* typedef to make the reference to token kinds shorter */
using TokenKind = a_lang::Parser::token;

%}

%option debug
%option nodefault
%option yyclass="a_lang::Scanner"
%option noyywrap
%option c++

DIGIT [0-9]
INTEGER (DIGIT)+
IDENTIFIER [a-zA-Z_](DIGIT | [a-zA-Z] | _)*
STRING \"(\\n | \\t | \\" | \\ | [^"\\])\"
COMMENT \#.*
WHITESPACE [ \t\r]+
ASSIGN =
COLON :
COMMA ,
CROSS +
DASH -    
EQUALS ==
GREATER >
GREATEREQ >=
LCURLY {
LESS <
LESSEQ <=    
LPAREN (
NOT !
REF &
NOTEQUALS !=
POSTDEC --
POSTINC ++    
RCURLY }
RPAREN )
SEMICOL ;
SLASH /
STAR *
ARROW ->  



%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}


IDENTIFIER {
	auto tokenKind = TokenKind::ID;
	auto pos = new Position(lineNum, colNum, lineNum, colNum+strlen(yytext));
	auto tok = new IDToken(pos, yytext);
	this->yylval->emplace<a_lang::Token *>(tok);
	colNum++;
	return tokenKind;
}

STRING {
	int tokenKind = TokenKind::STRINGLITERAL;
	auto pos = new Position(lineNum, colNum, lineNum, colNum+strlen(yytext));
	auto tok = new StrToken(pos, yytext);
	this->yylval->emplace<a_lang::Token *>(tok);
	colNum++;
	return tokenKind;
}

COMMENT {}
WHITESPACE {}

    
    
    
ASSIGN 	{
            int tokenKind = TokenKind::ASSIGN;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
COLON 	{
            int tokenKind = TokenKind::COLON;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
COMMA 	{
            int tokenKind = TokenKind::COMMA;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
CROSS 	{
            int tokenKind = TokenKind::CROSS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
DASH 	{
            int tokenKind = TokenKind::DASH;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
EQUALS 	{
            int tokenKind = TokenKind::EQUALS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
GREATER {
            int tokenKind = TokenKind::GREATER;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
GREATEREQ {
            int tokenKind = TokenKind::GREATEREQ;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
LCURLY 	{
            int tokenKind = TokenKind::LCURLY;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
LESS 	{
            int tokenKind = TokenKind::LESS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
LESSEQ {
            int tokenKind = TokenKind::LESSEQ;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
LPAREN {
            int tokenKind = TokenKind::LPAREN;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
NOT 	{
            int tokenKind = TokenKind::NOT;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
REF 	{
            int tokenKind = TokenKind::REF;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
NOTEQUALS {
            int tokenKind = TokenKind::NOTEQUALS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
POSTDEC {
            int tokenKind = TokenKind::POSTDEC;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
POSTINC {
            int tokenKind = TokenKind::POSTINC;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
RCURLY 	{
            int tokenKind = TokenKind::RCURLY;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
RPAREN 	{
            int tokenKind = TokenKind::RPAREN;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
SEMICOL {
            int tokenKind = TokenKind::SEMICOL;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
SLASH 	{
            int tokenKind = TokenKind::SLASH;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
STAR 	{
            int tokenKind = TokenKind::STAR;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
ARROW {
            int tokenKind = TokenKind::ARROW;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
    }


INTEGER	{
		//TODO: The following computation of the integer
		// value does not check for overflow. It should do so.
		int intVal = atoi(yytext);


		//Note that the IntLitToken does not take a TokenKind argument
		// since it's implicitly known that the token represents an
		// INTLITERAL		
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		Token * tok = new IntLitToken(pos, intVal);
		yylval->emplace<a_lang::Token *>(tok);

		//TODO: update the colNum count according to the number of
		// characters consumed.
		colNum += strlen(yytext); 

		//As before, alert the client program that an INTLITERAL 
		// token was matched
		return TokenKind::INTLITERAL;
		}

\n		{
		//Update the line number, reset the columnNumber
		lineNum++;
		colNum = 1;
		}






.   {
		//TODO: Add the rest of the rules above this point so that legal characters
		// are properly accounted for. 
		std::string msg = "Illegal character ";
		msg += yytext;
		Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+1);
		Report::fatal(pos,msg);
		colNum += strlen(yytext);
    }
%%
