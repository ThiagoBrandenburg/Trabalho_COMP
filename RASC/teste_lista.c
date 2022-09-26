#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

int main(){
	printf("\n1");
	Lista *l= lista_cria();
	printf("\n2");
	Nodo_Token *a1= token_nodo_inteiro("a1",3);
	printf("\n3");
	Nodo_Token *a2= token_nodo_inteiro("b1",2);
	lista_adiciona_token(l,a1);
	lista_adiciona_token(l,a2);
	printf("\n4");
	printf("\nl: tamanho = %d, inicio = %d, leitor = %d",l->tamanho,l->inicio->valor_inteiro, l->leitor->valor_inteiro);
	lista_destroi(l);
	system("PAUSE");
	return 0;
}
