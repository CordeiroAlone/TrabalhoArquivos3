#include "DELETE.h"
#include "../TAD/buscar.h"
#include "../TAD/fornecidas.h"


// Realiza a deleção de registros de estações e seus índices associados.
// Essa função abre os arquivos de dados e de índice, valida o status de consistência de ambos, e realiza deleção várias vezes.
void DELETE_AR(char *arquivoBIN, char *arquivoBIN_ar, int vezes) {

    //Abertura dos arquivos e checando a consistência
    FILE *dados_arq = abrir_arquivo_validar(arquivoBIN, "rb+");
    if (dados_arq == NULL) return;

    FILE *indice_arq = abrir_arquivo_validar(arquivoBIN_ar, "rb+");
    if (indice_arq == NULL) {
        fclose(dados_arq);
        return;
    }

    //Marcando os arquivos como incosistentes
    char status_instavel = '0';    
    fseek(dados_arq, 0, SEEK_SET);
    fwrite(&status_instavel, sizeof(char), 1, dados_arq);
    fseek(indice_arq, 0, SEEK_SET);
    fwrite(&status_instavel, sizeof(char), 1, indice_arq);
    fseek(indice_arq, 0, SEEK_SET);

    // Lendo cabeçalho 
    CabecalhoArvore* cabecalho_ar = ler_cabecalho(indice_arq);

    // Marca quantidade de pares de estações removidos para atualizar depois
    int totalParesRemovidos = 0;

    // Para cada vez que for preciso, fazemos a deleção de acordo com o pedido pela entrada.
    for (int i = 0; i < vezes; i++) {
        int m;
        scanf("%d", &m);
        estacao temp;
        nulifica_estacao(&temp); 

        // Le as informações necessarias da estacao para que possa ser deletado e salva no temp
        pegar_info_estacao(&temp, m); 

        // Se o codigo estacao for -1, nao podemos fazer a deleção pela árvore, então fazemos usando a função implementada análoga ao trabalho introdutório.
        if (temp.codEstacao == -1) {
            int paresTirados = 0;            
            paresTirados = deletar_sequencial(&temp, dados_arq, indice_arq, cabecalho_ar);
            totalParesRemovidos += paresTirados; // Atualiza quantos pares foram removidos
        } else {
            // Realizando a deleção pela árvore. Função explicada no DELETE_ONE.c
            int raiz_atual = cabecalho_ar->noRaiz;
            int pares_removidos_agora = 0;
            RetornoDelecao ret = deletar_recursivo(indice_arq, dados_arq, raiz_atual, temp.codEstacao, cabecalho_ar, &temp, &pares_removidos_agora);

            // Se a raiz for vazia, precisamos trocá-la
            atualizar_raiz_se_vazia(indice_arq, cabecalho_ar);
            
            // D_ERROR acontece quando a estação não é encontrada no arquivo, ou seja, todos já foram deletados
            if (ret != D_ERROR) {
                totalParesRemovidos += pares_removidos_agora;
            }
        }

        // Liberando memória
        liberar_memoria_estacao(&temp);
    }
    
    // Checagem final para ter certeza que a raiz não fique vazia
    atualizar_raiz_se_vazia(indice_arq, cabecalho_ar);

    // Atualizando os arquivos no final da operação.
    int qtdParEstacao;
    fseek(dados_arq, 13, SEEK_SET);
    fread(&qtdParEstacao, sizeof(int), 1, dados_arq); // Quantos pares estacoes tinha antes da remocao
    qtdParEstacao -= totalParesRemovidos;
    int qtdEstacao = conta_estacao(dados_arq);
    fseek(dados_arq, 9, SEEK_SET);
    fwrite(&qtdEstacao, sizeof(int), 1, dados_arq);
    fwrite(&qtdParEstacao, sizeof(int), 1, dados_arq);
    fseek(indice_arq, 0, SEEK_SET);
    escrever_cabecalho(indice_arq, cabecalho_ar); // Atualizando o cabeçalho final aqui

    // Ao final, marcamos os arquivos como estável e finalizamos fechando os arquivos e mostrando o binario na tela.
    fseek(dados_arq, 0, SEEK_SET);
    marcar_estavel(dados_arq);
    fseek(indice_arq, 0, SEEK_SET);
    marcar_estavel(indice_arq);

    free(cabecalho_ar);
    fclose(dados_arq);
    fclose(indice_arq);

    BinarioNaTela(arquivoBIN);
    BinarioNaTela(arquivoBIN_ar);
}

// Verifica e atualiza a raiz da árvore-B caso ela fique sem chaves após deleções.
// Se a raiz atual ficar vazia (0 chaves) devido a operações de concatenação, a função a marca como removida (adicionando-a à pilha de nós livres no topo do cabeçalho) e promove seu primeiro filho (se existir) como a nova raiz da árvore.
void atualizar_raiz_se_vazia(FILE* indice_arq, CabecalhoArvore* cabecalho_ar) {
    int RRN_raiz_antiga = cabecalho_ar->noRaiz;

    // Precisamos atualizar continuamente até que a raiz não seja vazia
    while(RRN_raiz_antiga != -1) {
        fseek(indice_arq, get_offset(RRN_raiz_antiga), SEEK_SET);
        NoArvore* raiz_aux = ler_no(indice_arq);
        
        // Se a raiz perdeu todas as suas chaves
        if (raiz_aux->nroChaves == 0) {
            int nova_raiz_RRN = -1;
            if (raiz_aux->tipoNo != FOLHA) {
                nova_raiz_RRN = raiz_aux->P[0];
            }

            // Marca a raiz antiga como removida, atualiza a pilha de removidos e o resto do cabeçalho.
            raiz_aux->removido = '1';
            raiz_aux->proximo = cabecalho_ar->topo;
            cabecalho_ar->topo = RRN_raiz_antiga;
            cabecalho_ar->noRaiz = nova_raiz_RRN;
            cabecalho_ar->nroNos--;

            // Sobreescreve a raiz antiga com a nova e atualiza o cabeçalho
            fseek(indice_arq, get_offset(RRN_raiz_antiga), SEEK_SET);
            escrever_no(indice_arq, raiz_aux);
            fseek(indice_arq, 0, SEEK_SET);
            escrever_cabecalho(indice_arq, cabecalho_ar);
            
            // Atualiza o tipo da nova raiz no disco, se ela existir
            if (nova_raiz_RRN != -1) {
                fseek(indice_arq, get_offset(nova_raiz_RRN), SEEK_SET);

                NoArvore* nova_raiz = ler_no(indice_arq);
                if (nova_raiz->tipoNo != FOLHA) {
                    nova_raiz->tipoNo = RAIZ;
                } else nova_raiz->tipoNo = FOLHA; // Raiz-folha é considerado folha

                fseek(indice_arq, get_offset(nova_raiz_RRN), SEEK_SET);
                escrever_no(indice_arq, nova_raiz);
                free(nova_raiz);
            }
            free(raiz_aux);

            // Verifica novamente caso a nova raiz também tenha ficado vazia e volta pro while
            RRN_raiz_antiga = cabecalho_ar->noRaiz;
        } else {
            free(raiz_aux);
            break;
        }
    }
}


 // Itera sequencialmente pelo arquivo de dados e deleta os registros que satisfazem as condições de busca.
 // Para cada registro compatível encontrado, a função aciona a remoção de sua respectiva chave na Árvore-B e atualiza a estrutura do índice.
 // Essa função é análoga a função iterar_estacao feita no trabalho introdutório, com a diferença de servir somente para deleção e deletar na árvore após conseguir o código da estação.
int deletar_sequencial(estacao* base, FILE* arquivo, FILE* arquivo_ar, CabecalhoArvore* cabecalho_ar) {
    // Lê o topo da pilha de registros removidos do arquivo de dados
    int topo;
    fseek(arquivo, 1, SEEK_SET);
    fread(&topo,sizeof(int),1,arquivo);

    int cont = 0; // Quantidades de estações que satisfazem a base
    int parestirados = 0;
    
    // Lë a quantidade de registros no arquivo
    int tam;
    fseek(arquivo, 5, SEEK_SET);
    fread(&tam,sizeof(int),1,arquivo);

    fseek(arquivo, 17, SEEK_SET); // Coloca o ponteiro no final do cabeçalho
    for(int i = 0; i < tam; i++) { // Iterando sobre cada registro

        // Colocando o registro atual em 'temp'
        estacao temp;
        fread(&temp.removido,sizeof(char),1,arquivo);
        if(temp.removido == '1'){
            if(i != tam-1) fseek(arquivo,17 + ((i+1) * 80),SEEK_SET); 
            continue;
        } 

        binario_para_estacao_apontado(&temp,arquivo);
        
        // Se satisfaz base (mesma função do trabalho introdutorio, presente em ../TAD/buscar.c), podemos deletar na árvore.
        if(satisfazBase(base, &temp)) { 
            cont++; // Encontramos mais uma estação a ser removido
            
            int raiz_atual = cabecalho_ar->noRaiz;
            int p = 0;
            deletar_recursivo(arquivo_ar, arquivo, raiz_atual, temp.codEstacao, cabecalho_ar, base, &p); // Removendo da árvore
            parestirados += p; 

            atualizar_raiz_se_vazia(arquivo_ar, cabecalho_ar);
            
            fseek(arquivo,17 + ((i+1) * 80),SEEK_SET);
        }

        // Desalocando memória
        liberar_memoria_estacao(&temp);
    }

    return parestirados; 
}