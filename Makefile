assembler: main.o
	gcc -g -ansi -Wall -pedantic main.o -lm -o assembler

main.o: main.c
	gcc -c -ansi -Wall -pedantic main.c -o main.o