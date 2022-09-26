%{

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "lista.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;


Lista *tbl_de_simbolos;
Nodo_Token *aux_sin;
Nodo_Ast *raiz_ast;

void ajuda(int d);

void yyerror(const char* s);
%}

%union {
	int ival;
	float fval;
	char* idval;
	Nodo_Ast* astval;
}

%token<ival> T_INTEIRO
%token<fval> T_FLUTUANTE
%token<idval> T_ID
%token T_SOMA T_SUBTRACAO T_BARRA T_NEGACAO T_IGUALDADE T_MAIOR T_MENOR T_NOT T_AND T_OR T_RESTO
%token T_IF T_ELSE T_WHILE T_FOR
%token T_PARENTESE_ESQUERDO T_PARENTESE_DIREITO T_CHAVE_ESQUERDA T_CHAVE_DIREITA
%token T_TIPO_INT T_TIPO_FLOAT T_TIPO_BOOL
%token T_FIM T_PONTO_VIRGULA T_VIRGULA
%token T_NOVA_LINHA
%token T_PRINT T_SCAN T_EXPONENCIAL T_HIP_COS T_HIP_SIN

%left T_SOMA T_SUBTRACAO
%left T_MULTIPLICACAO T_DIVISAO T_RESTO
%left T_NOT T_AND T_OR T_MAIOR T_MENOR T_IGUALDADE
%left T_VIRGULA

/*%type<ival> operacao_inteira
%type<fval> operacao_real*/

%type<idval> declaracao_var

%type<astval> fita inicializador conteudo linha expressao controle declaracao atribuicao chamada_funcao
%type<astval> arg declaracao_inteiro declaracao_flutuante declaracao_booleano
%type<astval> operacao_float1 operacao_float2 valores variavel_declarada controle_if demonstracao


%start fita

%%

fita: inicializador {ajuda(__LINE__);}
	;
	
inicializador: conteudo {printf("raiz");ajuda(__LINE__);}
	;

conteudo: {ajuda(__LINE__);$$=0;}
	| conteudo linha  {ajuda(__LINE__);$$=ast_conteudo($1,$2);raiz_ast=$1;}//conteudo define o conjunto de expressoes nao validadas, vai ser muito usado daqui para frente
	;
	
/*uma linha em c pode ser definida por:
	uma expressao convencional marcada por ponto e virgula:
	uma estrutura de controle, marcada por chaves
	uma inclusão de arquivo, a ser implementado posteriormente no contexto desse trabalho
	* */
linha: expressao T_PONTO_VIRGULA T_NOVA_LINHA {$$=$1;}//expressao
	| T_FIM T_NOVA_LINHA {
		ajuda(__LINE__);
		printf("\n(comp:Fim da montagem da ast e tbl de simbolos)\n");
		//if(tbl_de_simbolos->tamanho>0)lista_imprime_tabela_de_simbolos(tbl_de_simbolos,"tabela_de_simbolos.txt");
		printf("(comp: validacao da arvore sintatica)\n");
		ast_print(raiz_ast);
		printf("\n(comp: sucesso validacao)\n");
		gen_code(raiz_ast,tbl_de_simbolos,"java_bytecode.j");
		printf("\n(comp)fim da execução\n");
		fclose(yyin);
		exit(0);
		}
	| controle
	;
	
expressao: declaracao //declaracao de variavel DONE
	| atribuicao//operacao entre variaveis
	| chamada_funcao//chamada da tabela de simbolos
	;
chamada_funcao: T_PRINT T_TIPO_INT T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {ajuda(__LINE__);$$=ast_chamada("print_int",$4);}
	| T_ID T_PARENTESE_ESQUERDO arg T_PARENTESE_DIREITO {ajuda(__LINE__);$$=ast_chamada($1,$3);}
	;
arg: {ajuda(__LINE__);$$=NULL;}//funcoes sem parametro
	|T_INTEIRO {ajuda(__LINE__);$$=ast_inteiro($1);}
	| T_FLUTUANTE {ajuda(__LINE__);$$=ast_inteiro($1);}
	|T_ID {ajuda(__LINE__);$$=ast_chamada($1,NULL);}
	;//Nao vou trabalhar ainda com multiplos parametros, mas ainda acredito que seria de facil implementacao

/*
 * INICIO validação de declaracoes	
 */
declaracao: T_TIPO_INT declaracao_inteiro {ajuda(__LINE__);$$=$2;}
	| T_TIPO_FLOAT declaracao_flutuante {ajuda(__LINE__);$$=$2;}
	| T_TIPO_BOOL declaracao_booleano {ajuda(__LINE__);$$=$2;}//tipo da variavel a ser declarada
	;
declaracao_var: T_ID
	;
//declaracao int
declaracao_inteiro: T_ID {
		ajuda(__LINE__);
		$$=ast_declaracao($1,"int",NULL);
		lista_adiciona_token(tbl_de_simbolos,token_nodo_inteiro($1,0));
		}
	| declaracao_var T_IGUALDADE T_INTEIRO {
		ajuda(__LINE__);
		$$=ast_declaracao($1,"int",ast_inteiro($3));
		lista_adiciona_token(tbl_de_simbolos,token_nodo_inteiro($1,$3));
		}
	| declaracao_inteiro T_VIRGULA declaracao_inteiro {ajuda(__LINE__);$$=ast_ramificacao($1,$3);}
	;

//declaracao float
declaracao_flutuante: T_ID	{
	ajuda(__LINE__);
	lista_adiciona_token(tbl_de_simbolos,token_nodo_flutuante($1,0.0));
	$$=ast_declaracao($1,"float",NULL);
	}
	| declaracao_var T_IGUALDADE T_FLUTUANTE {
		ajuda(__LINE__);
		$$=ast_declaracao($1,"float",ast_flutuante($3));
		lista_adiciona_token(tbl_de_simbolos,token_nodo_flutuante($1,$3));
		}
	| declaracao_flutuante T_VIRGULA declaracao_flutuante {ajuda(__LINE__);$$=ast_ramificacao($1,$3);}
	;

//declaracao booleano
declaracao_booleano: T_ID {
	ajuda(__LINE__);
	lista_adiciona_token(tbl_de_simbolos,token_nodo_booleano($1,0));
	$$=ast_declaracao($1,"int",NULL);
	}
	|T_ID T_IGUALDADE T_INTEIRO {
		ajuda(__LINE__);
		$$=ast_declaracao($1,"int",ast_inteiro($3));
		lista_adiciona_token(tbl_de_simbolos,token_nodo_booleano($1,$3));
		}
	| declaracao_booleano T_VIRGULA declaracao_booleano {ajuda(__LINE__);$$=ast_ramificacao($1,$3);}
	;
/*
 * Validação de operações	
 */
atribuicao: T_ID T_IGUALDADE operacao_float1 {
	ajuda(__LINE__);
	$$=ast_atribuicao(ast_chamada($1,NULL),$3);
	}
	;
//Todas as operações são realizadas como float e é usado typecast quando necessário
	;

operacao_float1: operacao_float2
	| operacao_float1 T_SOMA operacao_float1 {ajuda(__LINE__);$$ = ast_operacao('+',$1,$3);}
	| operacao_float1 T_SUBTRACAO operacao_float1 {ajuda(__LINE__);$$ =ast_operacao('-',$1,$3);}
	;
operacao_float2: valores
	| operacao_float1 T_MULTIPLICACAO operacao_float1 {ajuda(__LINE__);$$=ast_operacao('*',$1,$3);}
	| operacao_float1 T_DIVISAO operacao_float1 {ajuda(__LINE__);$$=ast_operacao('/',$1,$3);}
	| operacao_float1 T_RESTO operacao_float1 {ajuda(__LINE__);$$=ast_operacao('%',$1,$3);}
	;
valores: T_FLUTUANTE {ajuda(__LINE__);ast_flutuante($1);}
	| T_INTEIRO {ajuda(__LINE__);$$ = ast_inteiro($1);}
	| variavel_declarada {ajuda(__LINE__);$$ = $1;}
	| T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {ajuda(__LINE__);$$ = ast_precedencia($2);}
	| T_EXPONENCIAL T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {ajuda(__LINE__);$$ = ast_chamada("pow",$3);}
	| T_HIP_SIN T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {ajuda(__LINE__);$$=ast_chamada("sinh",$3);}
	| T_HIP_COS T_PARENTESE_ESQUERDO operacao_float1 T_PARENTESE_DIREITO {ajuda(__LINE__);$$ = ast_chamada("cosh",$3);}
	;

/*variavel declarada é extremamente importante, retorna 
 * qualquer variavel declarada no formato float
 */
variavel_declarada: T_ID {
	ajuda(__LINE__);
	$$=ast_chamada($1,NULL);}
	;
controle: controle_if 
	| T_WHILE T_PARENTESE_ESQUERDO demonstracao T_PARENTESE_DIREITO T_CHAVE_ESQUERDA conteudo T_CHAVE_DIREITA {
		ajuda(__LINE__);
		ast_while($3,$6);
		}
	;
controle_if: T_IF T_PARENTESE_ESQUERDO demonstracao T_PARENTESE_DIREITO T_CHAVE_ESQUERDA conteudo T_CHAVE_DIREITA{
		ajuda(__LINE__);
		$$=ast_if($3,$6,NULL);
	}
	| T_IF T_PARENTESE_ESQUERDO demonstracao T_PARENTESE_DIREITO T_CHAVE_ESQUERDA conteudo T_CHAVE_DIREITA T_ELSE T_CHAVE_ESQUERDA conteudo T_CHAVE_DIREITA{
		ajuda(__LINE__);
		$$=ast_if($3,$6,$10);
		}
	;
demonstracao: T_NOT demonstracao {
	ajuda(__LINE__);
	$$=ast_not($2);
	}
	| demonstracao T_OR demonstracao{
		ajuda(__LINE__);
		$$=ast_comparacao("or",$1,$3);
		}
	| demonstracao T_AND demonstracao{
		ajuda(__LINE__);
		$$=ast_comparacao("and",$1,$3);
		}
	| operacao_float1 T_IGUALDADE T_IGUALDADE operacao_float1 {
		ajuda(__LINE__);
		$$=ast_comparacao("==",$1,$4);
		}
	| operacao_float1 T_MENOR operacao_float1 {
		ajuda(__LINE__);
		$$=ast_comparacao("<",$1,$3);
		}
	| operacao_float1 T_MAIOR operacao_float1 {
		ajuda(__LINE__);
		$$=ast_comparacao(">",$1,$3);
		}
	| operacao_float1 T_NOT T_IGUALDADE operacao_float1 {
		ajuda(__LINE__);
		$$=ast_comparacao("!=",$1,$4);
		}
	| operacao_float1 T_MAIOR T_IGUALDADE operacao_float1 {
		ajuda(__LINE__);
		$$=ast_comparacao(">=",$1,$4);
		}
	| operacao_float1 T_MENOR T_IGUALDADE operacao_float1 {
		ajuda(__LINE__);
		$$=ast_comparacao("<=",$1,$4);
		}
	;

%%

int main(int argc,char *argv[]) {
	//printf("tamanho em bytes nodo:%d\n",sizeof(Nodo_Ast));
	//ajuda(__LINE__);
	/*yyin = fopen("teste.c","r+");
	if(yyin){
		printf("teste.c acessado\n");
		tbl_de_simbolos = lista_cria();
		printf("p\n");
		yyparse();
		}*/
	if(argc>1){
		if(yyin = fopen(argv[1],"r+")){
			tbl_de_simbolos = lista_cria();
			printf("p\n");
			yyparse();
			}
		else{
			printf("(comp: Argumento Invalido!)");
			}
		}
	printf("(comp: Erro de compilacao! Input Invalido!)\n");
	fclose(yyin);
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Erro de análise (sintática): %s\n", s);
	fprintf(stderr,"%s",s);
	lista_destroi(tbl_de_simbolos);//libera tabela de simbolos
	exit(1);
}

void ajuda(int d){
	printf("l[%d]-",d);
}
