all: calc test

calc: parser.o lexer.o calc.o mathnode.o
	${CXX} -g -o calc calc.o lexer.o parser.o mathnode.o

test: calc
	./calc input.txt | dot -Tpng -o dfa1.png

%.o: %.cpp
	${CXX} -g -c $< -I./

lexer.o: lexer.yy.cc
	${CXX} -g -c $< -o lexer.o

lexer.yy.cc: grammar.hh lexer.l
	flex --outfile lexer.yy.cc lexer.l

grammar.hh: parser.yy
	bison --defines=grammar.hh -d -v parser.yy

parser.cc: parser.yy
	bison --defines=grammar.hh -d -v parser.yy


graph:
	cat graph.test | dot -Tpng -o dfa1.png

.PHONY: clean test
clean: 
	rm -rf *.o *.cc *.hh parser.output calc
