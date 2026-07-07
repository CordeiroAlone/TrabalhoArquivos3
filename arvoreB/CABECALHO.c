#include "CABECALHO.h"

// Cria a struct de um cabecalho de uma pagina vazia.
CabecalhoArvore* criar_cabecalho() {
    CabecalhoArvore* cabecalho = malloc(sizeof(CabecalhoArvore));
    
    cabecalho->status = '0'; // incosistente
    cabecalho->noRaiz = -1;
    cabecalho->topo = -1;  
    cabecalho->proxRRN = 0;
    cabecalho->nroNos = 0;
    
    return cabecalho;
}

// Atualiza o cabeçalho de um arquivo para as informações da variavel cabecalho.
void escrever_cabecalho(FILE* arquivo, CabecalhoArvore* cabecalho) {
    if (arquivo == NULL || cabecalho == NULL) return;

    fseek(arquivo,0,SEEK_SET);

    // Lendo o cabecalho e atualizando o arquivo
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nroNos, sizeof(int), 1, arquivo);
}

// Recebe um arquivo e retorna o seu cabeçalho em forma de struct.
CabecalhoArvore* ler_cabecalho(FILE* arquivo) {
    if (arquivo == NULL) return NULL;

    CabecalhoArvore* cabecalho = malloc(sizeof(CabecalhoArvore));
    
    // Lendo o arquivo e atualizando o cabecalho
    fseek(arquivo,0,SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1, arquivo);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivo);
    fread(&cabecalho->topo, sizeof(int), 1, arquivo);
    fread(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fread(&cabecalho->nroNos, sizeof(int), 1, arquivo);

    return cabecalho;
}

// Atualiza a estabilidade de um arquivo (marcando estável ou instável)

void marcar_estavel(FILE* arquivo) {
    if (arquivo == NULL) return;
    
    char status_estavel = '1';
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&status_estavel, sizeof(char), 1, arquivo); // Atualizando para estavel
}
void marcar_instavel(FILE* arquivo) {
    if (arquivo == NULL) return;
    
    char status_estavel = '0';
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&status_estavel, sizeof(char), 1, arquivo); // Atualizando para instável
}