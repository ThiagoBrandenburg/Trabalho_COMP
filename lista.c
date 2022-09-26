#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


Nodo_Token* token_nodo_inteiro(char *nome,int valor){
	printf("(t1)");
	Nodo_Token *n = malloc(sizeof(Nodo_Token));
	n->nome = strdup(nome);
	n->tipo=Inteiro;
	n->valor.valor_int=valor;
	return n;
	}
Nodo_Token* token_nodo_flutuante(char *nome, float valor){
	printf("(t2)");
	Nodo_Token *n=malloc(sizeof(Nodo_Token));
	n->nome = strdup(nome);
	n->tipo=Flutuante;
	n->valor.valor_float=valor;
	return n;
	}
Nodo_Token* token_nodo_booleano(char *nome, int valor){
	printf("(t3)");
	Nodo_Token *n=malloc(sizeof(Nodo_Token));
	n->nome = strdup(nome);
	n->tipo=Inteiro;
	if(valor!=0){
		n->valor.valor_int=1;
		}
	else{
		n->valor.valor_int=0;
		}
	return n;
	}
Nodo_Token* token_nodo_funcao(char *nome, Nodo_Ast *argumentos, Nodo_Ast *execucao){
	printf("(t4)");
	Nodo_Token *n=malloc(sizeof(Nodo_Token));
	n->nome=nome;
	n->tipo=Funcao;
	n->valor.valor_funcao.argumentos=argumentos;
	n->valor.valor_funcao.execucao=execucao;
	return n;
	}

Lista* lista_cria(){
	printf("(t5)");
	Lista *l= malloc(sizeof(Lista));
	l->tamanho = 0;
	l->leitor = NULL;
	l->inicio = NULL;
	return l;
	}

	
void lista_adiciona_token(Lista *l, Nodo_Token *n){
	printf("(t6)");
	if(l->tamanho == 0){
		l->leitor = n;
		l->inicio = n;
		l->leitor->proximo = NULL;
		l->leitor->anterior = NULL;
		}
	else{
		while(l->leitor->proximo!=NULL){
			l->leitor = l->leitor->proximo;
			}
		l->leitor->proximo=n;
		l->leitor->proximo->proximo = NULL;
		l->leitor->proximo->anterior = l->leitor;
		l->leitor = l->leitor->proximo;
		}
	l->tamanho+=1;
	}
	

Nodo_Token* lista_busca(Lista *l, char *nome){
	printf("(t7)");
	Nodo_Token *aux= l->leitor;
	printf("&inicio = %p, &leitor= %p\n",l->inicio,l->leitor);
	printf("inicio= %s, leitor=%s\n",l->inicio->nome,l->leitor->nome);
	do{
		if(strcmp(nome,l->leitor->nome) != 0){
			printf("%s != %s\n",nome,l->leitor->nome);
			if(l->leitor->proximo == NULL){
				l->leitor = l->inicio;
			}
			else{
				l->leitor = l->leitor->proximo;
				}
		}
		else{
			printf("here\n");
			return l->leitor;
			}
		}while(l->leitor !=aux);
	printf("\n(comp:Erro! A variavel %s nao foi declarada)",nome);
	return NULL;
	}

int lista_destroi(Lista *l){
	printf("(t8)");
	Nodo_Token *aux1;
	while(l->tamanho >1){
		printf("tamanho lista=%d",0);
		aux1=l->inicio->proximo;
		if(l->inicio->tipo=Funcao){
			free(l->inicio->valor.valor_funcao.argumentos);
			free(l->inicio->valor.valor_funcao.execucao);
			}
		free(l->inicio);
		l->inicio=aux1;
		l->tamanho-=1;
		}
	if(l->inicio->tipo=Funcao){
		free(l->inicio->valor.valor_funcao.argumentos);
		free(l->inicio->valor.valor_funcao.execucao);
		}
	free(l->inicio);
	printf("aqui");
	free(l);
	printf("(sucess)");
	return 1;
	}

char* token_para_string(Nodo_Token *n){
	printf("(t9)");
	char aux[30];
	strcat(aux,n->nome);
	strcat(aux,",");
	switch(n->tipo){
		case Flutuante:
			strcat(aux,"float");
			break;
		case Inteiro:
			strcat(aux,"int");
			break;
		case Funcao:
			strcat(aux,"func");
			break;
		default:
			break;
		}
	strcat(aux,",");
	if(n->tipo==Flutuante){
		char aux2[10];
		sprintf(aux2,"%f",n->valor.valor_float);
		strcat(aux,aux2);
		}
	if(n->tipo==Inteiro){
		char aux2[10];
		sprintf(aux2,"%d",n->valor.valor_int);
		strcat(aux,aux2);
		}
	strcat(aux,";\n");
	char *str = strdup(aux);
	return str;
	}

void lista_imprime_tabela_de_simbolos(Lista *l, char *file){
	printf("(t10)");
	FILE *f=fopen(file,"wt");
	printf("\n(comp:print to file):");
	l->leitor=l->inicio;
	while(l->leitor!=NULL){
		printf("%s-",l->leitor->nome);
		char *aux_string=token_para_string(l->leitor);
		fprintf(f,"%s",aux_string);
		free(aux_string);
		l->leitor= l->leitor->proximo;
		}
	l->leitor=l->inicio;
	fclose(f);
	printf("(sucess)");
	}

	
Nodo_Ast* ast_inteiro(int numero){
	printf("(a1)");
	Nodo_Ast *ptr_conteudo=calloc(1,sizeof(Nodo_Ast));
	ptr_conteudo->tipo=tpInteiro;
	ptr_conteudo->dados.valor_int=numero;
	return ptr_conteudo;
	}
Nodo_Ast* ast_flutuante(float numero){
	printf("(a2)");
	Nodo_Ast *ptr_conteudo=calloc(1,sizeof(Nodo_Ast));
	ptr_conteudo->tipo=tpFlutuante;
	ptr_conteudo->dados.valor_float=numero;
	return ptr_conteudo;
	}

Nodo_Ast* ast_conteudo(Nodo_Ast *ptr_conteudo,Nodo_Ast *ptr_linha){
	printf("(a3)");
	if(ptr_conteudo==NULL){
		ptr_conteudo=calloc(1,sizeof(Nodo_Ast));
		ptr_conteudo->tipo=tpConteudo;
		ptr_conteudo->dados.Conteudo.tamanho=0;
		}
	ptr_conteudo->dados.Conteudo.tamanho+=1;
	int aux=ptr_conteudo->dados.Conteudo.tamanho;
	ptr_conteudo->dados.Conteudo.lista_de_linhas=realloc(ptr_conteudo->dados.Conteudo.lista_de_linhas,sizeof(Nodo_Ast)*aux);
	ptr_conteudo->dados.Conteudo.lista_de_linhas[aux-1]=ptr_linha;
	return ptr_conteudo;
	}


	
Nodo_Ast* ast_declaracao(char *nome, char *tipo, Nodo_Ast *valor){
	printf("(a4)");
	Nodo_Ast *auxpt = calloc(1,sizeof(Nodo_Ast));
	auxpt->tipo=tpDeclaracao;
	auxpt->dados.Declaracao.nome=strdup(nome);
	auxpt->dados.Declaracao.tipo=strdup(tipo);
	auxpt->dados.Declaracao.valor=valor;
	return auxpt;
	}
Nodo_Ast* ast_ramificacao(Nodo_Ast *esquerda, Nodo_Ast *direita){
	printf("(a5)");
	Nodo_Ast *auxpt = calloc(1,sizeof(Nodo_Ast));
	auxpt->tipo=tpRamificacao;
	auxpt->dados.Ramificacao.esquerda=esquerda;
	auxpt->dados.Ramificacao.direita=direita;
	return auxpt;
	}
Nodo_Ast* ast_chamada(char *nome, Nodo_Ast *argumentos){
	printf("(a6)");
	Nodo_Ast *aux_pt = calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo=tpChamada;
	aux_pt->dados.Chamada.nome=strdup(nome);
	aux_pt->dados.Chamada.argumentos=argumentos;
	return aux_pt;
	}
Nodo_Ast* ast_atribuicao(Nodo_Ast *variavel, Nodo_Ast *valor){
	printf("(a7)");
	Nodo_Ast *aux_pt = calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo = tpAtribuicao;
	aux_pt->dados.Atribuicao.variavel=variavel;
	aux_pt->dados.Atribuicao.valor=valor;
	return aux_pt;
	}
Nodo_Ast* ast_operacao(char op,Nodo_Ast *esquerda, Nodo_Ast *direita){
	printf("(a8)");
	Nodo_Ast *aux_pt = calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo = tpOperacao;
	aux_pt->dados.Operacao.op=op;
	aux_pt->dados.Operacao.esquerda=esquerda;
	aux_pt->dados.Operacao.direita=direita;
	return aux_pt;
	}
Nodo_Ast* ast_precedencia(Nodo_Ast *valor){
	printf("(a9)");
	Nodo_Ast *aux_pt = calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo = tpPrecedencia;
	aux_pt->dados.Precedencia.valor = valor;
	return aux_pt;
	}
	
Nodo_Ast* ast_while(Nodo_Ast *condicao,Nodo_Ast *execucao){
	printf("(a10)");
	Nodo_Ast *aux_pt = calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo= tpControleWhile;
	aux_pt->dados.ControleWhile.condicoes=condicao;
	aux_pt->dados.ControleWhile.execucao=execucao;
	return aux_pt;
	}
Nodo_Ast* ast_if(Nodo_Ast *condicoes, Nodo_Ast *execucao, Nodo_Ast *excessao){
	printf("(a11)");
	Nodo_Ast *aux_pt = calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo=tpControleIf;
	aux_pt->dados.ControleIf.condicoes=condicoes;
	aux_pt->dados.ControleIf.execucao=execucao;
	aux_pt->dados.ControleIf.exceto=excessao;
	return aux_pt;
	}

Nodo_Ast *ast_not(Nodo_Ast *afirmacao){
	printf("(a12)");
	Nodo_Ast *aux_pt=calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo=tpNot;
	aux_pt->dados.Not.afirmacacao=afirmacao;
	return aux_pt;
	}
	
Nodo_Ast *ast_comparacao(char *op, Nodo_Ast *esquerda, Nodo_Ast *direita){
	printf("(a13)");
	Nodo_Ast *aux_pt=calloc(1,sizeof(Nodo_Ast));
	aux_pt->tipo=tpComparacao;
	aux_pt->dados.Comparacao.sinal=strdup(op);
	aux_pt->dados.Comparacao.esquerda=esquerda;
	aux_pt->dados.Comparacao.direita=direita;
	return aux_pt;
	}


void ast_percorre(Nodo_Ast *raiz){
	printf("(a14)");
	/*tpInteirox,tpFlutuantex,tpCaracterx,tpConteudox,tpDeclaracaox,tpRamificacaox,tpAtribuicaox,tpOperacaox,tpPrecedenciax,
	 * tpChamadax,tpChamadaBuildInx,tpControleIfx,tpControleWhilex,tpNot,tpComparacao
	*/
	if(raiz!=NULL){
		int i;
		switch(raiz->tipo){
			case tpInteiro:
				//acoes
				break;
			case tpFlutuante:
				//acoes
				break;
			case tpCaracter:
				//acoes
				break;
			case tpConteudo:
				//acoes
				for(i=0;i<raiz->dados.Conteudo.tamanho;i++){
					ast_percorre(raiz->dados.Conteudo.lista_de_linhas[i]);
					}
				break;
			case tpDeclaracao:
				//acoes
				ast_percorre(raiz->dados.Declaracao.valor);
				break;
			case tpRamificacao:
				ast_percorre(raiz->dados.Ramificacao.esquerda);
				ast_percorre(raiz->dados.Ramificacao.direita);
				break;
			case tpAtribuicao:
				ast_percorre(raiz->dados.Atribuicao.variavel);
				ast_percorre(raiz->dados.Atribuicao.valor);
				break;
			case tpPrecedencia:
				ast_percorre(raiz->dados.Precedencia.valor);
				break;
			case tpOperacao:
				//acoes
				ast_percorre(raiz->dados.Operacao.esquerda);
				ast_percorre(raiz->dados.Operacao.direita);
				break;
			case tpChamada:
				//acoes
				ast_percorre(raiz->dados.Chamada.argumentos);
				break;
			case tpChamadaBuildIn:
				//acoes
				ast_percorre(raiz->dados.ChamadaBuildIn.argumentos);
				break;
			case tpControleIf:
				//acoes
				ast_percorre(raiz->dados.ControleIf.condicoes);
				ast_percorre(raiz->dados.ControleIf.execucao);
				ast_percorre(raiz->dados.ControleIf.exceto);
				break;
			case tpControleWhile:
				//acoes
				ast_percorre(raiz->dados.ControleWhile.condicoes);
				ast_percorre(raiz->dados.ControleWhile.execucao);
				break;
			case tpNot:
				ast_percorre(raiz->dados.Not.afirmacacao);
				break;
			case tpComparacao:
				ast_percorre(raiz->dados.Comparacao.esquerda);
				ast_percorre(raiz->dados.Comparacao.direita);
				break;
			default:
				break;
			}
		}
	}

void ast_destroi(Nodo_Ast *raiz){
	printf("(a15)");
	/*tpInteirox,tpFlutuantex,tpCaracterx,tpConteudox,tpDeclaracaox,tpRamificacao,tpAtribuicao,tpOperacao,tpPrecedencia,
	 * tpChamada,tpChamadaBuildIn,tpControleIf,tpControleWhile,tpNot,tpComparacao
	*/
	if(raiz!=NULL){
		int i;
		switch(raiz->tipo){
			case tpInteiro:
				break;
			case tpFlutuante:
				break;
			case tpCaracter:
				break;
			case tpConteudo:
				for(i=0;i<raiz->dados.Conteudo.tamanho;i++){
					ast_destroi(raiz->dados.Conteudo.lista_de_linhas[i]);
					}
				break;
			case tpDeclaracao:
				free(raiz->dados.Declaracao.nome);
				free(raiz->dados.Declaracao.tipo);
				ast_destroi(raiz->dados.Declaracao.valor);
				break;
			case tpRamificacao:
				ast_destroi(raiz->dados.Ramificacao.esquerda);
				ast_destroi(raiz->dados.Ramificacao.direita);
				break;
			case tpAtribuicao:
				ast_destroi(raiz->dados.Atribuicao.variavel);
				ast_destroi(raiz->dados.Atribuicao.valor);
				break;
			case tpPrecedencia:
				ast_destroi(raiz->dados.Precedencia.valor);
				break;
			case tpOperacao:
				ast_destroi(raiz->dados.Operacao.esquerda);
				ast_destroi(raiz->dados.Operacao.direita);
				break;
			case tpChamada:
				free(raiz->dados.Chamada.nome);
				ast_destroi(raiz->dados.Chamada.argumentos);
				break;
			case tpChamadaBuildIn:
				ast_destroi(raiz->dados.ChamadaBuildIn.argumentos);
				break;
			case tpControleIf:
				ast_destroi(raiz->dados.ControleIf.condicoes);
				ast_destroi(raiz->dados.ControleIf.execucao);
				ast_destroi(raiz->dados.ControleIf.exceto);
				break;
			case tpControleWhile:
				ast_destroi(raiz->dados.ControleWhile.condicoes);
				ast_destroi(raiz->dados.ControleWhile.execucao);
				break;
			case tpNot:
				ast_destroi(raiz->dados.Not.afirmacacao);
				break;
			case tpComparacao:
				free(raiz->dados.Comparacao.sinal);
				ast_destroi(raiz->dados.Comparacao.esquerda);
				ast_destroi(raiz->dados.Comparacao.direita);
				break;
			default:
				break;
			}
		free(raiz);
		}
	}
void ast_print(Nodo_Ast *raiz){
	printf("(a16)");
	ast_printa(raiz);
	}

void ast_printa(Nodo_Ast *raiz){
	/*tpInteirox,tpFlutuantex,tpCaracterx,tpConteudox,tpDeclaracaox,tpRamificacaox,tpAtribuicaox,tpOperacaox,tpPrecedenciax,
	 * tpChamadax,tpChamadaBuildInx,tpControleIfx,tpControleWhilex,tpNot,tpComparacao
	*/
	if(raiz!=NULL){
		int i;
		switch(raiz->tipo){
			case tpInteiro:
				printf("#%d",raiz->dados.valor_int);
				break;
			case tpFlutuante:
				printf("#%f",raiz->dados.valor_float);
				break;
			case tpCaracter:
				printf("#%c",raiz->dados.valor_char);
				break;
			case tpConteudo:
				//acoes
				for(i=0;i<raiz->dados.Conteudo.tamanho;i++){
					printf("#\n");
					ast_printa(raiz->dados.Conteudo.lista_de_linhas[i]);
					}
				break;
			case tpDeclaracao:
				printf("#%s %s",raiz->dados.Declaracao.tipo,raiz->dados.Declaracao.nome);
				ast_printa(raiz->dados.Declaracao.valor);
				break;
			case tpRamificacao:
				ast_printa(raiz->dados.Ramificacao.esquerda);
				printf("#,");
				ast_printa(raiz->dados.Ramificacao.direita);
				break;
			case tpAtribuicao:
				ast_printa(raiz->dados.Atribuicao.variavel);
				printf("#=");
				ast_printa(raiz->dados.Atribuicao.valor);
				break;
			case tpPrecedencia:
				printf("#(");
				ast_printa(raiz->dados.Precedencia.valor);
				printf(")");
				break;
			case tpOperacao:
				ast_printa(raiz->dados.Operacao.esquerda);
				printf("#%c",raiz->dados.Operacao.op);
				ast_printa(raiz->dados.Operacao.direita);
				break;
			case tpChamada:
				printf("#%s",raiz->dados.Chamada.nome);
				if(raiz->dados.Chamada.argumentos!=NULL){
					printf("(");
					ast_printa(raiz->dados.Chamada.argumentos);
					printf(")");
				}
				break;
			case tpChamadaBuildIn:
				//acoes
				ast_printa(raiz->dados.ChamadaBuildIn.argumentos);
				break;
			case tpControleIf:
				//acoes
				printf("#if(");
				ast_printa(raiz->dados.ControleIf.condicoes);
				printf("){");
				ast_printa(raiz->dados.ControleIf.execucao);
				printf("}else{");
				ast_printa(raiz->dados.ControleIf.exceto);
				printf("}");
				break;
			case tpControleWhile:
				//acoes
				while("#while (");
				ast_printa(raiz->dados.ControleWhile.condicoes);
				printf("){");
				ast_printa(raiz->dados.ControleWhile.execucao);
				printf("}");
				break;
			case tpNot:
				printf("!#");
				ast_printa(raiz->dados.Not.afirmacacao);
				break;
			case tpComparacao:
				ast_printa(raiz->dados.Comparacao.esquerda);
				printf("#%s",raiz->dados.Comparacao.sinal);
				ast_printa(raiz->dados.Comparacao.direita);
				break;
			default:
				break;
			}
		}
	
	}

