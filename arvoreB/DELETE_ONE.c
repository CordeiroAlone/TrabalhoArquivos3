#include "DELETE_ONE.h"
#include "CABECALHO.h"

// Remove uma chave de um NÓ(página) folha da árvore
bool remover_chave_no(NoArvore* pag_at, int pos, FILE* arquivo_dados, estacao* temp, int* pares_removidos) {
    int offset_dados = pag_at->PR[pos]; // PR é o ponteiro para o arquivo de dados
    
    fseek(arquivo_dados, offset_dados+1, SEEK_SET); // byte 'removido'
    estacao possivel_estacao;
    
    // Lê a estação para ver se atendo aos criterios
    binario_para_estacao_apontado(&possivel_estacao, arquivo_dados);
    
    // Se a busca tiver critérios (temp != NULL) e a estação não os satisfizer, cancela a remoção
    if(temp != NULL && !satisfazBase(temp, &possivel_estacao)) {
        liberar_memoria_estacao(&possivel_estacao);
        return false; 
    }

    // Se não, faz a remoção no arquivo de dados
    if(temp != NULL) {
        // Incrementra um par removido, se tiver par
        if (possivel_estacao.codEstacao != -1 && possivel_estacao.codProxEstacao != -1) (*pares_removidos)++;

        // Lê o topo da pilha de removidos, marca o registro como removido e atualiza o cabeçalho com o novo topo
        int topo_dados;
        fseek(arquivo_dados, 1, SEEK_SET);
        fread(&topo_dados, sizeof(int), 1, arquivo_dados);
        int rrn_removido = (offset_dados - 17) / 80;
        char removido = '1';
        fseek(arquivo_dados, offset_dados, SEEK_SET);
        fwrite(&removido, sizeof(char), 1, arquivo_dados);
        fwrite(&topo_dados, sizeof(int), 1, arquivo_dados);
        fseek(arquivo_dados, 1, SEEK_SET);
        fwrite(&rrn_removido, sizeof(int), 1, arquivo_dados);
    }

    liberar_memoria_estacao(&possivel_estacao);

    // Remove a chave da página da árvore deslocando os elementos para esquerda
    int n = pag_at->nroChaves;
    for(int i = pos; i < n-1; i++) {
        pag_at->C[i] = pag_at->C[i+1];
        pag_at->PR[i] = pag_at->PR[i+1];
        pag_at->P[i+1] = pag_at->P[i+2];
    }

    // Limpa a última posição que ficou duplicada após o deslocamento
    pag_at->C[n-1] = -1;
    pag_at->PR[n-1] = -1;
    pag_at->P[n] = -1;
    pag_at->nroChaves--;

    return true;
}

// Função recursiva para buscar e deletar UMA chave na árvore
RetornoDelecao deletar_recursivo(FILE *arquivo_ar, FILE* arquivo_dados, int RRN, int chave, CabecalhoArvore* cabecalho, estacao* temp, int* pares_removidos) {
    if(RRN == -1) {
        return D_ERROR; // Chegou no final da árvore: não encontrou a chave
    }

    fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
    NoArvore* pag_at = ler_no(arquivo_ar);
    int pos;

    // buscar_no retorna verdadeiro se a chave a ser procurada está na página(nó) atual.
    if(buscar_no(chave, pag_at, &pos)) {
        // Se for num nó folha, podemos só remover
        if(pag_at->tipoNo == FOLHA) {

            // Se não puder remover, significa que ele não satisfaz as condições (só o código estação está correto, mas o resto não).
            if(!remover_chave_no(pag_at, pos, arquivo_dados, temp, pares_removidos)) {
                free(pag_at);
                return D_ERROR; 
            }

            // Atualiza a página
            fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
            escrever_no(arquivo_ar, pag_at);

            if(pag_at->nroChaves < MIN_CHAVES) { // Tem menos que o mínimo de chaves necessária para a remoção (underflow)
                free(pag_at);
                return D_UNDERFLOW;
            } else {
                free(pag_at);
                return D_NORMAL;
            }
        } else {
            // Estamos num nó intermediário. Vamos substituí-lo pelo seu sucessor e dps remover o sucessor.
            int offset_dados = pag_at->PR[pos];
            fseek(arquivo_dados, offset_dados+1, SEEK_SET);
            estacao possivel_estacao;
            binario_para_estacao_apontado(&possivel_estacao, arquivo_dados);

            // Verifica os critérios de busca
            if(temp != NULL && !satisfazBase(temp, &possivel_estacao)) {
                liberar_memoria_estacao(&possivel_estacao);
                free(pag_at);
                return D_ERROR;
            }

            // Estação é válida, podemos remover do arquivo de dados
            if(temp != NULL) {
                if (possivel_estacao.codEstacao != -1 && possivel_estacao.codProxEstacao != -1) (*pares_removidos)++;
                int topo_dados;
                fseek(arquivo_dados, 1, SEEK_SET);
                fread(&topo_dados, sizeof(int), 1, arquivo_dados);
                int rrn_removido = (offset_dados - 17) / 80;
                char removido = '1';
                fseek(arquivo_dados, offset_dados, SEEK_SET);
                fwrite(&removido, sizeof(char), 1, arquivo_dados);
                fwrite(&topo_dados, sizeof(int), 1, arquivo_dados);
                fseek(arquivo_dados, 1, SEEK_SET);
                fwrite(&rrn_removido, sizeof(int), 1, arquivo_dados);
            }

            liberar_memoria_estacao(&possivel_estacao);
            
            // Busca a chave sucessora (vai pra direita e dps desce td pra esquerda)
            int chave_suc, pr_suc;
            encontrar_sucessor(arquivo_ar, pag_at->P[pos+1], &chave_suc, &pr_suc);

            // Substitui o nó atual pelo sucessor
            pag_at->C[pos] = chave_suc;
            pag_at->PR[pos] = pr_suc;
            fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
            escrever_no(arquivo_ar, pag_at);

            // Pede para remover o sucessor agora. Note que estacao* temp = NULL, isso faz com que ele não entre nos IFs para ser removido do arquivo de dados.
            RetornoDelecao RETORNO = deletar_recursivo(arquivo_ar, arquivo_dados, pag_at->P[pos+1], chave_suc, cabecalho, NULL, pares_removidos);

            // Trata um possível underflow gerado na subárvore à direita
            if(RETORNO == D_UNDERFLOW) {
                RETORNO = tratar_underflow(arquivo_ar, pag_at, pos + 1, RRN, cabecalho);
            }
            free(pag_at);
            return RETORNO;
        }
    } else {
        // A chave não está no nó atual, então continuamos descendo.
        RetornoDelecao RETORNO = deletar_recursivo(arquivo_ar, arquivo_dados, pag_at->P[pos], chave, cabecalho, temp, pares_removidos);

        // Backtracking: Se der underflow nos filhos precisamos tratar.
        if(RETORNO == D_UNDERFLOW) {
            RETORNO = tratar_underflow(arquivo_ar, pag_at, pos, RRN, cabecalho);
        }

        free(pag_at);
        pag_at = NULL;
        return RETORNO;
    }   
}

// Busca recursivamente a chave sucessora imediata de uma chave localizada num nó interno.
// (menor chave contida na subárvore à direita da chave original, ou seja, vai para o filho da direita e depois segue ao máximo para esquerda)
void encontrar_sucessor(FILE *arquivo_ar, int RRN, int *chave_sucessora, int *pr_sucessor) {
    if(RRN == -1) { // Já passou da folha, então só retorna sem atualizar nada.
        return;
    }
    fseek(arquivo_ar, get_offset(RRN), SEEK_SET);
    NoArvore* sucessor = ler_no(arquivo_ar);

    if(sucessor->tipoNo == FOLHA) { // Chegamos na folha, entao atualizamos os ponteiros
        *chave_sucessora = sucessor->C[0];
        *pr_sucessor = sucessor->PR[0];
    }

    // Continua descendo na árvore
    encontrar_sucessor(arquivo_ar, sucessor->P[0], chave_sucessora, pr_sucessor);
    free(sucessor);
    sucessor = NULL;
    return;
}

// Trata o underflow de um nó filho, tentando rotacionar ou concatenar, seguindo a ordem especificada.
RetornoDelecao tratar_underflow(FILE *arquivo_ar, NoArvore *pai, int idx_filho, int RRN_pai, CabecalhoArvore* cabecalho) {
    fseek(arquivo_ar, get_offset(pai->P[idx_filho]), SEEK_SET);
    NoArvore* filho = ler_no(arquivo_ar);

    // Índice dos irmãos adjacentes
    int id_esq = idx_filho-1, id_dir = idx_filho+1;
    NoArvore *irmao_esq = NULL, *irmao_dir = NULL;

    // Existe o irmão da direita
    if(idx_filho < pai->nroChaves) {
        fseek(arquivo_ar, get_offset(pai->P[id_dir]), SEEK_SET);
        irmao_dir = ler_no(arquivo_ar);
        
        // --- Rotaciona para esquerda se tiver chave suficiente ---
        if(irmao_dir->nroChaves > MIN_CHAVES) {
            rotaciona_esquerda(filho, pai, irmao_dir, idx_filho);
            
            // Atualizando os arquivos
            fseek(arquivo_ar, get_offset(RRN_pai), SEEK_SET);
            escrever_no(arquivo_ar, pai);            
            fseek(arquivo_ar, get_offset(pai->P[idx_filho]), SEEK_SET);
            escrever_no(arquivo_ar, filho);
            fseek(arquivo_ar, get_offset(pai->P[id_dir]), SEEK_SET);
            escrever_no(arquivo_ar, irmao_dir);

            if(filho != NULL) free(filho);
            if(irmao_esq != NULL) free(irmao_esq);
            if(irmao_dir != NULL) free(irmao_dir);
            filho = irmao_esq = irmao_dir = NULL;
            return D_NORMAL; // Deu certo
        }
    }

    // Existe o irmão da esquerda
    if(idx_filho > 0) {
        fseek(arquivo_ar, get_offset(pai->P[id_esq]), SEEK_SET);
        irmao_esq = ler_no(arquivo_ar);

        // --- Rotaciona para direita se tiver chave suficiente ---
        if(irmao_esq->nroChaves > MIN_CHAVES) {
            rotaciona_direita(filho, pai, irmao_esq, id_esq);

            // Atualizando os arquivos
            fseek(arquivo_ar, get_offset(RRN_pai), SEEK_SET);
            escrever_no(arquivo_ar, pai);
            fseek(arquivo_ar, get_offset(pai->P[idx_filho]), SEEK_SET);
            escrever_no(arquivo_ar, filho);
            fseek(arquivo_ar, get_offset(pai->P[id_esq]), SEEK_SET);
            escrever_no(arquivo_ar, irmao_esq);

            if(filho != NULL) free(filho);
            if(irmao_esq != NULL) free(irmao_esq);
            if(irmao_dir != NULL) free(irmao_dir);
            filho = irmao_esq = irmao_dir = NULL;
            return D_NORMAL; // Deu certo
        }
    }

    // --- Concatenação ---
    // Note que, ao chegar até aqui, um dos irmãos tem exatamente uma chave.

    // Tem irmão para esquerda
    if(idx_filho > 0) {
        int rrn_filho_removido = pai->P[idx_filho];
        int rrn_irmao_esq = pai->P[id_esq];

        // Irmão da esquerda tem 1, joga filho e pai para irmao esquerda
        concatena_esquerda(filho, pai, irmao_esq, id_esq, cabecalho);

        // Atualiza os arquivos e cabeçalho
        fseek(arquivo_ar, get_offset(RRN_pai), SEEK_SET);
        escrever_no(arquivo_ar, pai);
        fseek(arquivo_ar, get_offset(rrn_irmao_esq), SEEK_SET);
        escrever_no(arquivo_ar, irmao_esq);
        fseek(arquivo_ar, get_offset(rrn_filho_removido), SEEK_SET);
        escrever_no(arquivo_ar, filho);

        cabecalho->topo = rrn_filho_removido;
        cabecalho->nroNos--;
        fseek(arquivo_ar, 0, SEEK_SET);
        escrever_cabecalho(arquivo_ar, cabecalho);

        if(filho != NULL) free(filho);
        if(irmao_esq != NULL) free(irmao_esq);
        if(irmao_dir != NULL) free(irmao_dir);
        filho = irmao_esq = irmao_dir = NULL;

        // Se der underflow no pai retornamos D_UNDERFLOW
        if(pai->nroChaves < MIN_CHAVES) return D_UNDERFLOW;
        else return D_NORMAL;
    } else {

        // Se não tem irmão para esquerda, ele obrigatoriamente tem irmão para direita, pela estrutura da árvore.
        int rrn_filho = pai->P[idx_filho];
        int rrn_irmao_dir_removido = pai->P[id_dir];

        // Junta o pai e irmão da direita para o nó filho.
        concatena_direita(filho, pai, irmao_dir, idx_filho, cabecalho);

        // Atualiza os arquivos e cabeçalho
        fseek(arquivo_ar, get_offset(RRN_pai), SEEK_SET);
        escrever_no(arquivo_ar, pai);
        fseek(arquivo_ar, get_offset(rrn_filho), SEEK_SET);
        escrever_no(arquivo_ar, filho);
        fseek(arquivo_ar, get_offset(rrn_irmao_dir_removido), SEEK_SET);
        escrever_no(arquivo_ar, irmao_dir);

        cabecalho->topo = rrn_irmao_dir_removido;
        cabecalho->nroNos--;
        fseek(arquivo_ar, 0, SEEK_SET);
        escrever_cabecalho(arquivo_ar, cabecalho);
        
        if(filho != NULL) free(filho);
        if(irmao_esq != NULL) free(irmao_esq);
        if(irmao_dir != NULL) free(irmao_dir);
        filho = irmao_esq = irmao_dir = NULL;

        // Se der underflow no pai retornamos D_UNDERFLOW
        if(pai->nroChaves < MIN_CHAVES) return D_UNDERFLOW;
        else return D_NORMAL;
    }

    if(filho != NULL) free(filho);
    if(irmao_esq != NULL) free(irmao_esq);
    if(irmao_dir != NULL) free(irmao_dir);
    filho = irmao_esq = irmao_dir = NULL;
}

// Realiza a redistribuição de chaves (rotação à direita) pegando emprestado do irmão à esquerda
void rotaciona_direita(NoArvore* filho, NoArvore* pai, NoArvore* irmao_esq, int id_esq) {
    // Divide a qtd de nós na metade
    int total_chaves = filho->nroChaves + 1 + irmao_esq->nroChaves;
    int chaves_para_esq = total_chaves / 2;
    
    // Vai jogando os nós até o da esquerda ficar com qtd certa
    while(irmao_esq->nroChaves > chaves_para_esq) {
        for(int i = filho->nroChaves; i > 0; i--) {
            filho->C[i] = filho->C[i-1];
            filho->PR[i] = filho->PR[i-1];
            filho->P[i+1] = filho->P[i];
        }
        filho->P[1] = filho->P[0];

        filho->C[0] = pai->C[id_esq];
        filho->PR[0] = pai->PR[id_esq];
        filho->P[0] = irmao_esq->P[irmao_esq->nroChaves];
        filho->nroChaves++;

        pai->C[id_esq] = irmao_esq->C[irmao_esq->nroChaves - 1];
        pai->PR[id_esq] = irmao_esq->PR[irmao_esq->nroChaves - 1];

        irmao_esq->C[irmao_esq->nroChaves - 1] = -1;
        irmao_esq->PR[irmao_esq->nroChaves - 1] = -1;
        irmao_esq->P[irmao_esq->nroChaves] = -1;
        irmao_esq->nroChaves--;
    }
}

// Análogo a rotação direita, exceto na part de deslocamento
void rotaciona_esquerda(NoArvore* filho, NoArvore* pai, NoArvore* irmao_dir, int idx_filho) {
    int total_chaves = filho->nroChaves + 1 + irmao_dir->nroChaves;
    int chaves_para_filho = total_chaves / 2; 

    while (filho->nroChaves < chaves_para_filho) {
        filho->C[filho->nroChaves] = pai->C[idx_filho];
        filho->PR[filho->nroChaves] = pai->PR[idx_filho];
        filho->P[filho->nroChaves + 1] = irmao_dir->P[0];
        filho->nroChaves++;

        pai->C[idx_filho] = irmao_dir->C[0];
        pai->PR[idx_filho] = irmao_dir->PR[0];

        // Desloca tudo
        int n_dir = irmao_dir->nroChaves;
        for(int i = 0; i < n_dir - 1; i++) {
            irmao_dir->C[i] = irmao_dir->C[i+1];
            irmao_dir->PR[i] = irmao_dir->PR[i+1];
            irmao_dir->P[i] = irmao_dir->P[i+1];
        }

        // Tira o último caso repetido
        irmao_dir->P[n_dir - 1] = irmao_dir->P[n_dir]; 
        irmao_dir->C[n_dir - 1] = -1;
        irmao_dir->PR[n_dir - 1] = -1;
        irmao_dir->P[n_dir] = -1;
        irmao_dir->nroChaves--;
    }
}

// Realiza a concatenação de um nó filho com seu irmão adjacente à esquerda.
void concatena_esquerda(NoArvore* filho, NoArvore* pai, NoArvore* irmao_esq, int id_esq, CabecalhoArvore* cabecalho) {

    // Chave do pai vai para posição 1 do irmao (que tem exatamente uma chave, pois chegou na concatenação)
    irmao_esq->C[1] = pai->C[id_esq];
    irmao_esq->PR[1] = pai->PR[id_esq];

    irmao_esq->P[2] = filho->P[0];
    irmao_esq->nroChaves++;

    // Remove do pai fazendo deslocamento
    for(int i = id_esq; i < pai->nroChaves-1; i++) {
        pai->C[i] = pai->C[i+1];
        pai->PR[i] = pai->PR[i+1];
        pai->P[i+1] = pai->P[i+2];
    }
    pai->nroChaves--;
    pai->C[pai->nroChaves] = -1;
    pai->PR[pai->nroChaves] = -1;
    pai->P[pai->nroChaves+1] = -1;

    // Deleta o filho e atualiza a pilha de remivido
    filho->removido = '1';
    filho->proximo = cabecalho->topo;
}

// Análogo a concatena_esquerda.
void concatena_direita(NoArvore* filho, NoArvore* pai, NoArvore* irmao_dir, int idx_filho, CabecalhoArvore* cabecalho) {
    filho->C[0] = pai->C[idx_filho];
    filho->PR[0] = pai->PR[idx_filho];

    filho->C[1] = irmao_dir->C[0];
    filho->PR[1] = irmao_dir->PR[0];

    filho->P[1] = irmao_dir->P[0];
    filho->P[2] = irmao_dir->P[1];

    filho->nroChaves = 2;

    for(int i = idx_filho; i < pai->nroChaves-1; i++) {
        pai->C[i] = pai->C[i+1];
        pai->PR[i] = pai->PR[i+1];
        pai->P[i+1] = pai->P[i+2];
    }

    pai->nroChaves--;
    pai->C[pai->nroChaves] = -1;
    pai->PR[pai->nroChaves] = -1;
    pai->P[pai->nroChaves+1] = -1;

    irmao_dir->removido = '1';
    irmao_dir->proximo = cabecalho->topo;
}