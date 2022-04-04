all: assembler

assembler: common.o data_list.o entry_list.o files.o first_pass.o macro_list.o pre_assembler.o second_pass.o symbol_table.o utils.o word_list.o externals.o main.o
	gcc -g -ansi -Wall -pedantic common.o data_list.o entry_list.o files.o first_pass.o macro_list.o pre_assembler.o second_pass.o symbol_table.o utils.o word_list.o externals.o main.o -lm -o assembler

common.o: common.c
	gcc -c -ansi -Wall -pedantic common.c -o common.o

data_list.o: data_list.c
	gcc -c -ansi -Wall -pedantic data_list.c -o data_list.o

entry_list.o: entry_list.c
	gcc -c -ansi -Wall -pedantic entry_list.c -o entry_list.o

files.o: files.c
	gcc -c -ansi -Wall -pedantic files.c -o files.o

first_pass.o: first_pass.c
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

macro_list.o: macro_list.c
	gcc -c -ansi -Wall -pedantic macro_list.c -o macro_list.o

pre_assembler.o: pre_assembler.c
	gcc -c -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o

second_pass.o: second_pass.c
	gcc -c -ansi -Wall -pedantic second_pass.c -o second_pass.o

symbol_table.o: symbol_table.c
	gcc -c -ansi -Wall -pedantic symbol_table.c -o symbol_table.o

utils.o: utils.c
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o

word_list.o: word_list.c
	gcc -c -ansi -Wall -pedantic word_list.c -o word_list.o

externals.o: externals.c
	gcc -c -ansi -Wall -pedantic externals.c -o externals.o

main.o: main.c
	gcc -c -ansi -Wall -pedantic main.c -o main.o