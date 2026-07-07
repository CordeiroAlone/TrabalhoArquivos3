#ifndef UTIL_H
#define UTIL_H

// Esse arquivo possui várias funcionalidades auxiliares muitos usadas em todas as funcionalidades.
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Struct da estação, onde os campos funcionam do mesmo modo pedido pela especificação dos trabalhos.
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

// Constantes para facilitar no fwrite
extern const int um;
extern const int zero;
extern const char c_zero;
extern const char c_um;
extern const char cifrao;

// Recebe uma estação e imprimi-a no terminal
void printar_estacao(estacao* estacaoPrintar);

// Recebe uma estação e torna todos os seus campos vazios.
void nulifica_estacao(estacao* temp);

// Conta a quantidade de estações presentes num arquivo
int conta_estacao(FILE* arquivo);

// Recebe no terminal, rep vezes, campos que devem estar contidas numa estacao e atualiza a estacao *nova.
void pegar_info_estacao(estacao *nova,int rep);

// 
void estacao_para_binario(estacao *est, int index ,FILE* arquivo);
void binario_para_estacao(estacao *est, int index,FILE* arquivo);
void binario_para_estacao_apontado(estacao *est,FILE* arquivo);
FILE* abrir_arquivo_validar(char *arquivoBin, char *modo);
void liberar_memoria_estacao(estacao* est);
void pegaNomeEst(char **nome,FILE* arquivo, int index);

#endif