#include "BUSCAR.h"

// Dado a chave de uma estação, retorna a posição do elemento na página

// int key: chave para a busca
// NoArvore* p_page: Página onde a busca ocorre
// int *pos: Variável a ser atualizada para a posição onde foi encontrada a chave
// Retorno: true se for encontrado e false se não for
bool buscar_no(int key, NoArvore *p_page, int *pos) {
    for ((*pos) = 0; (*pos) < p_page->nroChaves; (*pos)++) {
        if (key <= p_page->C[*pos]) break;
    }
    
    if (*pos < p_page->nroChaves && key == p_page->C[*pos]) return true; 
    return false;
}

// Função pega a chave e retorna a posição do campo(página) da árvore e a posição do elemento dentro do campo.

// FILE* arquivo: arquivo da arvore onde ocorre a busca
// int RRN: o primeiro RRN de onde começa a busca. Normalmente é o RRN do nó raiz se for passar por toda a árvore.
// int key: chave (codEstacao) da estação a ser buscada
// int *found_RRN: Retorna o RRN da pagina de onde a chave foi encontrada
// int *found_pos: Retorna a posição dentro da pagina onde se encontra a chave
// Retorno: true se encontrar e false se não encontrar.
bool busca_arvoreB(FILE* arquivo, int RRN, int key, int *found_RRN, int *found_pos) {
    if (RRN == -1) return false; // Se tiver numa pagina invalida, retorna

    fseek(arquivo, get_offset(RRN), SEEK_SET);
    NoArvore* page = ler_no(arquivo);

    if(page->removido == '1') { // Se tiver numa pagina removida, retorna
        free(page);
        page = NULL;
        return false;
    }

    int pos;
    // Found representa se a chave está na pagina atual. Se tiver, atualizamos o found_RRN e found_pos e se nao tiver, passamos para a proxima pagina a partir do filho da pos retornada.
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