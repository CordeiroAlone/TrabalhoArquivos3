#ifndef CABECALHO_H
#define CABECALHO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../TAD/util.h"

typedef struct {
    char status;       // '1' se consistente e '0' se incosistente
    int noRaiz;        // armazena o RRN do nó (página) raiz do índice árvore-B. Quando a árvore-B está vazia, noRaiz = -1
    int topo;          // armazena o RRN de um registro logicamente removido, ou -1 caso não haja registros logicamente removidos
    int proxRRN;       // armazena o valor do próximo RRN a ser usado para conter um nó (página da árvore-B).
    int nroNos;        // armazena o número de nós (páginas) do índice árvore-B
} CabecalhoArvore;

// Cria a struct de um cabecalho de uma pagina vazia.
CabecalhoArvore* criar_cabecalho();

// Atualiza o cabeçalho de um arquivo para as informações da variavel cabecalho.
void escrever_cabecalho(FILE* arquivo, CabecalhoArvore* cabecalho);

// Recebe um arquivo e retorna o seu cabeçalho em forma de struct.
CabecalhoArvore* ler_cabecalho(FILE* arquivo);

// Atualiza a estabilidade de um arquivo (marcando estável ou instável)
void marcar_estavel(FILE* arquivo);
void marcar_instavel(FILE* arquivo);

#endif