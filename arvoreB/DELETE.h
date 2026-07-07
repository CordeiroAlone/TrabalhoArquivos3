#ifndef AR_DELETE_H
#define AR_DELETE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "CABECALHO.h"
#include "DELETE_ONE.h"
#include "../TAD/buscar.h" 
#include "../TAD/util.h"

void lerlinha_ar(estacao* est);

void DELETE_AR(char *arquivoBIN, char *arquivoBIN_ar, int vezes);
void atualizar_raiz_se_vazia(FILE* indice_arq, CabecalhoArvore* cabecalho_ar);
int deletar_sequencial(estacao* base, FILE* arquivo, FILE* arquivo_ar, CabecalhoArvore* cabecalho_ar);

#endif