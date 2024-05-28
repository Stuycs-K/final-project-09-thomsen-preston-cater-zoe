.PHONY: run compile clean
ARGS=recipes/opposition.chef

run: main
	./main $(ARGS)

compile main: main.o parser.o types.o stack.o
	gcc -Wall -Werror -fsanitize=address,undefined -o main main.o parser.o types.o stack.o -lpcre2-8 

main.o: main.c parser.c parser.h types.c types.h stack.c stack.h
	gcc -c main.c

parser.o: parser.c parser.h types.c types.h
	gcc -c parser.c

types.o: types.c types.h
	gcc -c types.c

stack.o: stack.c stack.h
	gcc -c stack.c

clean:
	rm -f main *.o
