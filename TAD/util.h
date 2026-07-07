#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Estacao {
    char removido;
    int proximo;
    int codEstacao;
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char* nomeEstacao;
    int tamNomeLinha;
    char* nomeLinha;
} estacao;

extern const int um;
extern const int zero;
extern const char c_zero;
extern const char c_um;
extern const char cifrao;

void printar_estacao(estacao* estacaoPrintar);
void nulifica_estacao(estacao* temp);
int conta_estacao(FILE* arquivo);
void pegar_info_estacao(estacao *nova,int rep);
void estacao_para_binario(estacao *est, int index ,FILE* arquivo);
void binario_para_estacao(estacao *est, int index,FILE* arquivo);
void binario_para_estacao_apontado(estacao *est,FILE* arquivo);
FILE* abrir_arquivo_validar(char *arquivoBin, char *modo);
void liberar_memoria_estacao(estacao* est);

#endif