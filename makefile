all: TAD/util.o TAD/fornecidas.o TAD/buscar.o TAD/CREATE.o TAD/DELETE.o TAD/SELECT.o TAD/SELECT_ALL.o TAD/UPDATE.o TAD/INSERT.o arvoreB/INSERT_INTO.o arvoreB/CABECALHO.o arvoreB/CREATE.o arvoreB/INSERT.o arvoreB/NO.o arvoreB/BUSCAR.o arvoreB/SELECT.o arvoreB/DELETE.o arvoreB/DELETE_ONE.o merge/SELECTDUPLO.o merge/SELECTUNICO.o merge/ORDENAR.o merge/ORDINTER.o programaTrab.o
	gcc -g TAD/util.o TAD/fornecidas.o TAD/buscar.o TAD/CREATE.o TAD/DELETE.o TAD/SELECT.o TAD/SELECT_ALL.o TAD/UPDATE.o TAD/INSERT.o arvoreB/INSERT_INTO.o arvoreB/CABECALHO.o arvoreB/CREATE.o arvoreB/INSERT.o arvoreB/NO.o arvoreB/BUSCAR.o arvoreB/SELECT.o arvoreB/DELETE.o arvoreB/DELETE_ONE.o merge/SELECTDUPLO.o merge/SELECTUNICO.o merge/ORDENAR.o merge/ORDINTER.o programaTrab.o -o programaTrab -std=c99 -Wall -lm

TAD/util.o:
	gcc -g -c TAD/util.c -o TAD/util.o
	
TAD/fornecidas.o:
	gcc -g -c TAD/fornecidas.c -o TAD/fornecidas.o

TAD/buscar.o:
	gcc -g -c TAD/buscar.c -o TAD/buscar.o

TAD/CREATE.o:
	gcc -g -c TAD/CREATE.c -o TAD/CREATE.o

TAD/DELETE.o:
	gcc -g -c TAD/DELETE.c -o TAD/DELETE.o

TAD/SELECT.o:
	gcc -g -c TAD/SELECT.c -o TAD/SELECT.o
	
TAD/SELECT_ALL.o:
	gcc -g -c TAD/SELECT_ALL.c -o TAD/SELECT_ALL.o

TAD/UPDATE.o:
	gcc -g -c TAD/UPDATE.c -o TAD/UPDATE.o

TAD/INSERT.o:
	gcc -g -c TAD/INSERT.c -o TAD/INSERT.o

arvoreB/INSERT_INTO.o:
	gcc -g -c arvoreB/INSERT_INTO.c -o arvoreB/INSERT_INTO.o

arvoreB/CABECALHO.o:
	gcc -g -c arvoreB/CABECALHO.c -o arvoreB/CABECALHO.o

arvoreB/CREATE.o:
	gcc -g -c arvoreB/CREATE.c -o arvoreB/CREATE.o

arvoreB/INSERT.o:
	gcc -g -c arvoreB/INSERT.c -o arvoreB/INSERT.o

arvoreB/NO.o:
	gcc -g -c arvoreB/NO.c -o arvoreB/NO.o

arvoreB/BUSCAR.o:
	gcc -g -c arvoreB/BUSCAR.c -o arvoreB/BUSCAR.o

arvoreB/SELECT.o:
	gcc -g -c arvoreB/SELECT.c -o arvoreB/SELECT.o

arvoreB/DELETE.o:
	gcc -g -c arvoreB/DELETE.c -o arvoreB/DELETE.o

arvoreB/DELETE_ONE.o:
	gcc -g -c arvoreB/DELETE_ONE.c -o arvoreB/DELETE_ONE.o

merge/SELECTDUPLO.o:
	gcc -g -c merge/SELECTDUPLO.c -o merge/SELECTDUPLO.o

merge/SELECTUNICO.o:
	gcc -g -c merge/SELECTUNICO.c -o merge/SELECTUNICO.o

merge/ORDENAR.o:
	gcc -g -c merge/ORDENAR.c -o merge/ORDENAR.o

merge/ORDINTER.o:
	gcc -g -c merge/ORDINTER.c -o merge/ORDINTER.o

programaTrab.o:
	gcc -g -c programaTrab.c -o programaTrab.o

run:
	./programaTrab

clean:
	rm -f TAD/util.o TAD/fornecidas.o TAD/buscar.o TAD/CREATE.o TAD/DELETE.o TAD/SELECT.o TAD/SELECT_ALL.o TAD/UPDATE.o TAD/INSERT.o arvoreB/INSERT_INTO.o
	rm -f arvoreB/CABECALHO.o arvoreB/CREATE.o arvoreB/INSERT.o arvoreB/NO.o arvoreB/BUSCAR.o arvoreB/SELECT.o arvoreB/DELETE.o arvoreB/DELETE_ONE.o
	rm -f merge/SELECTDUPLO.o merge/SELECTUNICO.o merge/ORDENAR.o merge/ORDINTER.o
	rm -f programaTrab.o
	rm -f programaTrab

r:
	make clean
	make all

zip:
	rm -f trab2.zip
	zip trab2.zip TAD/buscar.c TAD/buscar.h TAD/CREATE.c TAD/CREATE.h TAD/DELETE.c TAD/DELETE.h TAD/fornecidas.c TAD/fornecidas.h TAD/INSERT.c TAD/INSERT.h arvoreB/INSERT_INTO.c arvoreB/INSERT_INTO.h TAD/SELECT_ALL.c TAD/SELECT_ALL.h TAD/SELECT.c TAD/SELECT.h TAD/UPDATE.c TAD/UPDATE.h TAD/util.c TAD/util.h arvoreB/BUSCAR.c arvoreB/BUSCAR.h arvoreB/CABECALHO.c arvoreB/CABECALHO.h arvoreB/CREATE.c arvoreB/CREATE.h arvoreB/INSERT.c arvoreB/INSERT.h arvoreB/NO.c arvoreB/NO.h arvoreB/SELECT.c arvoreB/SELECT.h arvoreB/DELETE.c arvoreB/DELETE.h arvoreB/DELETE_ONE.c arvoreB/DELETE_ONE.h merge/SELECTDUPLO.c merge/SELECTDUPLO.h merge/SELECTUNICO.c merge/SELECTUNICO.h merge/ORDENAR.c merge/ORDENAR.h merge/ORDINTER.c merge/ORDINTER.h estacoes_removidos.bin estacoes.bin estacoes.csv makefile programaTrab.c

subs:
	cp arq_entrada/* ./

memcheck:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./programaTrab