#ifndef ORDENAR
#define ORDENAR
#include <stdio.h>
#include <stdbool.h>
#include "../TAD/util.h"

// Le todos as estacoes validas, traz para RAM e faz a ordenação usando o qsort do C
estacao* ordem(FILE *arquivo_en, bool qual, int* ultimo);

// Recebe o arquivo para ordenar(bin1) e manda para outro arquivo (bin2)
void ordenacao(char *bin1, char *bin2, char* Stipo);

#endif