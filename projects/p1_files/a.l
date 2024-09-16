%{
#include <string>
#include <limits.h>
#include <iostream>
#include <algorithm> // For std::count
#include <cstdlib>   // For std::exit

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
INTEGER [0-9]+
IDENTIFIER [a-zA-Z_]([a-zA-Z0-9_])*
STRING \"(\\[nt\\"\\]|[^"\\])*\"
UNTERMSTRING \"(\\[nt\\"\\]|[^"\\])*[^\"\\]
BADESCAPESTRLIT \"((\\[^nt\\"\\])|[^"\\])*\"
BADESCAPESANDUTERM \"((\\[^nt\\"\\])|[^"\\])*[^\"\\]
COMMENT \#.*
WHITESPACE " "
ASSIGN =
COLON :
COMMA ,
CROSS \+
DASH \-    
EQUALS ==
GREATER >
GREATEREQ >=
LCURLY \{
LESS <
LESSEQ <=    
LPAREN \(
NOT !
REF &
NOTEQUALS !=
POSTDEC --
POSTINC \+\+    
RCURLY \}
RPAREN \)
SEMICOL ;
SLASH \/
STAR \*
ARROW ->  



%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}




{IDENTIFIER} {

	int tokenKind;
	Token* tok;
	auto pos = new Position(lineNum, colNum, lineNum, colNum+strlen(yytext));
	if (isKeyword(yytext))
	{
		tokenKind = GetKeywordTokenKind(yytext);
		tok = new Token(pos, tokenKind);
	}
	else 
	{
		tokenKind = TokenKind::ID;
		tok = new IDToken(pos, yytext);
	}
	
	this->yylval->emplace<a_lang::Token *>(tok);
	colNum += strlen(yytext);
	return tokenKind;
}

{STRING} {
	int tokenKind = TokenKind::STRINGLITERAL;
	auto pos = new Position(lineNum, colNum, lineNum, colNum+strlen(yytext));
	auto tok = new StrToken(pos, yytext);
	this->yylval->emplace<a_lang::Token *>(tok);
	colNum += strlen(yytext);
	return tokenKind;
}

{UNTERMSTRING} {
        
        std::string text(yytext);
        int lines = std::count(text.begin(), text.end(), '\n');
        size_t rfind = text.rfind('\n');
        int cols = rfind == std::string::npos ? text.size() : text.size() - rfind;
        lineNum += lines;
        
        if (lines > 0)
                colNum = 1;
        auto pos = new Position(lineNum, colNum, lineNum, colNum+cols);
        colNum += cols;

        errStrUnterm(pos);
        return TokenKind::YYEMPTY;
}

{BADESCAPESTRLIT} {
        auto pos = new Position(lineNum, colNum, lineNum, colNum+strlen(yytext));
        colNum += strlen(yytext);
        errStrEsc(pos);
        return TokenKind::YYEMPTY;
}


{BADESCAPESANDUTERM} {
        std::string text(yytext);
        int lines = std::count(text.begin(), text.end(), '\n');
        size_t rfind = text.rfind('\n');
        int cols = rfind == std::string::npos ? text.size() : text.size() - rfind;
        lineNum += lines;
        
        if (lines > 0)
                colNum = 1;
        auto pos = new Position(lineNum, colNum, lineNum, colNum+cols);
        colNum += cols;

        errStrEscAndUnterm(pos);
        return TokenKind::YYEMPTY;
}

{COMMENT} {}
{WHITESPACE} {colNum++;}

    
    
    
{ASSIGN} 	{
            int tokenKind = TokenKind::ASSIGN;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{COLON} 	{
            int tokenKind = TokenKind::COLON;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{COMMA} 	{
            int tokenKind = TokenKind::COMMA;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{CROSS} {
        int tokenKind = TokenKind::CROSS;
        auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
        auto tok = new Token(pos, tokenKind);
        this->yylval->emplace<a_lang::Token *>(tok);
        colNum++;
        return tokenKind;
}
    
{DASH} 	{
            int tokenKind = TokenKind::DASH;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{EQUALS} 	{
            int tokenKind = TokenKind::EQUALS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
        }
    
{GREATER} {
            int tokenKind = TokenKind::GREATER;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{GREATEREQ} {
            int tokenKind = TokenKind::GREATEREQ;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
        }
    
{LCURLY} 	{
            int tokenKind = TokenKind::LCURLY;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{LESS} 	{
            int tokenKind = TokenKind::LESS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{LESSEQ} {
            int tokenKind = TokenKind::LESSEQ;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
        }
    
{LPAREN} {
            int tokenKind = TokenKind::LPAREN;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{NOT} 	{
            int tokenKind = TokenKind::NOT;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{REF} 	{
            int tokenKind = TokenKind::REF;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{NOTEQUALS} {
            int tokenKind = TokenKind::NOTEQUALS;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
        }
    
{POSTDEC} {
            int tokenKind = TokenKind::POSTDEC;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
        }
    
{POSTINC} {
            int tokenKind = TokenKind::POSTINC;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
        }
    
{RCURLY} 	{
            int tokenKind = TokenKind::RCURLY;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{RPAREN} 	{
            int tokenKind = TokenKind::RPAREN;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{SEMICOL} {
            int tokenKind = TokenKind::SEMICOL;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{SLASH} 	{
            int tokenKind = TokenKind::SLASH;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{STAR} 	{
            int tokenKind = TokenKind::STAR;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum++;
            return tokenKind;
        }
    
{ARROW}  {
            int tokenKind = TokenKind::ARROW;
            auto pos = new Position(lineNum, colNum, lineNum, colNum+1);
            auto tok = new Token(pos, tokenKind);
            this->yylval->emplace<a_lang::Token *>(tok);
            colNum += 2;
            return tokenKind;
    }


{INTEGER} {
		//TODO: The following computation of the integer
		// value does not check for overflow. It should do so.ER
                Position * pos = new Position(
			lineNum, colNum, lineNum, colNum+strlen(yytext));
		std::string text = std::string(yytext);
		if (std::stoull(text) > 2147483648)
		{
                        colNum += strlen(yytext);
			errIntOverflow(pos);
			return TokenKind::YYEMPTY;
		}


		int intVal = atoi(yytext);


		//Note that the IntLitToken does not take a TokenKind argument
		// since it's implicitly known that the token represents an
		// INTLITERAL		
		
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
