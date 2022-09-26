%{

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "lista.h"
#define PRINTAR 1

extern int yylex();
extern int yyparse();
extern FILE* yyin;

Lista *tbl_de_simbolos;
Nodo_Token *aux_sin;

void ajuda(int d);

void yyerror(const char* s);
%}

%union {
	int ival;
	float fval;
	char idval[10];
}

%token<ival> T_INTEIRO
%token<fval> T_FLUTUANTE
%token<idval> T_ID
%token T_SOMA T_SUBTRACAO T_BARRA T_NEGACAO T_IGUALDADE
%token T_IF T_ELSE T_WHILE
%token T_PARENTESE_ESQUERDO T_PARENTESE_DIREITO T_CHAVE_ESQUERDA T_CHAVE_DIREITA
%token T_TIPO_INT T_TIPO_FLOAT T_TIPO_BOOL
%token T_FIM T_PONTO_VIRGULA T_VIRGULA
%token T_NOVA_LINHA
%token T_PRINT_INT T_SCAN_INT T_EXPONENCIAL

%left T_SOMA T_SUBTRACAO
%left T_MULTIPLICACAO T_DIVISAO

/*%type<ival> operacao_inteira
%type<fval> operacao_real*/

%type<idval> declaracao_inteiro
%type<idval> declaracao_flutuante
%type<idval> declaracao_booleano

%type<idval> declaracao_var

%type<fval> operacao_float1 operacao_float2 valores variavel_declarada


%start fita

%%

fita: conteudo
	;
	
conteudo:
	| conteudo linha  {printf("raiz\n");}
	;
	
/*uma linha em c pode ser definida por:
	uma expressao convencional marcada por ponto e virgula:
	uma estrutura de controle, marcada por chaves
	uma inclusão de arquivo, a ser implementado posteriormente no contexto desse trabalho
	* */
linha: expressao T_PONTO_VIRGULA T_NOVA_LINHA//expressao
	| T_FIM T_NOVA_LINHA {
		printf("\n(comp)fim da execução\n");
		if(tbl_de_simbolos->tamanho>0)lista_imprime_tabela_de_simbolos(tbl_de_simbolos,"tabela_de_simbolos.txt");
		if(lista_destroi(tbl_de_simbolos)==1)printf("\n(comp)pilha limpa\n");
		exit(0);
		}
//	| controle
	;

/*controle: condicional
	| repeticao
	;
*/
	
expressao: declaracao //declaracao de variavel
	| operacao//operacao entre variaveis
	|funcao
	;
funcao: T_PRINT_INT T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {printf("%d\n",(int)$3);ajuda(__LINE__);}
	;

/*
 * INICIO validação de declaracoes	
 */
declaracao:
	| T_TIPO_INT declaracao_inteiro {ajuda(__LINE__);}
	| T_TIPO_FLOAT	declaracao_flutuante {ajuda(__LINE__);}
	| T_TIPO_BOOL declaracao_booleano {ajuda(__LINE__);}//tipo da variavel a ser declarada
	;
declaracao_var: T_ID
	;
//declaracao int
declaracao_inteiro: T_ID {
		ajuda(__LINE__);
		lista_adiciona_token(tbl_de_simbolos,token_nodo_inteiro($1,0));
		}
	| declaracao_var T_IGUALDADE T_INTEIRO {
		ajuda(__LINE__);
		lista_adiciona_token(tbl_de_simbolos,token_nodo_inteiro($1,$3));
		}
	| declaracao_inteiro T_VIRGULA declaracao_inteiro {ajuda(__LINE__);}
	;

//declaracao float
declaracao_flutuante: T_ID	{ajuda(__LINE__);}
	| declaracao_var T_IGUALDADE T_FLUTUANTE {
		ajuda(__LINE__);
		lista_adiciona_token(tbl_de_simbolos,token_nodo_flutuante($1,$3));
		}
	| declaracao_flutuante T_VIRGULA declaracao_flutuante {ajuda(__LINE__);}
	;

//declaracao booleano
declaracao_booleano: T_ID {ajuda(__LINE__);}
	|T_ID T_IGUALDADE T_INTEIRO {
		ajuda(__LINE__);
		lista_adiciona_token(tbl_de_simbolos,token_nodo_booleano($1,$3));
		}
	| declaracao_booleano T_VIRGULA declaracao_booleano {ajuda(__LINE__);}
	;
/*
 * Validação de operações	
 */
operacao: T_ID T_IGUALDADE operacao_float1 {
	aux_sin = lista_busca(tbl_de_simbolos, $1);
	if(aux_sin!=NULL){
		if(strcmp(aux_sin->tipo,"float")==0){
			aux_sin->valor_real = $3;
			}
		else{
			aux_sin->valor_inteiro = (int)$3;
			}
		}
	else{
		printf("\n(comp)Atencao! Variavel %s ainda nao declarada! ",$1);
		}
	ajuda(__LINE__);
	}
	;
//Todas as operações são realizadas como float e é usado typecast quando necessário
	;

operacao_float1: operacao_float2
	| operacao_float1 T_SOMA operacao_float1 {$$ = $1+$3;ajuda(__LINE__);}
	| operacao_float1 T_SUBTRACAO operacao_float1 {$$ =$1-$3;ajuda(__LINE__);}
	;
operacao_float2: valores
	| operacao_float1 T_MULTIPLICACAO operacao_float1 {$$=$1*$3;ajuda(__LINE__);}
	| operacao_float1 T_DIVISAO operacao_float1 {$$=$1/$3;ajuda(__LINE__);}
	;
valores: T_FLUTUANTE
	| T_INTEIRO {$$ = (float)$1;}
	| variavel_declarada
	| T_EXPONENCIAL T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {$$ = (float)pow(2.718281,$3);ajuda(__LINE__);}
	| T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {$$=$2;}
	;

/*variavel declarada é extremamente importante, retorna 
 * qualquer variavel declarada no formato float
 */
variavel_declarada: T_ID {
	aux_sin = lista_busca(tbl_de_simbolos,$1);
	if(aux_sin!=NULL){		
		if(strcmp("float",aux_sin->tipo)==0){
			$$ = aux_sin->valor_real;
			}
		else{
			$$ = (float)aux_sin->valor_inteiro;
			}
		}
	}
	;

%%

int main() {
	yyin = stdin;
	tbl_de_simbolos = lista_cria();
	printf("\nTrabalho de compiladores\ndefinir constante PRINTAR em arquivo_bison.h como 1 para ver arvore lexica, 0 para ocultar\nDigite FIM para terminar execucao:\n");
	do {
		yyparse();
	} while(!feof(yyin));
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Erro de análise (sintática): %s\n", s);
	exit(1);
}

void ajuda(int d){
	if(PRINTAR){printf("l[%d]-",d);}
	}
