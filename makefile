.PHONY: run compile clean

run: main
	./main 'recipes/opposition.chef'

compile main: main.o parser.o
	gcc -Wall -Werror -fsanitize=address,undefined -o main main.o parser.o

main.o: main.c parser.c parser.h
	gcc -c main.c

parser.o: parser.c parser.h
	gcc -c parser.c

clean:
	rm -f main *.o
