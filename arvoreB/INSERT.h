#ifndef AR_INSERT_H
#define AR_INSERT_H

#include "NO.h" 
#include "CABECALHO.h"

typedef enum {
    NORMAL,
    OVERFLOW,
    ERROR
} RetornoInsercao;

RetornoInsercao inserir_recursivo(FILE *arquivo, int RRN, int chave, int pr, int* promo_chave, int *promo_pr, int* promo_rrn_child, CabecalhoArvore* cabecalho);
bool INSERT_ARVORE(FILE *arquivo_ar, int chave, int pr, CabecalhoArvore* cabecalho);
void inserir_chave_no(NoArvore *pag_at, int chave, int pr, int rrn_filho);
void split(int chave_at, int offset_at, int child_at, NoArvore *pag_at, int *chave_promovida, int* pr_promovida, int *promo_rrn_child, NoArvore *pag_nova, int novo_RRN);

#endif