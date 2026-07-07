#ifndef AR_BUSCAR_H
#define AR_BUSCAR_H

#include "NO.h"

bool buscar_no(int key, NoArvore *p_page, int *pos);
bool busca_arvoreB(FILE* arquivo, int RRN, int key, int *found_RRN, int *found_pos);
#endif