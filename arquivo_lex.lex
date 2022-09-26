%option noyywrap

%{
	#include <stdio.h>
	#include <math.h>
	
	#define YY_DECL int yylex()
	
	#include "lista.h"
	#include "arquivo_bison.tab.h"
%}

DIGITO [-+]?[0-9]+
ID [a-z][a-z0-9]*

%%

[ \t] ;

{DIGITO}+ {yylval.ival = atoi(yytext);return T_INTEIRO;}
{DIGITO}+\.{DIGITO}* {yylval.fval = atof(yytext);return T_FLUTUANTE;}

"+" {return T_SOMA;}
"-" {return T_SUBTRACAO;}
"/" {return T_BARRA;}
"~" {return T_NEGACAO;}
"=" {return T_IGUALDADE;}
"<" {return T_MENOR;}
">" {return T_MAIOR;}
"!" {return T_NOT;}
"||" {return T_OR;}
"&&" {return T_AND;}
"%" {return T_RESTO;}

"if" {return T_IF;}
"else" {return T_ELSE;}
"while" {return T_WHILE;}
"for" {return T_FOR;}

"(" {return T_PARENTESE_ESQUERDO;}
")" {return T_PARENTESE_DIREITO;}
"{" {return T_CHAVE_ESQUERDA;}
"}" {return T_CHAVE_DIREITA;}

"print_" {return T_PRINT;}
"scan_" {return T_SCAN;}
"exp" {return T_EXPONENCIAL;}
"cosh" {return T_HIP_COS;}
"sinh" {return T_HIP_SIN;}

"int" {return T_TIPO_INT;}
"float" {return T_TIPO_FLOAT;}
"bool" {return T_TIPO_BOOL;}


";" {return T_PONTO_VIRGULA;}
"," {return T_VIRGULA;}


"FIM" {return T_FIM;}

{ID} {/*strcpy(yylval.idval, yytext)*/yylval.idval=strdup(yytext);return T_ID;}


\n {return T_NOVA_LINHA;}


%%
