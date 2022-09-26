build_flex:
	@lex arquivo_lex.lex

build_bison:
	@bison -d arquivo_bison.y

build_exec:
	@gcc arquivo_bison.tab.c lex.yy.c lista.c gerador_de_codigo.c -o comp -lm

build_all:
	@lex arquivo_lex.lex
	@bison -d arquivo_bison.y 
	@gcc arquivo_bison.tab.c lex.yy.c lista.c gerador_de_codigo.c -o compilador -lm

build_java: 
	@java -jar jasmin.jar java_bytecode.j
	@java java_class