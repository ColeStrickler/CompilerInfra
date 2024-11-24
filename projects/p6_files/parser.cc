// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "frontend.hh"


// Unqualified %code blocks.
#line 35 "a.yy"

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

#line 63 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "a.yy"
namespace a_lang {
#line 137 "parser.cc"

  /// Build a parser object.
  Parser::Parser (a_lang::Scanner &scanner_yyarg, a_lang::ProgramNode** root_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      root (root_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      case symbol_kind::S_callExp: // callExp
        value.copy< a_lang::CallExpNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decl: // decl
        value.copy< a_lang::DeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        value.copy< a_lang::ExpNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        value.copy< a_lang::FnDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        value.copy< a_lang::FormalDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_name: // name
        value.copy< a_lang::IDNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
        value.copy< a_lang::IDToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        value.copy< a_lang::IntLitToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_loc: // loc
        value.copy< a_lang::LocNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
        value.copy< a_lang::ProgramNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        value.copy< a_lang::StmtNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        value.copy< a_lang::StrToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        value.copy< a_lang::Token * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        value.copy< a_lang::TypeNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_varDecl: // varDecl
        value.copy< a_lang::VarDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_globals: // globals
        value.copy< std::list<a_lang::DeclNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_actualsList: // actualsList
        value.copy< std::list<a_lang::ExpNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        value.copy< std::list<a_lang::FormalDeclNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmtList: // stmtList
        value.copy< std::list<a_lang::StmtNode *> * > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_callExp: // callExp
        value.move< a_lang::CallExpNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_decl: // decl
        value.move< a_lang::DeclNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        value.move< a_lang::ExpNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        value.move< a_lang::FnDeclNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        value.move< a_lang::FormalDeclNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_name: // name
        value.move< a_lang::IDNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ID: // ID
        value.move< a_lang::IDToken * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        value.move< a_lang::IntLitToken * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_loc: // loc
        value.move< a_lang::LocNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_program: // program
        value.move< a_lang::ProgramNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        value.move< a_lang::StmtNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        value.move< a_lang::StrToken * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        value.move< a_lang::Token * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        value.move< a_lang::TypeNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_varDecl: // varDecl
        value.move< a_lang::VarDeclNode * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_globals: // globals
        value.move< std::list<a_lang::DeclNode *> * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_actualsList: // actualsList
        value.move< std::list<a_lang::ExpNode *> * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        value.move< std::list<a_lang::FormalDeclNode *> * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_stmtList: // stmtList
        value.move< std::list<a_lang::StmtNode *> * > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_callExp: // callExp
        value.YY_MOVE_OR_COPY< a_lang::CallExpNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decl: // decl
        value.YY_MOVE_OR_COPY< a_lang::DeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        value.YY_MOVE_OR_COPY< a_lang::ExpNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        value.YY_MOVE_OR_COPY< a_lang::FnDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        value.YY_MOVE_OR_COPY< a_lang::FormalDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_name: // name
        value.YY_MOVE_OR_COPY< a_lang::IDNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
        value.YY_MOVE_OR_COPY< a_lang::IDToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        value.YY_MOVE_OR_COPY< a_lang::IntLitToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_loc: // loc
        value.YY_MOVE_OR_COPY< a_lang::LocNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
        value.YY_MOVE_OR_COPY< a_lang::ProgramNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        value.YY_MOVE_OR_COPY< a_lang::StmtNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        value.YY_MOVE_OR_COPY< a_lang::StrToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        value.YY_MOVE_OR_COPY< a_lang::Token * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        value.YY_MOVE_OR_COPY< a_lang::TypeNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_varDecl: // varDecl
        value.YY_MOVE_OR_COPY< a_lang::VarDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_globals: // globals
        value.YY_MOVE_OR_COPY< std::list<a_lang::DeclNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_actualsList: // actualsList
        value.YY_MOVE_OR_COPY< std::list<a_lang::ExpNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        value.YY_MOVE_OR_COPY< std::list<a_lang::FormalDeclNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmtList: // stmtList
        value.YY_MOVE_OR_COPY< std::list<a_lang::StmtNode *> * > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_callExp: // callExp
        value.move< a_lang::CallExpNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_decl: // decl
        value.move< a_lang::DeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        value.move< a_lang::ExpNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        value.move< a_lang::FnDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        value.move< a_lang::FormalDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_name: // name
        value.move< a_lang::IDNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
        value.move< a_lang::IDToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        value.move< a_lang::IntLitToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_loc: // loc
        value.move< a_lang::LocNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
        value.move< a_lang::ProgramNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        value.move< a_lang::StmtNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        value.move< a_lang::StrToken * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        value.move< a_lang::Token * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        value.move< a_lang::TypeNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_varDecl: // varDecl
        value.move< a_lang::VarDeclNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_globals: // globals
        value.move< std::list<a_lang::DeclNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_actualsList: // actualsList
        value.move< std::list<a_lang::ExpNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        value.move< std::list<a_lang::FormalDeclNode *> * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmtList: // stmtList
        value.move< std::list<a_lang::StmtNode *> * > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_callExp: // callExp
        value.copy< a_lang::CallExpNode * > (that.value);
        break;

      case symbol_kind::S_decl: // decl
        value.copy< a_lang::DeclNode * > (that.value);
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        value.copy< a_lang::ExpNode * > (that.value);
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        value.copy< a_lang::FnDeclNode * > (that.value);
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        value.copy< a_lang::FormalDeclNode * > (that.value);
        break;

      case symbol_kind::S_name: // name
        value.copy< a_lang::IDNode * > (that.value);
        break;

      case symbol_kind::S_ID: // ID
        value.copy< a_lang::IDToken * > (that.value);
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        value.copy< a_lang::IntLitToken * > (that.value);
        break;

      case symbol_kind::S_loc: // loc
        value.copy< a_lang::LocNode * > (that.value);
        break;

      case symbol_kind::S_program: // program
        value.copy< a_lang::ProgramNode * > (that.value);
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        value.copy< a_lang::StmtNode * > (that.value);
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        value.copy< a_lang::StrToken * > (that.value);
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        value.copy< a_lang::Token * > (that.value);
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        value.copy< a_lang::TypeNode * > (that.value);
        break;

      case symbol_kind::S_varDecl: // varDecl
        value.copy< a_lang::VarDeclNode * > (that.value);
        break;

      case symbol_kind::S_globals: // globals
        value.copy< std::list<a_lang::DeclNode *> * > (that.value);
        break;

      case symbol_kind::S_actualsList: // actualsList
        value.copy< std::list<a_lang::ExpNode *> * > (that.value);
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        value.copy< std::list<a_lang::FormalDeclNode *> * > (that.value);
        break;

      case symbol_kind::S_stmtList: // stmtList
        value.copy< std::list<a_lang::StmtNode *> * > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_callExp: // callExp
        value.move< a_lang::CallExpNode * > (that.value);
        break;

      case symbol_kind::S_decl: // decl
        value.move< a_lang::DeclNode * > (that.value);
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        value.move< a_lang::ExpNode * > (that.value);
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        value.move< a_lang::FnDeclNode * > (that.value);
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        value.move< a_lang::FormalDeclNode * > (that.value);
        break;

      case symbol_kind::S_name: // name
        value.move< a_lang::IDNode * > (that.value);
        break;

      case symbol_kind::S_ID: // ID
        value.move< a_lang::IDToken * > (that.value);
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        value.move< a_lang::IntLitToken * > (that.value);
        break;

      case symbol_kind::S_loc: // loc
        value.move< a_lang::LocNode * > (that.value);
        break;

      case symbol_kind::S_program: // program
        value.move< a_lang::ProgramNode * > (that.value);
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        value.move< a_lang::StmtNode * > (that.value);
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        value.move< a_lang::StrToken * > (that.value);
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        value.move< a_lang::Token * > (that.value);
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        value.move< a_lang::TypeNode * > (that.value);
        break;

      case symbol_kind::S_varDecl: // varDecl
        value.move< a_lang::VarDeclNode * > (that.value);
        break;

      case symbol_kind::S_globals: // globals
        value.move< std::list<a_lang::DeclNode *> * > (that.value);
        break;

      case symbol_kind::S_actualsList: // actualsList
        value.move< std::list<a_lang::ExpNode *> * > (that.value);
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        value.move< std::list<a_lang::FormalDeclNode *> * > (that.value);
        break;

      case symbol_kind::S_stmtList: // stmtList
        value.move< std::list<a_lang::StmtNode *> * > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_callExp: // callExp
        yylhs.value.emplace< a_lang::CallExpNode * > ();
        break;

      case symbol_kind::S_decl: // decl
        yylhs.value.emplace< a_lang::DeclNode * > ();
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_term: // term
        yylhs.value.emplace< a_lang::ExpNode * > ();
        break;

      case symbol_kind::S_fnDecl: // fnDecl
        yylhs.value.emplace< a_lang::FnDeclNode * > ();
        break;

      case symbol_kind::S_formalDecl: // formalDecl
        yylhs.value.emplace< a_lang::FormalDeclNode * > ();
        break;

      case symbol_kind::S_name: // name
        yylhs.value.emplace< a_lang::IDNode * > ();
        break;

      case symbol_kind::S_ID: // ID
        yylhs.value.emplace< a_lang::IDToken * > ();
        break;

      case symbol_kind::S_INTLITERAL: // INTLITERAL
        yylhs.value.emplace< a_lang::IntLitToken * > ();
        break;

      case symbol_kind::S_loc: // loc
        yylhs.value.emplace< a_lang::LocNode * > ();
        break;

      case symbol_kind::S_program: // program
        yylhs.value.emplace< a_lang::ProgramNode * > ();
        break;

      case symbol_kind::S_blockStmt: // blockStmt
      case symbol_kind::S_stmt: // stmt
        yylhs.value.emplace< a_lang::StmtNode * > ();
        break;

      case symbol_kind::S_STRINGLITERAL: // STRINGLITERAL
        yylhs.value.emplace< a_lang::StrToken * > ();
        break;

      case symbol_kind::S_AND: // AND
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_ARROW: // ARROW
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_COLON: // COLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_CUSTOM: // CUSTOM
      case symbol_kind::S_DASH: // DASH
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_EH: // EH
      case symbol_kind::S_EQUALS: // EQUALS
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_FROMCONSOLE: // FROMCONSOLE
      case symbol_kind::S_GREATER: // GREATER
      case symbol_kind::S_GREATEREQ: // GREATEREQ
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_IMMUTABLE: // IMMUTABLE
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_LESS: // LESS
      case symbol_kind::S_LESSEQ: // LESSEQ
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_MAYBE: // MAYBE
      case symbol_kind::S_MEANS: // MEANS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_NOTEQUALS: // NOTEQUALS
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_OTHERWISE: // OTHERWISE
      case symbol_kind::S_CROSS: // CROSS
      case symbol_kind::S_POSTDEC: // POSTDEC
      case symbol_kind::S_POSTINC: // POSTINC
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_REF: // REF
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_SEMICOL: // SEMICOL
      case symbol_kind::S_SLASH: // SLASH
      case symbol_kind::S_STAR: // STAR
      case symbol_kind::S_TOCONSOLE: // TOCONSOLE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_WHILE: // WHILE
        yylhs.value.emplace< a_lang::Token * > ();
        break;

      case symbol_kind::S_type: // type
      case symbol_kind::S_primType: // primType
        yylhs.value.emplace< a_lang::TypeNode * > ();
        break;

      case symbol_kind::S_varDecl: // varDecl
        yylhs.value.emplace< a_lang::VarDeclNode * > ();
        break;

      case symbol_kind::S_globals: // globals
        yylhs.value.emplace< std::list<a_lang::DeclNode *> * > ();
        break;

      case symbol_kind::S_actualsList: // actualsList
        yylhs.value.emplace< std::list<a_lang::ExpNode *> * > ();
        break;

      case symbol_kind::S_maybeFormals: // maybeFormals
      case symbol_kind::S_formalList: // formalList
        yylhs.value.emplace< std::list<a_lang::FormalDeclNode *> * > ();
        break;

      case symbol_kind::S_stmtList: // stmtList
        yylhs.value.emplace< std::list<a_lang::StmtNode *> * > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: globals
#line 140 "a.yy"
                  {
		  yylhs.value.as < a_lang::ProgramNode * > () = new ProgramNode(yystack_[0].value.as < std::list<a_lang::DeclNode *> * > ());
		  *root = yylhs.value.as < a_lang::ProgramNode * > ();
		  }
#line 1463 "parser.cc"
    break;

  case 3: // globals: globals decl
#line 146 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::DeclNode *> * > () = yystack_[1].value.as < std::list<a_lang::DeclNode *> * > ();
		  DeclNode * declNode = yystack_[0].value.as < a_lang::DeclNode * > ();
		  yylhs.value.as < std::list<a_lang::DeclNode *> * > ()->push_back(declNode);
		  }
#line 1473 "parser.cc"
    break;

  case 4: // globals: %empty
#line 152 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::DeclNode *> * > () = new std::list<DeclNode *>();
		  }
#line 1481 "parser.cc"
    break;

  case 5: // decl: varDecl SEMICOL
#line 157 "a.yy"
                  {
		  yylhs.value.as < a_lang::DeclNode * > () = yystack_[1].value.as < a_lang::VarDeclNode * > ();
		  }
#line 1489 "parser.cc"
    break;

  case 6: // decl: fnDecl
#line 161 "a.yy"
                  {
		  yylhs.value.as < a_lang::DeclNode * > () = yystack_[0].value.as < a_lang::FnDeclNode * > ();
		  }
#line 1497 "parser.cc"
    break;

  case 7: // varDecl: name COLON type
#line 166 "a.yy"
                  {
		  Position * p = new Position(yystack_[2].value.as < a_lang::IDNode * > ()->pos(), yystack_[0].value.as < a_lang::TypeNode * > ()->pos());
		  yylhs.value.as < a_lang::VarDeclNode * > () = new VarDeclNode(p,yystack_[2].value.as < a_lang::IDNode * > (), yystack_[0].value.as < a_lang::TypeNode * > (), nullptr);
		  }
#line 1506 "parser.cc"
    break;

  case 8: // varDecl: name COLON type ASSIGN exp
#line 171 "a.yy"
                  {
		  Position * p = new Position(yystack_[4].value.as < a_lang::IDNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::VarDeclNode * > () = new VarDeclNode(p,yystack_[4].value.as < a_lang::IDNode * > (), yystack_[2].value.as < a_lang::TypeNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1515 "parser.cc"
    break;

  case 9: // type: IMMUTABLE primType
#line 177 "a.yy"
                  {
		  Position * p = new Position(yystack_[1].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::TypeNode * > ()->pos());
		  yylhs.value.as < a_lang::TypeNode * > () = new ImmutableTypeNode(p, yystack_[0].value.as < a_lang::TypeNode * > ());
		  }
#line 1524 "parser.cc"
    break;

  case 10: // type: primType
#line 182 "a.yy"
                  {
		  yylhs.value.as < a_lang::TypeNode * > () = yystack_[0].value.as < a_lang::TypeNode * > ();
		  }
#line 1532 "parser.cc"
    break;

  case 11: // primType: INT
#line 187 "a.yy"
                  {
		  yylhs.value.as < a_lang::TypeNode * > () = new IntTypeNode(yystack_[0].value.as < a_lang::Token * > ()->pos());
		  }
#line 1540 "parser.cc"
    break;

  case 12: // primType: BOOL
#line 191 "a.yy"
                  {
		  yylhs.value.as < a_lang::TypeNode * > () = new BoolTypeNode(yystack_[0].value.as < a_lang::Token * > ()->pos());
		  }
#line 1548 "parser.cc"
    break;

  case 13: // primType: VOID
#line 195 "a.yy"
                  {
		  yylhs.value.as < a_lang::TypeNode * > () = new VoidTypeNode(yystack_[0].value.as < a_lang::Token * > ()->pos());
		  }
#line 1556 "parser.cc"
    break;

  case 14: // fnDecl: name COLON LPAREN maybeFormals RPAREN ARROW type LCURLY stmtList RCURLY
#line 200 "a.yy"
                  {
		  auto pos = new Position(yystack_[9].value.as < a_lang::IDNode * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::FnDeclNode * > () = new FnDeclNode(pos, yystack_[9].value.as < a_lang::IDNode * > (), yystack_[6].value.as < std::list<a_lang::FormalDeclNode *> * > (), yystack_[3].value.as < a_lang::TypeNode * > (), yystack_[1].value.as < std::list<a_lang::StmtNode *> * > ());
		  }
#line 1565 "parser.cc"
    break;

  case 15: // maybeFormals: %empty
#line 206 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::FormalDeclNode *> * > () = new std::list<FormalDeclNode *>();
		  }
#line 1573 "parser.cc"
    break;

  case 16: // maybeFormals: formalList
#line 210 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::FormalDeclNode *> * > () = yystack_[0].value.as < std::list<a_lang::FormalDeclNode *> * > ();
		  }
#line 1581 "parser.cc"
    break;

  case 17: // formalList: formalDecl
#line 215 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::FormalDeclNode *> * > () = new std::list<FormalDeclNode *>();
		  yylhs.value.as < std::list<a_lang::FormalDeclNode *> * > ()->push_back(yystack_[0].value.as < a_lang::FormalDeclNode * > ());
		  }
#line 1590 "parser.cc"
    break;

  case 18: // formalList: formalList COMMA formalDecl
#line 220 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::FormalDeclNode *> * > () = yystack_[2].value.as < std::list<a_lang::FormalDeclNode *> * > ();
		  yylhs.value.as < std::list<a_lang::FormalDeclNode *> * > ()->push_back(yystack_[0].value.as < a_lang::FormalDeclNode * > ());
		  }
#line 1599 "parser.cc"
    break;

  case 19: // formalDecl: name COLON type
#line 226 "a.yy"
                  {
		  auto pos = new Position(yystack_[2].value.as < a_lang::IDNode * > ()->pos(), yystack_[1].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::FormalDeclNode * > () = new FormalDeclNode(pos, yystack_[2].value.as < a_lang::IDNode * > (), yystack_[0].value.as < a_lang::TypeNode * > ());
		  }
#line 1608 "parser.cc"
    break;

  case 20: // stmtList: %empty
#line 232 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::StmtNode *> * > () = new std::list<StmtNode *>();
		  }
#line 1616 "parser.cc"
    break;

  case 21: // stmtList: stmtList stmt SEMICOL
#line 236 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::StmtNode *> * > () = yystack_[2].value.as < std::list<a_lang::StmtNode *> * > ();
		  yylhs.value.as < std::list<a_lang::StmtNode *> * > ()->push_back(yystack_[1].value.as < a_lang::StmtNode * > ());
		  }
#line 1625 "parser.cc"
    break;

  case 22: // stmtList: stmtList blockStmt
#line 241 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::StmtNode *> * > () = yystack_[1].value.as < std::list<a_lang::StmtNode *> * > ();
		  yylhs.value.as < std::list<a_lang::StmtNode *> * > ()->push_back(yystack_[0].value.as < a_lang::StmtNode * > ());
		  }
#line 1634 "parser.cc"
    break;

  case 23: // blockStmt: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
#line 247 "a.yy"
                  {
		  const Position * p = new Position(yystack_[6].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new WhileStmtNode(p, yystack_[4].value.as < a_lang::ExpNode * > (), yystack_[1].value.as < std::list<a_lang::StmtNode *> * > ());
		  }
#line 1643 "parser.cc"
    break;

  case 24: // blockStmt: IF LPAREN exp RPAREN LCURLY stmtList RCURLY
#line 252 "a.yy"
                  {
		  const Position * p = new Position(yystack_[6].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new IfStmtNode(p, yystack_[4].value.as < a_lang::ExpNode * > (), yystack_[1].value.as < std::list<a_lang::StmtNode *> * > ());
		  }
#line 1652 "parser.cc"
    break;

  case 25: // blockStmt: IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
#line 257 "a.yy"
                  {
		  const Position * p = new Position(yystack_[10].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new IfElseStmtNode(p, yystack_[8].value.as < a_lang::ExpNode * > (), yystack_[5].value.as < std::list<a_lang::StmtNode *> * > (), yystack_[1].value.as < std::list<a_lang::StmtNode *> * > ());
		  }
#line 1661 "parser.cc"
    break;

  case 26: // stmt: varDecl
#line 263 "a.yy"
                  {
		  yylhs.value.as < a_lang::StmtNode * > () = yystack_[0].value.as < a_lang::VarDeclNode * > ();
		  }
#line 1669 "parser.cc"
    break;

  case 27: // stmt: loc ASSIGN exp
#line 267 "a.yy"
                  {
		  const auto p = new Position(yystack_[2].value.as < a_lang::LocNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new AssignStmtNode(p, yystack_[2].value.as < a_lang::LocNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ()); 
		  }
#line 1678 "parser.cc"
    break;

  case 28: // stmt: callExp
#line 272 "a.yy"
                  {
		  yylhs.value.as < a_lang::StmtNode * > () = new CallStmtNode(yystack_[0].value.as < a_lang::CallExpNode * > ()->pos(), yystack_[0].value.as < a_lang::CallExpNode * > ());
		  }
#line 1686 "parser.cc"
    break;

  case 29: // stmt: loc POSTDEC
#line 276 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::LocNode * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new PostDecStmtNode(p, yystack_[1].value.as < a_lang::LocNode * > ());
		  }
#line 1695 "parser.cc"
    break;

  case 30: // stmt: loc POSTINC
#line 281 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::LocNode * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new PostIncStmtNode(p, yystack_[1].value.as < a_lang::LocNode * > ());
		  }
#line 1704 "parser.cc"
    break;

  case 31: // stmt: TOCONSOLE exp
#line 286 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new ToConsoleStmtNode(p, yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1713 "parser.cc"
    break;

  case 32: // stmt: FROMCONSOLE loc
#line 291 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::LocNode * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new FromConsoleStmtNode(p, yystack_[0].value.as < a_lang::LocNode * > ());
		  }
#line 1722 "parser.cc"
    break;

  case 33: // stmt: MAYBE loc MEANS exp OTHERWISE exp
#line 296 "a.yy"
                  {
		  const Position * p = new Position(yystack_[5].value.as < a_lang::Token * > ()->pos(), yystack_[4].value.as < a_lang::LocNode * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new MaybeStmtNode(p, yystack_[4].value.as < a_lang::LocNode * > (), yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1731 "parser.cc"
    break;

  case 34: // stmt: RETURN exp
#line 301 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::StmtNode * > () = new ReturnStmtNode(p, yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1740 "parser.cc"
    break;

  case 35: // stmt: RETURN
#line 306 "a.yy"
                  {
		  const Position * p = yystack_[0].value.as < a_lang::Token * > ()->pos();
		  yylhs.value.as < a_lang::StmtNode * > () = new ReturnStmtNode(p, nullptr);
		  }
#line 1749 "parser.cc"
    break;

  case 36: // exp: exp DASH exp
#line 312 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new MinusNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1758 "parser.cc"
    break;

  case 37: // exp: exp CROSS exp
#line 317 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new PlusNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1767 "parser.cc"
    break;

  case 38: // exp: exp STAR exp
#line 322 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new TimesNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1776 "parser.cc"
    break;

  case 39: // exp: exp SLASH exp
#line 327 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new DivideNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1785 "parser.cc"
    break;

  case 40: // exp: exp AND exp
#line 332 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new AndNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1794 "parser.cc"
    break;

  case 41: // exp: exp OR exp
#line 337 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new OrNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1803 "parser.cc"
    break;

  case 42: // exp: exp EQUALS exp
#line 342 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new EqualsNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1812 "parser.cc"
    break;

  case 43: // exp: exp NOTEQUALS exp
#line 347 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new NotEqualsNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1821 "parser.cc"
    break;

  case 44: // exp: exp GREATER exp
#line 352 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new GreaterNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1830 "parser.cc"
    break;

  case 45: // exp: exp GREATEREQ exp
#line 357 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new GreaterEqNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1839 "parser.cc"
    break;

  case 46: // exp: exp LESS exp
#line 362 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new LessNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1848 "parser.cc"
    break;

  case 47: // exp: exp LESSEQ exp
#line 367 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::ExpNode * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new LessEqNode(p, yystack_[2].value.as < a_lang::ExpNode * > (), yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1857 "parser.cc"
    break;

  case 48: // exp: NOT exp
#line 372 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new NotNode(p, yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1866 "parser.cc"
    break;

  case 49: // exp: DASH term
#line 377 "a.yy"
                  {
		  const Position * p = new Position(yystack_[1].value.as < a_lang::Token * > ()->pos(), yystack_[0].value.as < a_lang::ExpNode * > ()->pos());
		  yylhs.value.as < a_lang::ExpNode * > () = new NegNode(p, yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1875 "parser.cc"
    break;

  case 50: // exp: term
#line 382 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = yystack_[0].value.as < a_lang::ExpNode * > (); }
#line 1881 "parser.cc"
    break;

  case 51: // callExp: loc LPAREN RPAREN
#line 386 "a.yy"
                  {
		  const Position * p = new Position(yystack_[2].value.as < a_lang::LocNode * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  std::list<ExpNode *> * noargs =
		    new std::list<ExpNode *>();
		  yylhs.value.as < a_lang::CallExpNode * > () = new CallExpNode(p, yystack_[2].value.as < a_lang::LocNode * > (), noargs);
		  }
#line 1892 "parser.cc"
    break;

  case 52: // callExp: loc LPAREN actualsList RPAREN
#line 393 "a.yy"
                  {
		  const Position * p = new Position(yystack_[3].value.as < a_lang::LocNode * > ()->pos(), yystack_[0].value.as < a_lang::Token * > ()->pos());
		  yylhs.value.as < a_lang::CallExpNode * > () = new CallExpNode(p, yystack_[3].value.as < a_lang::LocNode * > (), yystack_[1].value.as < std::list<a_lang::ExpNode *> * > ());
		  }
#line 1901 "parser.cc"
    break;

  case 53: // actualsList: exp
#line 399 "a.yy"
                  {
		  std::list<ExpNode *> * list =
		    new std::list<ExpNode *>();
		  list->push_back(yystack_[0].value.as < a_lang::ExpNode * > ());
		  yylhs.value.as < std::list<a_lang::ExpNode *> * > () = list;
		  }
#line 1912 "parser.cc"
    break;

  case 54: // actualsList: actualsList COMMA exp
#line 406 "a.yy"
                  {
		  yylhs.value.as < std::list<a_lang::ExpNode *> * > () = yystack_[2].value.as < std::list<a_lang::ExpNode *> * > ();
		  yylhs.value.as < std::list<a_lang::ExpNode *> * > ()->push_back(yystack_[0].value.as < a_lang::ExpNode * > ());
		  }
#line 1921 "parser.cc"
    break;

  case 55: // term: loc
#line 412 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = yystack_[0].value.as < a_lang::LocNode * > (); }
#line 1927 "parser.cc"
    break;

  case 56: // term: INTLITERAL
#line 414 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = new IntLitNode(yystack_[0].value.as < a_lang::IntLitToken * > ()->pos(), yystack_[0].value.as < a_lang::IntLitToken * > ()->num()); }
#line 1933 "parser.cc"
    break;

  case 57: // term: STRINGLITERAL
#line 416 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = new StrLitNode(yystack_[0].value.as < a_lang::StrToken * > ()->pos(), yystack_[0].value.as < a_lang::StrToken * > ()->str()); }
#line 1939 "parser.cc"
    break;

  case 58: // term: TRUE
#line 418 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = new TrueNode(yystack_[0].value.as < a_lang::Token * > ()->pos()); }
#line 1945 "parser.cc"
    break;

  case 59: // term: FALSE
#line 420 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = new FalseNode(yystack_[0].value.as < a_lang::Token * > ()->pos()); }
#line 1951 "parser.cc"
    break;

  case 60: // term: EH
#line 422 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = new EhNode(yystack_[0].value.as < a_lang::Token * > ()->pos()); }
#line 1957 "parser.cc"
    break;

  case 61: // term: LPAREN exp RPAREN
#line 424 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = yystack_[1].value.as < a_lang::ExpNode * > (); }
#line 1963 "parser.cc"
    break;

  case 62: // term: callExp
#line 426 "a.yy"
                  { yylhs.value.as < a_lang::ExpNode * > () = yystack_[0].value.as < a_lang::CallExpNode * > (); }
#line 1969 "parser.cc"
    break;

  case 63: // loc: name
#line 429 "a.yy"
                  {
		  yylhs.value.as < a_lang::LocNode * > () = yystack_[0].value.as < a_lang::IDNode * > ();
		  }
#line 1977 "parser.cc"
    break;

  case 64: // name: ID
#line 434 "a.yy"
                  {
		  yylhs.value.as < a_lang::IDNode * > () = new IDNode(yystack_[0].value.as < a_lang::IDToken * > ()->pos(), yystack_[0].value.as < a_lang::IDToken * > ()->value());
		  }
#line 1985 "parser.cc"
    break;


#line 1989 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -78;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -78,     5,    -9,   -78,   -78,   -78,   -24,   -78,    12,   -78,
      38,   -78,   -78,     1,    -9,   -78,    13,   -78,   -78,     4,
      45,   -78,    48,   222,    52,    -9,     2,   256,   -78,   -78,
     -78,   222,   222,   -78,   -78,   186,   -78,   -78,    33,   -78,
       2,   -78,   -78,   -78,    66,   -78,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   221,    39,
     -78,   262,   -28,   281,   281,   281,   281,   281,   281,   188,
     -28,   -78,   -78,   -78,   186,    10,   -78,   222,   -78,   294,
     186,    -9,    40,    -9,   222,   -78,   222,    41,   -78,   -78,
      28,   -78,    16,    63,   -78,   222,    43,   186,   186,   222,
     -78,   222,   -78,   -78,     2,   101,   222,   120,   186,    49,
     153,    50,   -78,   222,   -78,   300,   186,   324,    64,   -78,
      58,   -78,   330,   -78
  };

  const signed char
  Parser::yydefact_[] =
  {
       4,     0,     2,     1,    64,     3,     0,     6,     0,     5,
       0,    12,    11,     0,    15,    13,     7,    10,     9,     0,
      16,    17,     0,     0,     0,     0,     0,     0,    60,    59,
      56,     0,     0,    57,    58,     8,    62,    50,    55,    63,
       0,    18,    19,    49,     0,    48,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    40,    36,    42,    44,    45,    46,    47,    43,    41,
      37,    39,    38,    51,    53,     0,    20,     0,    52,     0,
      54,     0,     0,     0,    35,    14,     0,     0,    26,    22,
       0,    28,     0,    63,    32,     0,     0,    34,    31,     0,
      21,     0,    29,    30,     0,     0,     0,     0,    27,     0,
       0,     0,    20,     0,    20,     0,    33,     0,    24,    23,
       0,    20,     0,    25
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -78,   -78,   -78,   -78,    84,   -25,    74,   -78,   -78,   -78,
      68,   -60,   -78,   -78,   -21,   -76,   -78,    61,   -77,    -2
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     1,     2,     5,    88,    16,    17,     7,    19,    20,
      21,    79,    89,    90,    35,    36,    75,    37,    38,    39
  };

  const signed char
  Parser::yytable_[] =
  {
       8,    42,    92,    91,    94,     3,    96,    11,    11,     4,
      44,    45,    22,    56,    57,    59,     9,    23,    77,    10,
     101,    12,    12,    22,    13,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    74,    92,    91,
      92,    91,    58,    24,    11,    92,    91,    15,    15,    78,
     102,   103,   115,    25,   117,    26,    80,    40,    12,    58,
      13,   122,    76,    97,    14,    98,    95,    99,   100,    46,
     104,   106,   112,   114,   105,   120,    47,    93,   107,    48,
     108,   121,    49,    50,    15,   110,     6,    18,    43,     0,
      51,    52,   116,    41,     0,     0,    53,    54,     0,    55,
       0,     0,     0,     0,    46,    60,     0,    56,    57,     0,
       0,    47,     0,    93,    48,    93,     0,    49,    50,     0,
      93,     0,     0,    46,     0,    51,    52,     0,     0,     0,
      47,    53,    54,    48,    55,     0,    49,    50,     0,     0,
     109,     0,    56,    57,    51,    52,     0,     0,     0,     0,
      53,    54,     0,    55,     0,     0,    46,     0,     0,   111,
       0,    56,    57,    47,     0,     0,    48,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,    54,   113,    55,     0,     0,    46,
       0,    46,     0,     0,    56,    57,    47,     0,    47,    48,
       0,    48,    49,    50,    49,    50,     0,     0,     0,     0,
      51,    52,    51,    52,     0,     0,    53,    54,    53,    55,
       0,    55,     0,     0,     0,     0,     0,    56,    57,    56,
      57,    27,    27,    28,    28,    29,    29,     0,     0,     4,
       4,     0,    30,    30,     0,     0,     0,    31,    31,     0,
      32,    32,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,     0,     0,    33,    33,    34,    34,    28,     0,
      29,     0,    47,     0,     4,    48,     0,    30,    49,    50,
       0,     0,    31,     0,     0,     0,    51,    52,     0,     0,
       0,    47,    53,     0,    -1,    55,     0,    -1,    -1,    33,
       0,    34,     0,    56,    57,    -1,    -1,     0,     0,    81,
       0,    -1,     4,    82,    55,    81,     0,     0,     4,    82,
       0,    83,    56,    57,     0,     0,     0,    83,     0,     0,
      84,    85,     0,     0,     0,     0,    84,   118,    86,    81,
       0,    87,     4,    82,    86,    81,     0,    87,     4,    82,
       0,    83,     0,     0,     0,     0,     0,    83,     0,     0,
      84,   119,     0,     0,     0,     0,    84,   123,    86,     0,
       0,    87,     0,     0,    86,     0,     0,    87
  };

  const signed char
  Parser::yycheck_[] =
  {
       2,    26,    79,    79,    81,     0,    83,     6,     6,    18,
      31,    32,    14,    41,    42,    40,    40,     4,     8,     7,
       4,    20,    20,    25,    22,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,   115,   115,
     117,   117,    26,    39,     6,   122,   122,    46,    46,    39,
      34,    35,   112,     8,   114,     7,    77,     5,    20,    26,
      22,   121,    23,    84,    26,    86,    26,    26,    40,     3,
       7,    28,    23,    23,    95,    11,    10,    79,    99,    13,
     101,    23,    16,    17,    46,   106,     2,    13,    27,    -1,
      24,    25,   113,    25,    -1,    -1,    30,    31,    -1,    33,
      -1,    -1,    -1,    -1,     3,    39,    -1,    41,    42,    -1,
      -1,    10,    -1,   115,    13,   117,    -1,    16,    17,    -1,
     122,    -1,    -1,     3,    -1,    24,    25,    -1,    -1,    -1,
      10,    30,    31,    13,    33,    -1,    16,    17,    -1,    -1,
      39,    -1,    41,    42,    24,    25,    -1,    -1,    -1,    -1,
      30,    31,    -1,    33,    -1,    -1,     3,    -1,    -1,    39,
      -1,    41,    42,    10,    -1,    -1,    13,    -1,    -1,    16,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    -1,    -1,     3,
      -1,     3,    -1,    -1,    41,    42,    10,    -1,    10,    13,
      -1,    13,    16,    17,    16,    17,    -1,    -1,    -1,    -1,
      24,    25,    24,    25,    -1,    -1,    30,    31,    30,    33,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    41,    42,    41,
      42,    10,    10,    12,    12,    14,    14,    -1,    -1,    18,
      18,    -1,    21,    21,    -1,    -1,    -1,    26,    26,    -1,
      29,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    43,    43,    45,    45,    12,    -1,
      14,    -1,    10,    -1,    18,    13,    -1,    21,    16,    17,
      -1,    -1,    26,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    10,    30,    -1,    13,    33,    -1,    16,    17,    43,
      -1,    45,    -1,    41,    42,    24,    25,    -1,    -1,    15,
      -1,    30,    18,    19,    33,    15,    -1,    -1,    18,    19,
      -1,    27,    41,    42,    -1,    -1,    -1,    27,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    36,    37,    44,    15,
      -1,    47,    18,    19,    44,    15,    -1,    47,    18,    19,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    36,    37,    44,    -1,
      -1,    47,    -1,    -1,    44,    -1,    -1,    47
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    49,    50,     0,    18,    51,    52,    55,    67,    40,
       7,     6,    20,    22,    26,    46,    53,    54,    54,    56,
      57,    58,    67,     4,    39,     8,     7,    10,    12,    14,
      21,    26,    29,    43,    45,    62,    63,    65,    66,    67,
       5,    58,    53,    65,    62,    62,     3,    10,    13,    16,
      17,    24,    25,    30,    31,    33,    41,    42,    26,    53,
      39,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    39,    62,    64,    23,     8,    39,    59,
      62,    15,    19,    27,    36,    37,    44,    47,    52,    60,
      61,    63,    66,    67,    66,    26,    66,    62,    62,    26,
      40,     4,    34,    35,     7,    62,    28,    62,    62,    39,
      62,    39,    23,    32,    23,    59,    62,    59,    37,    37,
      11,    23,    59,    37
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    48,    49,    50,    50,    51,    51,    52,    52,    53,
      53,    54,    54,    54,    55,    56,    56,    57,    57,    58,
      59,    59,    59,    60,    60,    60,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    65,    65,
      65,    65,    65,    66,    67
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     2,     1,     3,     5,     2,
       1,     1,     1,     1,    10,     0,     1,     1,     3,     3,
       0,     3,     2,     7,     7,    11,     1,     3,     1,     2,
       2,     2,     2,     6,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     3,     4,     1,     3,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end file\"", "error", "\"invalid token\"", "AND", "ASSIGN", "ARROW",
  "BOOL", "COLON", "COMMA", "CUSTOM", "DASH", "ELSE", "EH", "EQUALS",
  "FALSE", "FROMCONSOLE", "GREATER", "GREATEREQ", "ID", "IF", "INT",
  "INTLITERAL", "IMMUTABLE", "LCURLY", "LESS", "LESSEQ", "LPAREN", "MAYBE",
  "MEANS", "NOT", "NOTEQUALS", "OR", "OTHERWISE", "CROSS", "POSTDEC",
  "POSTINC", "RETURN", "RCURLY", "REF", "RPAREN", "SEMICOL", "SLASH",
  "STAR", "STRINGLITERAL", "TOCONSOLE", "TRUE", "VOID", "WHILE", "$accept",
  "program", "globals", "decl", "varDecl", "type", "primType", "fnDecl",
  "maybeFormals", "formalList", "formalDecl", "stmtList", "blockStmt",
  "stmt", "exp", "callExp", "actualsList", "term", "loc", "name", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   139,   139,   145,   152,   156,   160,   165,   170,   176,
     181,   186,   190,   194,   199,   206,   209,   214,   219,   225,
     232,   235,   240,   246,   251,   256,   262,   266,   271,   275,
     280,   285,   290,   295,   300,   305,   311,   316,   321,   326,
     331,   336,   341,   346,   351,   356,   361,   366,   371,   376,
     381,   385,   392,   398,   405,   411,   413,   415,   417,   419,
     421,   423,   425,   428,   433
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
    };
    // Last valid token kind.
    const int code_max = 302;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "a.yy"
} // a_lang
#line 2634 "parser.cc"

#line 438 "a.yy"


void a_lang::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
