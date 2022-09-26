/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_ARQUIVO_BISON_TAB_H_INCLUDED
# define YY_YY_ARQUIVO_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_INTEIRO = 258,
    T_FLUTUANTE = 259,
    T_ID = 260,
    T_SOMA = 261,
    T_SUBTRACAO = 262,
    T_BARRA = 263,
    T_NEGACAO = 264,
    T_IGUALDADE = 265,
    T_MAIOR = 266,
    T_MENOR = 267,
    T_NOT = 268,
    T_AND = 269,
    T_OR = 270,
    T_RESTO = 271,
    T_IF = 272,
    T_ELSE = 273,
    T_WHILE = 274,
    T_FOR = 275,
    T_PARENTESE_ESQUERDO = 276,
    T_PARENTESE_DIREITO = 277,
    T_CHAVE_ESQUERDA = 278,
    T_CHAVE_DIREITA = 279,
    T_TIPO_INT = 280,
    T_TIPO_FLOAT = 281,
    T_TIPO_BOOL = 282,
    T_FIM = 283,
    T_PONTO_VIRGULA = 284,
    T_VIRGULA = 285,
    T_NOVA_LINHA = 286,
    T_PRINT = 287,
    T_SCAN = 288,
    T_EXPONENCIAL = 289,
    T_HIP_COS = 290,
    T_HIP_SIN = 291,
    T_MULTIPLICACAO = 292,
    T_DIVISAO = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 24 "arquivo_bison.y" /* yacc.c:1909  */

	int ival;
	float fval;
	char* idval;
	Nodo_Ast* astval;

#line 100 "arquivo_bison.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ARQUIVO_BISON_TAB_H_INCLUDED  */
