/* definitions and declarations */
%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {EECS}
%define parser_class_name {Parser}
%define parse.error verbose
%output "parser.cc"
%token-table 

%code requires{
	namespace EECS {
		class Manager;
		class Scanner;
	}
	#include <iostream>
	#include "string.h"
	#include "ast.hpp"

#ifndef YY_NULLPTR
#define YY_NULLPTR 0
#endif
}

%parse-param { Scanner &scanner}
%parse-param { Manager& manager}

%code{
	#include <iostream>
	#include "string.h"
	#include <fstream>
	#include <cstdlib>

	#include "calc.hpp"
	#include <vector>
	#undef yylex
	#define yylex scanner.yylex
}

%union {
	int intVal;
	const char * strVal;

	OpNode* opnode;
	MathNode* mathnode;
}

%token END  0
%token <strVal> ID
%token <intVal> INTLIT
%token LPAR
%token RPAR
%token PLUS
%token MINUS
%token DIV
%token MULT
%token ORATE
%token CALCULATE

%type <vnode> Program
%type <opnode> Ops
%type <opnode> Op
%type <mathnode> Sum
%type <mathnode> Sub
%type <mathnode> Prod
%type <mathnode> Quot
%type <mathnode> Base

%%
Program : Ops END           {
	auto p = new ProgramNode($1->m_Children);
	manager.root = p;
} 
Ops     : Ops Op            {

	$$ = new OpNode;
	for (auto& o: $1->m_Children)
	{
		$$->m_Children.push_back(o);
	}
		
	$$->m_Children.push_back($2);
}
        | Op  {
		$$ = new OpNode();
		$$->m_Children.push_back($1);
		
	}
Op      : CALCULATE ID Sum  {$$ = new CalculateNode($2, $3);} 
        | ORATE Sum         {$$ = new OrateNode("", $2);}
		| ORATE ID Sum		{$$ = new OrateNode($2, $3);} 
		| ORATE ID			{$$ = new OrateNode($2);}
Sum     : Sum PLUS Sub      {$$ = new PlusNode({$1, $3});}
        | Sub		    {$$ = $1;}
Sub     : Sub MINUS Prod    {$$ = new MinusNode({$1, $3});}
        | Prod		    {$$ = $1;}
Prod    : Prod MULT Quot    {$$ = new MultNode({$1, $3});}
        | Quot 		    {$$ = $1;}
Quot    : Quot DIV Base	    {$$ = new DivNode({$1, $3});}
        | Base		    {$$ = $1;}
Base    : INTLIT            {$$ = new NumNode($1);}
        | LPAR Sum RPAR     {$$ = new ParenNode($2);}
        | ID                {$$ = new IDNode($1); }
/* CF-productions and actions */
%%
/* Program stub -- code at end of the parser.cc*/
void 
EECS::Parser::error(const std::string &err_message){
	std::cerr << "Parser Error: " << err_message << "\n";
}
