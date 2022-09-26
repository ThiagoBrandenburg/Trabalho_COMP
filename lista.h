
/* Arvore Sintatica Abstrata*/
typedef struct astnodo{
	enum {tpInteiro=1,tpFlutuante,tpCaracter,tpConteudo,tpDeclaracao,tpRamificacao,tpAtribuicao,tpOperacao,tpPrecedencia,tpChamada,tpChamadaBuildIn,tpControleIf,tpControleWhile,tpNot,tpComparacao}tipo;
	union {
		int valor_int;
		float valor_float;
		char valor_char;
		char *valor_string;
		
		struct{
			int tamanho;
			struct astnodo **lista_de_linhas;
			} Conteudo;//struct onde fica armazenado as linhas e estruturas de controle da main
			
		struct{
			char *nome;
			char *tipo;
			struct astnodo *valor;//pode ser uma função
			} Declaracao;
		struct{
			struct astnodo *esquerda,*direita;
			} Ramificacao;

		struct{
			char op;
			struct astnodo *esquerda,*direita;
			} Operacao;
		struct{
			struct astnodo *valor;
			} Precedencia;
			
		struct{
			struct astnodo *variavel,*valor;
			} Atribuicao;

		struct {//aqui é efetuada uma chamada de uma funcao da tabela de simbolos
			char *nome;
			struct astnodo *argumentos;
			} Chamada;
			
		struct{
			enum {Print_int=1}tipo;
			struct astnodo *argumentos;
			} ChamadaBuildIn;//obsoleto
		
		struct{
			struct astnodo *condicoes,*execucao,*exceto;
			} ControleIf;
		
		struct{
			struct astnodo *condicoes,*execucao;
			} ControleWhile;
			
		struct{
			struct astnodo *afirmacacao;
			} Not;
		struct{
			char *sinal;
			struct astnodo *esquerda, *direita;
			} Comparacao;
			
		} dados;
	}Nodo_Ast;
	
typedef struct tokennodo{
	char *nome;
	enum {Inteiro=1,Flutuante,Funcao}tipo;
	int local_pos;//posicao do token dentro dos locais do bycode

	struct tokennodo *proximo;
	struct tokennodo *anterior;
	
	union{
		float valor_float;
		int valor_int;
		struct{
			Nodo_Ast *execucao;//funcoes
			Nodo_Ast *argumentos;//argumentos caso seja funcao
			} valor_funcao;
		} valor;	
	/* Ponteiros do encadeamento, a lista de filhos serve para reconhecer loops*/
}Nodo_Token;

/*Lista de simbolos*/
typedef struct lista{
	Nodo_Token *pai;
	Nodo_Token *leitor;
	Nodo_Token *inicio;
	int tamanho;
}Lista;

typedef struct{
	int contLinha, contLabel, contLocal, contStack;
	int limitStack, limitLocal;
	} GenInfo;


Nodo_Token* token_nodo_inteiro(char *nome,int valor);
Nodo_Token* token_nodo_flutuante(char *nome, float valor);
Nodo_Token* token_nodo_booleano(char *nome, int valor);
Nodo_Token* token_nodo_simples(char* tipo,char *nome);
/*Nodo_Token* lista_cria_nodo_real(Lista *l, char *nome, float valor);
Nodo_Token* lista_nodo_vazio(Lista *l, char *nome);*/

Lista* lista_cria();
void lista_adiciona_token(Lista *l, Nodo_Token *n);
Nodo_Token* lista_busca(Lista *l, char *nome);
int lista_destroi(Lista *l);
char* token_para_string(Nodo_Token *n);
void lista_imprime_tabela_de_simbolos(Lista *l, char *file);

Nodo_Ast* ast_inteiro(int numero);
Nodo_Ast* ast_flutuante(float numero);
Nodo_Ast* ast_conteudo(Nodo_Ast *ptr_conteudo,Nodo_Ast *ptr_linha);
Nodo_Ast* ast_declaracao(char *nome, char *tipo, Nodo_Ast *valor);
Nodo_Ast* ast_ramificacao(Nodo_Ast *esquerda, Nodo_Ast *direita);
Nodo_Ast* ast_chamada(char *nome, Nodo_Ast *argumentos);
Nodo_Ast* ast_atribuicao(Nodo_Ast *variavel, Nodo_Ast *valor);
Nodo_Ast* ast_operacao(char op,Nodo_Ast *esquerda, Nodo_Ast *direita);
Nodo_Ast* ast_precedencia(Nodo_Ast *valor);
Nodo_Ast* ast_while(Nodo_Ast *condicao,Nodo_Ast *execucao);
Nodo_Ast* ast_if(Nodo_Ast *condicoes, Nodo_Ast *execucao, Nodo_Ast *excessao);
Nodo_Ast *ast_comparacao(char *op, Nodo_Ast *esquerda, Nodo_Ast *direita);
Nodo_Ast *ast_not(Nodo_Ast *afirmacao);
void ast_percorre(Nodo_Ast *raiz);
void ast_destroi(Nodo_Ast *raiz);
void ast_print(Nodo_Ast *raiz);
void ast_printa(Nodo_Ast *raiz);

void gen_code(Nodo_Ast *raiz_ast, Lista *tbl,char *nome_arquivo);
GenInfo* gen_inicializa();
char* gen_declaracao_flutuante(GenInfo *geni, float valor);
char* gen_declaracao_inteiro(GenInfo *geni, int valor);
void gen_declaracoes(Lista *tbl, FILE *arquivo, GenInfo *geni);
