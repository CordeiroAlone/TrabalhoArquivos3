#ifndef AR_BUSCAR_H
#define AR_BUSCAR_H

#include "NO.h"

// Dado a chave de uma estação, retorna a posição do elemento na página
bool buscar_no(int key, NoArvore *p_page, int *pos);

// Função pega a chave e retorna a posição do campo(página) da árvore e a posição do elemento dentro do campo.
bool busca_arvoreB(FILE* arquivo, int RRN, int key, int *found_RRN, int *found_pos);
#endif