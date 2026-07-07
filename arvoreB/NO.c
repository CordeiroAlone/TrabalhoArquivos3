#include "NO.h"
#include "CABECALHO.h"

NoArvore* criar_no(FILE* arquivo_ar, int tipo_no, CabecalhoArvore* cabecalho, int* rrn_alocado) {
    NoArvore *no = NULL;

    if (cabecalho->topo == -1) {
        no = malloc(sizeof(NoArvore));
        *rrn_alocado = cabecalho->proxRRN;
        cabecalho->proxRRN++;
    } else {
        *rrn_alocado = cabecalho->topo;
        fseek(arquivo_ar, 17 + (*rrn_alocado) * 53, SEEK_SET); 
        no = ler_no(arquivo_ar);
        cabecalho->topo = no->proximo;
    }
    cabecalho->nroNos++;

    no->removido = '0';
    no->proximo = -1;
    no->tipoNo = tipo_no;
    no->nroChaves = 0;
    
    for (int i = 0; i < 3; i++) {
        no->C[i] = -1;
        no->PR[i] = -1;
        no->P[i] = -1;
    }
    no->P[3] = -1;
    return no;
}
void escrever_no(FILE *arquivo, NoArvore *no) {
    if (arquivo == NULL || no == NULL) return;

    fwrite(&no->removido, sizeof(char), 1, arquivo);
    fwrite(&no->proximo, sizeof(int), 1, arquivo);
    fwrite(&no->tipoNo, sizeof(int), 1, arquivo);
    fwrite(&no->nroChaves, sizeof(int), 1, arquivo);
    
    for (int i = 0; i < 3; i++) {
        fwrite(&no->C[i], sizeof(int), 1, arquivo);
        fwrite(&no->PR[i], sizeof(int), 1, arquivo);
    }
    
    for (int i = 0; i < 4; i++) {
        fwrite(&no->P[i], sizeof(int), 1, arquivo);
    }
}

NoArvore* ler_no(FILE *arquivo) {
    if (arquivo == NULL) return NULL;

    NoArvore *no = malloc(sizeof(NoArvore));

    fread(&no->removido, sizeof(char), 1, arquivo);
    fread(&no->proximo, sizeof(int), 1, arquivo);
    fread(&no->tipoNo, sizeof(int), 1, arquivo);
    fread(&no->nroChaves, sizeof(int), 1, arquivo);
    
    for (int i = 0; i < 3; i++) {
        fread(&no->C[i], sizeof(int), 1, arquivo);
        fread(&no->PR[i], sizeof(int), 1, arquivo);
    }
    
    for (int i = 0; i < 4; i++) {
        fread(&no->P[i], sizeof(int), 1, arquivo);
    }

    return no;
}

int get_offset(int RRN) {
    return 17 + 53*RRN;
}