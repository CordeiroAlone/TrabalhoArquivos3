#include "INSERT.h"
#include "BUSCAR.h"

void inserir_chave_no(NoArvore *pag_at, int chave, int pr, int rrn_filho) {
    for(int i = pag_at->nroChaves-1; i >= -1; i--) { // desloca para direita
        if(i < 0 || chave >= pag_at->C[i]) {
            pag_at->C[i+1] = chave;
            pag_at->PR[i+1] = pr;
            pag_at->P[i+2] = rrn_filho; //filho da direita
            break;
        }
        pag_at->C[i+1] = pag_at->C[i];
        pag_at->PR[i+1] = pag_at->PR[i];
        pag_at->P[i+2] = pag_at->P[i+1];
    }
    pag_at->nroChaves++;
}

bool INSERT_ARVORE(FILE *arquivo_ar, int chave, int pr, CabecalhoArvore* cabecalho) {
    if(arquivo_ar == NULL) return false; 

    int raiz = cabecalho->noRaiz;
    if (raiz == -1) { // arvore vazia
        int rrn_novo;
        NoArvore* nova_raiz = criar_no(arquivo_ar, FOLHA, cabecalho, &rrn_novo);
        nova_raiz->nroChaves = 1;
        nova_raiz->C[0] = chave;
        nova_raiz->PR[0] = pr;

        cabecalho->noRaiz = rrn_novo;

        fseek(arquivo_ar, get_offset(rrn_novo), SEEK_SET);
        escrever_no(arquivo_ar, nova_raiz);
        free(nova_raiz);
        return true;
    } else {
        int promo_chave, promo_pr, promo_rrn_child;
        RetornoInsercao mudancaraiz = inserir_recursivo(arquivo_ar, raiz, chave, pr, &promo_chave, &promo_pr, &promo_rrn_child, cabecalho);
        
        if(mudancaraiz == OVERFLOW) {
            // cria nova raiz de cima
            int rrn_nova_raiz;
            NoArvore* nova_raiz = criar_no(arquivo_ar, RAIZ, cabecalho, &rrn_nova_raiz);
            nova_raiz->nroChaves = 1;
            nova_raiz->C[0] = promo_chave;
            nova_raiz->PR[0] = promo_pr;

            nova_raiz->P[0] = raiz;
            nova_raiz->P[1] = promo_rrn_child;

            fseek(arquivo_ar, get_offset(raiz), SEEK_SET);
            NoArvore* raiz_antiga = ler_no(arquivo_ar);
            if (raiz_antiga->tipoNo == RAIZ) {
                raiz_antiga->tipoNo = INTERMEDIARIO;
            }
            fseek(arquivo_ar, get_offset(raiz), SEEK_SET);
            escrever_no(arquivo_ar, raiz_antiga);
            free(raiz_antiga);
            
            cabecalho->noRaiz = rrn_nova_raiz;

            fseek(arquivo_ar, get_offset(rrn_nova_raiz), SEEK_SET);
            escrever_no(arquivo_ar, nova_raiz);
            free(nova_raiz);
        }
        if(mudancaraiz == ERROR) return false;
        else return true;
    }
}

// RRN, chave, pr são os caras que queremos inserir
// promo_chave, promo_r e promo_rrn_child é da estacao que é promovido (recursao)

RetornoInsercao inserir_recursivo(FILE *arquivo_ar, int RRN, int chave, int pr, int* promo_chave, int *promo_pr, int* promo_rrn_child, CabecalhoArvore* cabecalho) {
    if(RRN == -1) { // final arvore
        *promo_chave = chave;
        *promo_pr = pr;
        *promo_rrn_child = -1;
        return OVERFLOW;
    }
    
    fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
    NoArvore* no = ler_no(arquivo_ar);
    
    int pos;
    if(buscar_no(chave, no, &pos)) {
        free(no);
        return ERROR; // ja existe
    }

    int p_b_key, p_b_pr, p_b_child;
    RetornoInsercao RETORNO = inserir_recursivo(arquivo_ar, no->P[pos], chave, pr, &p_b_key, &p_b_pr, &p_b_child, cabecalho);
    
    if(RETORNO == NORMAL || RETORNO == ERROR) {
        free(no);
        return RETORNO;
    }

    bool temEspaco = (no->nroChaves < 3);
    if(temEspaco) {
        inserir_chave_no(no, p_b_key, p_b_pr, p_b_child);
        
        fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
        escrever_no(arquivo_ar, no);
        free(no);
        return NORMAL;
    }
    
    int novo_rrn;
    NoArvore* pag_nova;
    if(no->tipoNo == RAIZ){
        pag_nova = criar_no(arquivo_ar, INTERMEDIARIO, cabecalho, &novo_rrn);
    }else{
        pag_nova = criar_no(arquivo_ar, no->tipoNo, cabecalho, &novo_rrn);
    }
    
    split(p_b_key, p_b_pr, p_b_child, no, promo_chave, promo_pr, promo_rrn_child, pag_nova, novo_rrn);

    fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
    escrever_no(arquivo_ar, no);
    
    fseek(arquivo_ar, get_offset(novo_rrn), SEEK_SET);
    escrever_no(arquivo_ar, pag_nova);
    
    free(no);
    free(pag_nova);
    return OVERFLOW;
}

void split(int chave_at,int offset_at, int child_at, NoArvore *pag_at, int *chave_promovida, int* pr_promovida, int *promo_rrn_child, NoArvore *pag_nova, int novo_RRN){
    miniNo lista[4];
    bool jaInseriu = false;
    
    for(int i = 0; i < 3; i++){
        if(pag_at->C[i] > chave_at && !jaInseriu) {
            lista[i].chave = chave_at;
            lista[i].offset = offset_at;
            lista[i].pdir = child_at;
            
            jaInseriu = true;
        }
        lista[i+jaInseriu].chave = pag_at->C[i];
        lista[i+jaInseriu].offset = pag_at->PR[i];
        lista[i+jaInseriu].pdir = pag_at->P[i + 1];
    }
    if(!jaInseriu){
        lista[3].chave = chave_at;
        lista[3].offset = offset_at;
        lista[3].pdir = child_at;
    }
    
    *chave_promovida = lista[2].chave;
    *pr_promovida = lista[2].offset;
    *promo_rrn_child = novo_RRN;

    pag_at->nroChaves = 2;
    pag_at->C[0] = lista[0].chave;
    pag_at->C[1] = lista[1].chave;
    pag_at->C[2] = -1;
    pag_at->P[1] = lista[0].pdir;
    pag_at->P[2] = lista[1].pdir;
    pag_at->P[3] = -1;
    pag_at->PR[0] = lista[0].offset;
    pag_at->PR[1] = lista[1].offset;
    pag_at->PR[2] = -1;

    pag_nova->nroChaves = 1;
    pag_nova->C[0] = lista[3].chave;
    pag_nova->C[1] = -1;
    pag_nova->C[2] = -1;
    pag_nova->P[0] = lista[2].pdir;
    pag_nova->P[1] = lista[3].pdir;
    pag_nova->P[2] = -1;
    pag_nova->P[3] = -1;
    pag_nova->PR[0] = lista[3].offset;
    pag_nova->PR[1] = -1;
    pag_nova->PR[2] = -1;
}
