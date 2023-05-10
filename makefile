build_interpreter:
	bison -d parser.y
	flex scanner.l
	gcc -o parser interpreter.c lex.yy.c parser.tab.c 

build_compiler:
	bison -d parser.y
	flex scanner.l
	gcc -o compiler compiler.c lex.yy.c parser.tab.c


clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c parser.out

run:
	rm -f calls.log
	rm -f nodes.log
	./parser < test.js

play:
	rm -f calls.log
	rm -f nodes.log
	./parser < play.js

expr:
	rm -f calls.log
	rm -f nodes.log
	./parser < expr.js