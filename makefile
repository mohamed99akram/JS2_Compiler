build:
	bison -d parser.y
	flex scanner.l
	gcc -o parser interpreter.c lex.yy.c parser.tab.c 

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c parser 

run:
	./parser < test.js