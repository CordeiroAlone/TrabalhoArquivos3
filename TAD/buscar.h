#ifndef BUSCAR_H
#define BUSCAR_H
    // Funções auxiliares para todas as funcionalidades que utilizam a busca sequencial.

    #include "util.h"

    // Define qual funcionalidade devemos realizar ao iterar_estacao.
    typedef enum {
        F_SELECT,
        F_DELETE,
        F_UPDATE
    } Funcionalidade;

    // Verifica se a estacao temp é uma estacao que é validada pela base
    bool satisfazBase(estacao* base, estacao* temp);

    // Passa por todas as estações do arquivo e realiza a funcionalidade especificada.
    int iterar_estacao(estacao* base, FILE* arquivo, Funcionalidade funcionalidade, estacao* transformar);
    
    // Conta a quantidade de estações e pares de estações presentes num arquivo.
    int conta_estacao(FILE *ler);
    int conta_par_estacao(FILE *ler);
#endif