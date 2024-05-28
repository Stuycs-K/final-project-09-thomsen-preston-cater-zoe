.PHONY: run compile clean
ARGS=recipes/opposition.chef

run: main
	./main $(ARGS)

compile main: main.o parser.o types.o execute.o
	gcc -Wall -Werror -fsanitize=address,undefined -o main main.o parser.o types.o execute.o -lpcre2-8 

main.o: main.c parser.c parser.h types.c types.h execute.c execute.h
	gcc -c main.c

parser.o: parser.c parser.h types.c types.h
	gcc -c parser.c

types.o: types.c types.h
	gcc -c types.c

execute.o: execute.c execute.h types.c types.h
	gcc -c execute.c

clean:
	rm -f main *.o
