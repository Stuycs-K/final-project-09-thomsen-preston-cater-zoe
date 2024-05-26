.PHONY: run compile clean
ARGS=recipes/opposition.chef

run: main
	./main $(ARGS)

compile main: main.o parser.o
	gcc -Wall -Werror -fsanitize=address,undefined -o main main.o parser.o -lpcre2-8 

main.o: main.c parser.c parser.h
	gcc -c main.c

parser.o: parser.c parser.h
	gcc -c parser.c

clean:
	rm -f main *.o
