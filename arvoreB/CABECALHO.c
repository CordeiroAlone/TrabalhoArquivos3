#include "CABECALHO.h"

CabecalhoArvore* criar_cabecalho() {
    CabecalhoArvore* cabecalho = malloc(sizeof(CabecalhoArvore));
    
    cabecalho->status = '0'; // incosistente
    cabecalho->noRaiz = -1;
    cabecalho->topo = -1;  
    cabecalho->proxRRN = 0;
    cabecalho->nroNos = 0;
    
    return cabecalho;
}

void escrever_cabecalho(FILE* arquivo, CabecalhoArvore* cabecalho) {
    if (arquivo == NULL || cabecalho == NULL) return;

    fseek(arquivo,0,SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nroNos, sizeof(int), 1, arquivo);
}

CabecalhoArvore* ler_cabecalho(FILE* arquivo) {
    if (arquivo == NULL) return NULL;

    CabecalhoArvore* cabecalho = malloc(sizeof(CabecalhoArvore));
    
    fseek(arquivo,0,SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1, arquivo);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivo);
    fread(&cabecalho->topo, sizeof(int), 1, arquivo);
    fread(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fread(&cabecalho->nroNos, sizeof(int), 1, arquivo);

    return cabecalho;
}

void marcar_estavel(FILE* arquivo) {
    if (arquivo == NULL) return;
    
    char status_estavel = '1';
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&status_estavel, sizeof(char), 1, arquivo);
}

void marcar_instavel(FILE* arquivo) {
    if (arquivo == NULL) return;
    
    char status_estavel = '0';
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&status_estavel, sizeof(char), 1, arquivo);
}