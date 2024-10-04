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
	#include "ast.hpp"
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
%parse-param { a_lang::ProgramNode** root }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

//%define parse.assert

/* Terminals 
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just Token pointers, 
 *  and thus have no fields (other than line and column).
 *  Some terminals, like ID, have the translation type IDToken *,
 *  meaning the translation also has a name field. 
*/
%token                     END   0 "end file"
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
%token	<a_lang::IDToken *>     ID
%token	<a_lang::Token *>       IF
%token	<a_lang::Token *>       INT
%token	<a_lang::IntLitToken *> INTLITERAL
%token	<a_lang::Token *>       IMMUTABLE
%token	<a_lang::Token *>       LCURLY
%token	<a_lang::Token *>       LESS
%token	<a_lang::Token *>       LESSEQ
%token	<a_lang::Token *>       LPAREN
%token	<a_lang::Token *>       MAYBE
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

/* Nonterminals
*  The specifier in angle brackets
*  indicates the type of the translation attribute.
*  TODO: You will need to add more attributes for other nonterminals
*  to this list as you add productions to the grammar
*  below (along with indicating the appropriate translation
*  attribute type).
*/
/*       (attribute type)    (nonterminal)    */
%type <a_lang::ProgramNode *> program
%type <std::list<a_lang::DeclNode*>*> globals
%type <a_lang::DeclNode*> decl
%type <a_lang::VarDeclNode*> varDecl
%type <a_lang::TypeNode*> type
%type <std::vector<a_lang::DeclNode*>> maybeFormals
%type <std::vector<a_lang::DeclNode*>> formalList
%type <a_lang::TypeNode*> datatype
%type <a_lang::TypeNode*> primType
%type <a_lang::FnDeclNode*> fnDecl
%type <a_lang::FormalDeclNode*> formalDecl
%type <a_lang::StmtListNode*> stmtList
%type <a_lang::StmtNode*> stmt
%type <a_lang::LocNode*> loc
%type <a_lang::IDNode*> name
%type <a_lang::ExpNode*> exp
%type <a_lang::ExpNode*> term
%type <a_lang::CallExpNode*> callExp
%type <std::vector<a_lang::ExpNode*>> actualList
%type <std::vector<a_lang::DeclNode*>> classBody
%type <a_lang::ClassDeclNode*> classTypeDecl

%right ASSIGN
%left OR
%left AND
%nonassoc LESS GREATER LESSEQ GREATEREQ EQUALS NOTEQUALS
%left DASH CROSS
%left STAR SLASH
%left NOT 






%%


program		: globals
		  {
		  $$ = new ProgramNode($1);
		  *root = $$;
		  }

globals		: globals decl
		  {
		  $$ = $1;
		  DeclNode * declNode = $2;
		  $$->push_back(declNode);
		  }
		| /* epsilon */
		  {
		  $$ = new std::list<DeclNode *>();
		  }

decl		: varDecl SEMICOL
		  {
		  	$$ = $1;
			$1->m_PrintSemicolon = true;
		  }
		| classTypeDecl
		  {
			$$ = $1;
		  }
		| fnDecl
		  {
			$$ = $1;
		  }

varDecl		: name COLON type
		  {
		  const Position * p;
		  p = new Position($1->pos(), $2->pos());
		  $$ = new VarDeclNode(p, $1, $3);
		  }
		| name COLON type ASSIGN exp
		  {
		  }

type		: IMMUTABLE datatype
		  {
			const Position * p;
		  	p = new Position($1->pos(), $2->pos());
			$$ = new ImmutableTypeNode(p, $2);
		  }
		| datatype
		  {
		  $$ = $1;
		  }

datatype	: REF primType
		  {
			$$ = new RefTypeNode($1->pos(), $2);
		  }
		| primType
		  {
		  $$ = $1;
		  }
		| REF name
		  {
			auto classType = new ClassTypeNode($2->pos(), $2);
			$$ = new RefTypeNode($1->pos(), classType);
		  }
		| name
		  {
			$$ = new ClassTypeNode($1->pos(), $1);
		  }

primType	: INT
		  {
		  $$ = new IntTypeNode($1->pos());
		  }
		| BOOL
		  {
			$$ = new BoolTypeNode($1->pos());
		  }
		| VOID
		  {
			$$ = new VoidTypeNode($1->pos());
		  }

classTypeDecl 	: name COLON CUSTOM LCURLY classBody RCURLY SEMICOL
		  {
			$$ = new ClassDeclNode($1->pos(), $1);
			$$->AddDecls($5);
			
		  }

classBody 	: classBody varDecl SEMICOL
		  {
			$$ = std::vector<DeclNode*>();
			$$.insert($$.end(), $1.begin(), $1.end());
			$2->m_PrintSemicolon = true;
			$$.push_back($2);

		  }
		| classBody fnDecl
		  {
			$$ = std::vector<DeclNode*>();
			$$.insert($$.end(), $1.begin(), $1.end());
			$$.push_back($2);
		  }
		| /* epsilon */
		  {
			$$ = {};
		  }

fnDecl 		: name COLON LPAREN maybeFormals RPAREN ARROW type LCURLY stmtList RCURLY
		  {
			$$ = new FnDeclNode($1->pos(), $1, $7);
			$$->AddFormals($4);
			$$->AddStatements($9);
		  }

maybeFormals	: /* epsilon */
		  {
			$$ = {};
		  }
		| formalList
		  {
			$$ = $1;
		  }

formalList	: formalDecl
		  {
			$$ = std::vector<DeclNode*>();
			$$.push_back($1);
		  }
		| formalList COMMA formalDecl
		  {
			$$ = std::vector<DeclNode*>();
			$$.insert($$.end(), $1.begin(), $1.end());
			$$.push_back($3);
		  }

formalDecl	: name COLON type
		  {
			$$ = new FormalDeclNode($1->pos(), $1, $3);
		  }

stmtList	: /* epsilon */
		  {
			$$ = new StmtListNode(nullptr, false);
		  }
		| stmtList stmt SEMICOL
		  {
			auto pos = $1->pos() == nullptr ? $2->pos() : $1->pos();
			$$ = new StmtListNode(pos, true);
			printf("stmt list\n");
			$$->AddChildren($1->m_Children);
			$$->AddChild($2);
		  }
		| stmtList blockStmt
		  {
		  }

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
		  {
		  }

stmt		: varDecl
		  {
			$$ = $1;
		  }
		| loc ASSIGN exp
		  {
			printf("returning assignstmtnode\n");
			$$ = new AssignStmtNode($1->pos(), $1, $3);
		  }
		| callExp
		  {
			$$ = new CallStmtNode($1->pos(), $1);
		  }
		| loc POSTDEC
		  {
		  }
		| loc POSTINC
		  {
		  }
		| TOCONSOLE exp
		  {
		  }
		| FROMCONSOLE loc
		  {
		  }
		| MAYBE exp MEANS exp OTHERWISE exp
		  {
		  }
		| RETURN exp
		  {
			$$ = new ReturnStmtNode($1->pos(), $2);
		  }
		| RETURN
		  {
			$$ = new ReturnStmtNode($1->pos(), nullptr);
		  }


exp		: exp DASH exp
		  {
			$$ = new MinusNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp CROSS exp
		  {
			$$ = new PlusNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp STAR exp
		  {
			$$ = new TimesNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp SLASH exp
		  {
			$$ = new DivideNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp AND exp
		  {
			$$ = new AndNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp OR exp
		  {
			$$ = new OrNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp EQUALS exp
		  {
			$$ = new EqualsNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp NOTEQUALS exp
		  {
			$$ = new NotNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp GREATER exp
		  {
			$$ = new GreaterNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp GREATEREQ exp
		  {
			$$ = new GreaterEqNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp LESS exp
		  {
			$$ = new LessNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| exp LESSEQ exp
		  {
			$$ = new LessEqNode($1->pos());
			$$->AddExpressions($1, $3);
		  }
		| NOT exp
		  {
			$$ = new NotNode($1->pos());
			$$->AddChild($2);
		  }
		| DASH term
		  {
			$$ = new NegationNode($1->pos());
			$$->AddChild($2);
		  }
		| term
		  {
			$$ = $1;
			printf("returning term\n");
		  }

callExp		: loc LPAREN RPAREN
		  {
			$$ = new CallExpNode($1->pos(), $1);
		  }
		| loc LPAREN actualList RPAREN
		  {
			$$ = new CallExpNode($1->pos(), $1);
			$$->AddExpressions($3);
		  }

actualList	: exp
		  {
			$$ = {$1};
		  }
		| actualList COMMA exp
		  {
			$$ = std::vector<ExpNode*>();
			$$.insert($$.end(), $1.begin(), $1.end());
			$$.push_back($3);
		  }

term 		: loc
		  {
			$$ = $1;
		  }
		| INTLITERAL
		  {
			$$ = new IntLitNode($1->pos(), $1->num());
		  }
		| STRINGLITERAL
		  {
			$$ = new StrLitNode($1->pos(), $1->str());
		  }
		| TRUE
		  {
			$$ = new TrueNode($1->pos());
		  }
		| FALSE
		  {
			$$ = new FalseNode($1->pos());
		  }
		| EH
		  {
		  }
		| LPAREN exp RPAREN
		  {
			printf("LPAREN exp RPAREN\n");
			$$ = $2;
			$$->m_InsideParentheses = true;
		  }
		| callExp 
		  {
			$$ = $1;
		  }

loc		: name
		  {
			$$ = new LocNode($1->pos(), $1, nullptr);
		  }
		| loc ARROW name
		  {
			$$ = new LocNode($1->pos(), $1, $3);
		  }

name		: ID
		  {
				
                const Position * pos = $1->pos();
	 	  		$$ = new IDNode(pos, $1->value());
		  }
	
%%

void a_lang::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
