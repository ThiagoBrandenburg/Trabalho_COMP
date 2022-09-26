#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	char linha1[]= "bison -d arquivo_bison.y";
	char linha2[]= "lex arquivo_lex.lex";
	char linha3[]= "gcc arquivo_bison.tab.c lex.yy.c lista.c gerador_de_codigo.c -o comp -lm";
	//char linha4[]= "./compilador";
	
	system(linha1);
	system(linha2);
	system(linha3);
	//system(linha4);
	
	return 0;
	
}
