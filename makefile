build:
	bison -d parser.y
	flex scanner.l
	gcc -o parser.out interpreter.c lex.yy.c parser.tab.c 

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c parser.out

run:
	rm -f calls.log
	rm -f nodes.log
	./parser.out < test.js

play:
	rm -f calls.log
	rm -f nodes.log
	./parser.out < play.js