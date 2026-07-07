#ifndef NO_H
#define NO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CABECALHO.h"

#define ORDEM 4
#define MAX_CHAVES (ORDEM - 1) // 3
#define MIN_CHAVES (ORDEM/2 - 1) // 1

typedef enum {
    FOLHA = -1, // (raiz-folha) é folha
    RAIZ = 0,
    INTERMEDIARIO = 1
} enumNo;

typedef struct {
    char removido;     // indica se o nó está logicamente removido ('1' = removido)
    int proximo;       // armazena o RRN do próximo nó logicamente removido
    enumNo tipoNo;     // indica o tipo de um nó
    int nroChaves;     // indicando o número de chaves presentes no nó
    
    int C[3];            // codEstacao
    int PR[3];           // offset do C
    int P[4];            // RRN do filho
} NoArvore;

typedef struct {
    int chave;
    int offset;
    int pdir;
} miniNo;

NoArvore* criar_no(FILE* arquivo_ar, int tipo_no, CabecalhoArvore* cabecalho, int* rrn_alocado);
void escrever_no(FILE *arquivo, NoArvore *no);
NoArvore* ler_no(FILE *arquivo);
int get_offset(int RRN);

#endif