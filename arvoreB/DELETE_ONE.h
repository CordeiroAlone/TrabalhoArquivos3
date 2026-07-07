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
    D_UNDERFLOW, // Deu underflow, precisa tratar
    D_NORMAL,   // Nao precisa fazer nada
    D_ERROR     // Registro nao foi encontrado
} RetornoDelecao;

// Remove uma chave de um NÓ(página) folha da árvore, deslocando os elementos, e depois deleta do arquivo de dados
bool remover_chave_no(NoArvore* pag_at, int pos, FILE* arquivo_dados, estacao* temp, int* pares_removidos);

// Função recursiva para buscar e deletar UMA chave na árvore
RetornoDelecao deletar_recursivo(FILE *arquivo_ar, FILE* arquivo_dados, int RRN, int chave, CabecalhoArvore* cabecalho, estacao* temp, int* pares_removidos);


// Busca recursivamente a chave sucessora imediata de uma chave localizada num nó interno.
// (menor chave contida na subárvore à direita da chave original, ou seja, vai para o filho da direita e depois segue ao máximo para esquerda)
void encontrar_sucessor(FILE *arquivo_ar, int RRN, int *chave_sucessora, int *pr_sucessor);

// Trata o underflow de um nó filho, tentando rotacionar ou concatenar, seguindo a ordem especificada.
RetornoDelecao tratar_underflow(FILE *arquivo_ar, NoArvore *pai, int idx_filho, int RRN_pai, CabecalhoArvore* cabecalho);

// Realiza a redistribuição de chaves (rotação à direita) pegando emprestado do irmão à esquerda
void rotaciona_direita(NoArvore* filho, NoArvore* pai, NoArvore* irmao_esq, int id_esq);
// Análogo a rotação direita, exceto na parte de deslocamento
void rotaciona_esquerda(NoArvore* filho, NoArvore* pai, NoArvore* irmao_dir, int idx_filho);
// Realiza a concatenação de um nó filho com seu irmão adjacente à esquerda.
void concatena_esquerda(NoArvore* filho, NoArvore* pai, NoArvore* irmao_esq, int id_esq, CabecalhoArvore* cabecalho);
// Análogo a concatena_esquerda.
void concatena_direita(NoArvore* filho, NoArvore* pai, NoArvore* irmao_dir, int idx_filho, CabecalhoArvore* cabecalho);

#endif