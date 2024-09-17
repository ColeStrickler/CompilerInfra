%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{a_lang}
%define api.parser.class {Parser}
%define api.value.type variant
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	namespace a_lang {
		class Scanner;
	}

//The following definition is required when 
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { a_lang::Scanner &scanner }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}


/* Terminals 
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just "transToken", which is defined in
 *  the %union above to mean that the token translation is an instance
 *  of a_lang::Token *, and thus has no fields (other than line and column).
 *  Some terminals, like ID, are "transIDToken", meaning the translation
 *  also has a name field. 
*/
%token                   END	0 "end file"
%token	<a_lang::Token *>       AND
%token	<a_lang::Token *>       ASSIGN
%token	<a_lang::Token *>       ARROW
%token	<a_lang::Token *>       BOOL
%token	<a_lang::Token *>       COLON
%token	<a_lang::Token *>       COMMA
%token	<a_lang::Token *>       CUSTOM
%token	<a_lang::Token *>       DASH
%token	<a_lang::Token *>       ELSE
%token	<a_lang::Token *>       EH
%token	<a_lang::Token *>       EQUALS
%token	<a_lang::Token *>       FALSE
%token	<a_lang::Token *>       FROMCONSOLE
%token	<a_lang::Token *>       GREATER
%token	<a_lang::Token *>       GREATEREQ
%token	<a_lang::IDToken *> ID
%token	<a_lang::Token *>       IF
%token	<a_lang::Token *>       INT
%token	<a_lang::IntLitToken *> INTLITERAL
%token	<a_lang::Token *>       IMMUTABLE
%token	<a_lang::Token *>       LCURLY
%token	<a_lang::Token *>       LESS
%token	<a_lang::Token *>       LESSEQ
%token	<a_lang::Token *>       LPAREN
%token	<a_lang::Token *>       MEANS
%token	<a_lang::Token *>       NOT
%token	<a_lang::Token *>       NOTEQUALS
%token	<a_lang::Token *>       OR
%token	<a_lang::Token *>       OTHERWISE
%token	<a_lang::Token *>       CROSS
%token	<a_lang::Token *>       POSTDEC
%token	<a_lang::Token *>       POSTINC
%token	<a_lang::Token *>       RETURN
%token	<a_lang::Token *>       RCURLY
%token	<a_lang::Token *>       REF
%token	<a_lang::Token *>       RPAREN
%token	<a_lang::Token *>       SEMICOL
%token	<a_lang::Token *>       SLASH
%token	<a_lang::Token *>       STAR
%token	<a_lang::StrToken *>    STRINGLITERAL
%token	<a_lang::Token *>       TOCONSOLE
%token	<a_lang::Token *>       TRUE
%token	<a_lang::Token *>       VOID
%token	<a_lang::Token *>       WHILE


/* NOTE: Make sure to add precedence and associativity 
 * declarations
 */


%right NOT 
%left STAR
%left SLASH
%left CROSS
%left DASH
%left AND
%left OR
%right ASSIGN
%nonassoc NOTEQUALS GREATER GREATEREQ LESS LESSEQ EQUALS



%%

program : globals
		{
		//For the project, we will only be checking std::cerr for 
		// correctness. You might choose to uncomment the following
		// Line to help you debug, which will print when this
		// production is applied
		//std::cout << "got to the program ::= globals rule\n";
		}

globals	: globals decl
	  	{
	  	}
		| /* epsilon */
		{
		}

decl	: varDecl SEMICOL
		{

		}
		| classTypeDecl
		{

		}
		| fnDecl
		{

		}




varDecl	: name COLON type
		{

		}
		| name COLON type ASSIGN exp
		{

		}


type	: datatype
		{

		}
		| IMMUTABLE datatype
		{

		}


datatype	: REF primtype
			{

			}
			| primtype
			{

			}
			| REF name
			{

			}
			| name
			{

			}

primtype	: INT
			{

			}
			| BOOL
			{

			}
			| VOID
			{
				
			}


classTypeDecl	: classBody varDecl SEMICOL
				{

				}
				| classBody fnDecl
				{

				}
				| /* epsilon */
				{

				}


classBody	: classBody varDecl SEMICOL
			{

			}
			| classBody fnDecl
			{

			}
			| /* epsilon */
			{

			}


fnDecl	: name COLON CUSTOM LCURLY classBody RCURLY SEMICOL
		{

		}


maybeformals	: formalsList
				{

				}
				| /* epsilon */
				{

				}
				
				
				
formalsList	: formalDecl
			{
			}
			formalsList COMMA formalDecl
			{

			}

formalDecl	: name COLON type


stmtList	: stmtList stmt SEMICOL
			{

			}
			| stmtList blockStmt
			{

			}
			| /* epsilon */
			{

			}

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
			{

			}
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
			{

			}
			| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
	

stmt	: varDecl
		{

		}
		| loc ASSIGN exp
		{

		}
		| loc POSTDEC
		{

		}
		| loc POSTINC
		{

		}
		| TOCONSOLE
		{

		}
		| FROMCONSOLE
		{

		}
		| loc MEANS exp OTHERWISE exp
		{

		}
		| RETURN exp
		{

		}
		| RETURN
		{

		}


exp	: exp DASH exp
	{

	}
	| exp CROSS exp
	{

	}
	| exp STAR exp
	{

	}
	| exp SLASH exp
	{

	}
	| exp AND exp
	{

	}
	| exp OR exp
	{

	}
	| exp EQUALS exp
	{

	} 
	| exp NOTEQUALS exp
	{

	}
	| exp GREATER exp
	{

	}
	| exp GREATEREQ exp
	{

	}
	| exp LESS exp
	{

	}
	| exp LESSEQ exp
	{

	}
	| NOT exp
	{

	}
	| DASH term
	{

	}
	| term
	{

	}


callExp	: loc LPAREN RPAREN
		{

		}
		| loc LPAREN actualsList RPAREN
		{

		}


actualsList	: exp
			{

			}
			| actualsList COMMA exp
			{

			}


term 	: loc
		{

		}
		| INTLITERAL
		{

		}
		| STRINGLITERAL
		{

		}
		| TRUE
		{

		}
		| FALSE
		{

		}
		| EH
		{

		}
		| LPAREN exp RPAREN
		{

		}
		| callExp
		{

		}

loc	: name
	{

	}
	| loc ARROW name


name : ID
	






id		: ID
		  {
		  }
 /* TODO: add productions for the entire grammar of the language */
	
%%

void a_lang::Parser::error(const std::string& msg){
	//For the project, we will only be checking std::cerr for 
	// correctness. You might choose to uncomment the following
	// Line to help you debug, since it gives slightly more 
	// descriptive error messages 
	//std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}