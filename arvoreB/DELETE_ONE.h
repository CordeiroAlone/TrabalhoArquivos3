#ifndef AR_DELETE_ONE_H
#define AR_DELETE_ONE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "NO.h"
#include "CABECALHO.h"
#include "../TAD/util.h"
#include "../TAD/buscar.h"
#include "BUSCAR.h"

typedef enum {
    D_UNDERFLOW,
    D_NORMAL,
    D_ERROR
} RetornoDelecao;

bool remover_chave_no(NoArvore* pag_at, int pos, FILE* arquivo_dados, estacao* temp, int* pares_removidos);
RetornoDelecao deletar_recursivo(FILE *arquivo_ar, FILE* arquivo_dados, int RRN, int chave, CabecalhoArvore* cabecalho, estacao* temp, int* pares_removidos);
void encontrar_sucessor(FILE *arquivo_ar, int RRN, int *chave_sucessora, int *pr_sucessor);
RetornoDelecao tratar_underflow(FILE *arquivo_ar, NoArvore *pai, int idx_filho, int RRN_pai, CabecalhoArvore* cabecalho);
void rotaciona_direita(NoArvore* filho, NoArvore* pai, NoArvore* irmao_esq, int id_esq);
void rotaciona_esquerda(NoArvore* filho, NoArvore* pai, NoArvore* irmao_dir, int idx_filho);
void concatena_esquerda(NoArvore* filho, NoArvore* pai, NoArvore* irmao_esq, int id_esq, CabecalhoArvore* cabecalho);
void concatena_direita(NoArvore* filho, NoArvore* pai, NoArvore* irmao_dir, int idx_filho, CabecalhoArvore* cabecalho);

#endif