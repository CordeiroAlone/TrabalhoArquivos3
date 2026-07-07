#ifndef ORDENAR
#define ORDENAR
#include <stdio.h>
#include <stdbool.h>
#include "../TAD/util.h"

estacao* ordem(FILE *arquivo_en, bool qual, int* ultimo);
void ordenacao(char *bin1, char *bin2, char* Stipo);

#endif