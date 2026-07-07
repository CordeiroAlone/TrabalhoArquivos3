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

// Recebe uma estação e sua RRN e atualiza o seu registro no arquivo binario.
void estacao_para_binario(estacao *est, int index ,FILE* arquivo);
// Recebe uma estação e coloca o seu registro no arquivo binario.
void estacao_para_binario_apontado(estacao *est ,FILE* arquivo);
// Função contrária do estacao_para_binario. Recebe um arquivo e uma RRN e atualiza esses dados na estacao "est".
void binario_para_estacao(estacao *est, int index,FILE* arquivo);
//Função similar a de cima só que com peculiaridades para encaixar no buscar. (começa a busca de onde o ponteiro parou e consome o lixo)
void binario_para_estacao_apontado(estacao *est,FILE* arquivo);

// Função auxiliar para a abertura de um arquivo, com o modo especifiaco com checagem de estabilidade
FILE* abrir_arquivo_validar(char *arquivoBin, char *modo);
// Função auxiliar que desaloca as memórias das strings de uma estação 
void liberar_memoria_estacao(estacao* est);

// Função auxiliar que retorna o nome de uma estacao no char** nome, a partir do seu indice no arquivo.
void pegaNomeEst(char **nome,FILE* arquivo, int index);

#endif