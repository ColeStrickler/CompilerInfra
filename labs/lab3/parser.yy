/* definitions and declarations */
%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {EECS}
%define parser_class_name {Parser}
%output "parser.cc"
%token-table

%code requires {
	namespace EECS {
		class Manager;
		class Scanner;
		class MathNode;
		class MultNode;
		class DivNode;
		class PlusNode;
		class SubNode;
		class IntNode;
	}

#ifndef YY_NULLPTR
#define YY_NULLPTR 0
#endif
}

%parse-param { Scanner &scanner}
%parse-param { Manager &manager}

%code{
	#include <iostream>
	#include "string.h"
	#include <fstream>
	#include <cstdlib>
	#include "mathnode.hpp"

	#include "calc.hpp"

	#undef yylex
	#define yylex scanner.yylex
}

%union {
	int intVal;
	MathNode* transNode;
	bool transGood;
}

%token END 0
%token LPAR
%token RPAR
%token PLUS
%token MINUS
%token DIV
%token MULT
%token <intVal> INTLIT

%type <transGood> Equation
%type <transNode> Sum
%type <transNode> Sub
%type <transNode> Prod
%type <transNode> Quot
%type <transNode> Base


%%
Equation: Sum END {
	$$ = true;
	MathNode* child = $1;
	std::cout << "digraph dfa{\n" << child->Print();
	std::cout << "}\n";
}
Sum 	: Sum PLUS Sub 		{
	$$ = new PlusNode();
	$$->AddChild("arm1", $1);
	$$->AddChild("arm2", $3);
}
		| Sub				{$$ = $1;}
Sub 	: Sub MINUS Prod	{
	$$ = new SubNode();
	$$->AddChild("arm1", $1);
	$$->AddChild("arm2", $3);
}
		| Prod				{$$ = $1;}
Prod 	: Prod MULT Quot	{
	$$ = new MultNode();
	$$->AddChild("arm1", $1);
	$$->AddChild("arm2", $3);
}
		| Quot				{$$ = $1;}
Quot	: Quot DIV Base		{
	$$ = new DivNode();
	$$->AddChild("arm1", $1);
	$$->AddChild("arm2", $3);
}
		| Base				{$$ = $1;}
Base	: INTLIT			{
	$$ = new IntNode($1);
	}
		| LPAR Sum RPAR		{$$ = $2;}


/* CF-productions and actions */
%%
/* Program stub -- code at end of the parser.cc*/
void
EECS::Parser::error(const std::string &err_message){
	std::cerr << "Parser Error: " << err_message << "\n";
}
