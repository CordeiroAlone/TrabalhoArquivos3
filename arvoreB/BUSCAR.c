#include "BUSCAR.h"

bool buscar_no(int key, NoArvore *p_page, int *pos) { //Dado o campo ele retorna a posição do elemento(ou se não está lá)
    for ((*pos) = 0; (*pos) < p_page->nroChaves; (*pos)++) {
        if (key <= p_page->C[*pos]) break;
    }
    
    if (*pos < p_page->nroChaves && key == p_page->C[*pos]) return true; 
    return false;
}

bool busca_arvoreB(FILE* arquivo, int RRN, int key, int *found_RRN, int *found_pos) { //Função pega a chave e retorna a posição do campo da árvore e a posição do elemento dentro do campo.
    if (RRN == -1) return false;

    fseek(arquivo, get_offset(RRN), SEEK_SET);
    NoArvore* page = ler_no(arquivo);
    if(page->removido == '1') {
        free(page);
        page = NULL;
        return false;
    }

    int pos;
    bool found = buscar_no(key, page, &pos);
    
    *found_RRN = RRN;
    *found_pos = pos;
    
    if (!found) {
        int next_RRN = page->P[pos];
        free(page);
        page = NULL;
        return busca_arvoreB(arquivo, next_RRN, key, found_RRN, found_pos);
    }
    free(page);
    page = NULL;
    return true;
}