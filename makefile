.PHONY: run compile clean
ARGS=recipes/opposition.chef

run: interpret
	./interpret $(ARGS)

compile: interpret obfuscate

interpret: main.o parser.o types.o execute.o stack.o
	gcc -Wall -Werror -fsanitize=address,undefined -o interpret main.o parser.o types.o execute.o stack.o -lpcre2-8 
	chmod +x interpet

obfuscate: obfuscate.o
	gcc -Wall -Werror -fsanitize=address,undefined -o obfuscate obfuscate.o
	chmod +x obfuscate

main.o: main.c parser.c parser.h types.c types.h execute.c execute.h stack.c stack.h
	gcc -c main.c

parser.o: parser.c parser.h types.c types.h
	gcc -c parser.c

types.o: types.c types.h
	gcc -c types.c

execute.o: execute.c execute.h types.c types.h stack.c stack.h
	gcc -c execute.c

stack.o: stack.c stack.h types.c types.h
	gcc -c stack.c

obfuscate.o: obfuscate.c obfuscate.h
	gcc -c obfuscate.c

clean:
	rm -f interpret *.o
