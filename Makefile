all:
	yacc -y -d allonsy.y
	lex allonsy.l
	gcc -c y.tab.c lex.yy.c
	gcc y.tab.o lex.yy.o AST_generator.c avl_tree.c interpreter.c mem_manager.c variable.c -o allonsy -lm -lfl

clean:
	rm lex.yy.c
	rm lex.yy.o
	rm y.tab.c
	rm y.tab.h
	rm y.tab.o
	rm allonsy
	
