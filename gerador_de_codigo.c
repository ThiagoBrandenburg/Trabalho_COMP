/*
 * gerador_de_codigo.c
 * 
 * Copyright 2021 Compiladores <com0002@VM-com0002>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


void gen_code(Nodo_Ast *raiz_ast, Lista *tbl,char *nome_arquivo){	
	printf("(g1)");
	//Contador de labels;
	GenInfo *geni=gen_inicializa();
	geni->limitLocal=100;//pode ser passados como argumentos
	geni->limitStack=5;
	char linha_aux[22];
	FILE *arquivo=fopen(nome_arquivo,"wt");
	do{
		//Cabeçalho do arquivo .j
		char **linhas=malloc(sizeof(char*)*15);
		linhas[0]=strdup(".class public start");
		linhas[1]=strdup(".super java/lang/Object");
		linhas[2]=strdup(".method public <init>()V");
		linhas[3]=strdup("\taload_0");
		linhas[4]=strdup("\tinvokespecial java/lang/Object/<init>()V");
		linhas[5]=strdup("\treturn");
		linhas[6]=strdup(".end method");
		linhas[7]=strdup(".method public static main([Ljava/lang/String;)V");
		sprintf(linha_aux,"\t.limit stack %d",geni->limitStack);
		linhas[8]=strdup(linha_aux);
		sprintf(linha_aux,"\tlimit locals %d",geni->limitLocal);//parametros de formacao do stack e locals
		linhas[9]=strdup(linha_aux);
		linhas[10]=strdup("\tldc 0");
		linhas[11]=strdup("\tistore 1 ;");
		linhas[12]=strdup("\tldc 0");
		linhas[13]=strdup("\tistore 50;");
		linhas[14]=strdup("Label1:");
		geni->contLabel+=1;
		for(int i=0;i<15;i++){
			fprintf(arquivo,"%s\n",linhas[i]);
			geni->contLinha+=1;
			free(linhas[i]);
			}
		}while(0);
	//aqui fica o acesso a arvore de dados
	gen_declaracoes(tbl,arquivo,geni);
	//gen_instrucoes(arquivo,geni);
	fprintf(arquivo,".end method");//termino do arquivo
	fclose(arquivo);
	free(geni);
	
	};

GenInfo* gen_inicializa(){
	printf("(g2)");
	GenInfo *aux=malloc(sizeof(GenInfo));
	aux->contLabel=0;
	aux->contLocal=0;
	aux->contStack=0;
	aux->limitLocal=100;
	aux->limitStack=5;
	aux->contLinha=0;
	}

char* gen_declaracao_flutuante(GenInfo *geni, float valor){
	printf("(g3)");
	char aux1[30], aux2[15], *aux3;
	sprintf(aux1,"\tfconst_%f\n",valor);
	geni->contLocal+=1;
	sprintf(aux2,"\tfstore_%d\n",geni->contLocal);
	strcat(aux1,aux2);
	aux3=strdup(aux1);
	geni->contLinha+=2;
	return aux3;
	}
char* gen_declaracao_inteiro(GenInfo *geni, int valor){
	printf("(g4)");
	char aux1[30], aux2[15], *aux3;
	sprintf(aux1,"\ticonst_%d\n",valor);
	geni->contLocal+=1;
	sprintf(aux2,"\tistore_%d\n",geni->contLocal);
	strcat(aux1,aux2);
	aux3=strdup(aux1);
	geni->contLinha+=2;
	return aux3;
	}

void gen_declaracoes(Lista *tbl, FILE *arquivo, GenInfo *geni){
	printf("(g5)");
	tbl->leitor=tbl->inicio;
	char *aux1;
	Nodo_Token *aux_token=NULL;
	while(tbl->leitor->proximo!=NULL){
		switch(tbl->leitor->tipo){
			case Flutuante:
			/*
			 * Perceba que devido a utilização de calloc na alocação, podemos confiantemente passar
			 * a declaracao de variavel, pois caso ela não esteja declarada teremos 0 em memória.
			 * 
			 * */
				aux1=gen_declaracao_flutuante(geni,tbl->leitor->valor.valor_float);
				fprintf(arquivo,"%s",aux1);
				free(aux1);
				break;
			case Inteiro:
				aux1=gen_declaracao_inteiro(geni,tbl->leitor->valor.valor_int);
				fprintf(arquivo,"%s",aux1);
				free(aux1);
				break;
			/*AQUI ficaria praticamente todo o resto da montagem,
			 *  podemos perceber que só seria implementacao exaustiva daqui para frente
			 *  toda a parte lógica do trabalho já foi implementada
			 */
			default:
				break;
			}
		tbl->leitor=tbl->leitor->proximo;
		}	
	}
/*
void gen_instrucoes(Nodo_Ast *raiz, FILE *arquivo, GenInfo *geni){
	printf("(a14)");
	//tpInteirox,tpFlutuantex,tpCaracterx,tpConteudox,tpDeclaracaox,tpRamificacaox,tpAtribuicaox,tpOperacaox,tpPrecedenciax,
	//tpChamadax,tpChamadaBuildInx,tpControleIfx,tpControleWhilex,tpNot,tpComparacao
	if(raiz!=NULL){
		int i;
		switch(raiz->tipo){
			case tpInteiro:
			
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
					gen_instrucoes(raiz->dados.Conteudo.lista_de_linhas[i],arquivo,geni);
					}
				break;
			case tpDeclaracao:
				do{
					if(!strcmp(raiz->dados.Declaracao.tipo,"int"){
						fprintf(arquivo,sprintf(gen_declaracao_inteiro(geni,raiz->dados.Declaracao.valor)));
						continue;
						}
					if(!strcmp(raiz->dados.Declaracao.tiṕo,"float"){
						fprintf(arquivo,sprintf(gen_declaracao_flutuante(geni,raiz->dados.Declaracao.valor)));
						continue;
						}
				}while(0);
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
char* genLabelNome(int cont){
	char aux1[10], *aux2;
	sprintf(aux1,"Label%d",cont);
	aux2=strdup(aux1);
	return aux2;
	}
*/
	
