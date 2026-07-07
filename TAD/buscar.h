#ifndef BUSCAR_H
#define BUSCAR_H

    #include "util.h"

    typedef enum {
        F_SELECT,
        F_DELETE,
        F_UPDATE
    } Funcionalidade;

    bool satisfazBase(estacao* base, estacao* temp);
    int iterar_estacao(estacao* base, FILE* arquivo, Funcionalidade funcionalidade, estacao* transformar);
    int conta_estacao(FILE *ler);
    int conta_par_estacao(FILE *ler);
#endif