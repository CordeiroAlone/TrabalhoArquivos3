#include "../TAD/util.h"
#include "../arvoreB/BUSCAR.h"
#include "SELECTUNICO.h"

// Faz a auto junção de loop único, onde percorre o primeiro arquivo de forma sequencial e o segundo usando o indice do arvore B

// char *bin1: Nome do arquivo lido no loop externo
// char *bin2: Nome do arquivo onde tem os dados a ser usado (não percorremos esse arquivo sequencialmente)
// char *bin3: Nome do arquivo da árvore-b
void buscaUnico(char *bin1, char *bin2, char *bin3){
    // Abre os arquivos e valida
    FILE *arquivo1 = abrir_arquivo_validar(bin1, "rb");
    if (arquivo1 == NULL) return;

    FILE *arquivo2 = abrir_arquivo_validar(bin2, "rb");
    if (arquivo2 == NULL) {
        fclose(arquivo1);
        return;
    }

    FILE *arquivo_ar = abrir_arquivo_validar(bin3, "rb");
    if (arquivo_ar == NULL) {
        fclose(arquivo1);
        fclose(arquivo2);
        return;
    }

    int tam1,raiz;
    fseek(arquivo1,5,SEEK_SET);
    fread(&tam1, sizeof(int), 1, arquivo1);
    fread(&raiz, sizeof(int), 1, arquivo_ar);
    bool deubom = false;

    fseek(arquivo1, 17,SEEK_SET);

    // Loop que passa pelo primeiro arquivo sequencialmente
    for(int i = 0; i < tam1; i++){
        char delet;
        fread(&delet, sizeof(char),1,arquivo1);
        if(delet == '1'){
            if(i != tam1 - 1) fseek(arquivo1, 17 + (80 * (i+1)),SEEK_SET);
            continue;
        } 
        
        estacao est;
        binario_para_estacao_apontado(&est,arquivo1);

        bool achou = false;
        char *nome;
        
        int rrnencontrada,posicaoencontrada;
        // A partir da chave de busca, procuramos na arvoreB usando a função já implementada
        achou = busca_arvoreB(arquivo_ar, raiz, est.codProxEstacao, &rrnencontrada, &posicaoencontrada);

        if(achou){
            // Como foi encontrado, basta imprimir os campos da estação.
            fseek(arquivo_ar,17 + 53 * (rrnencontrada) + 17 + (8 * posicaoencontrada),SEEK_SET);
            int index;
            fread(&index,sizeof(int),1,arquivo_ar);

            pegaNomeEst(&nome, arquivo2, (index-17)/80);

            printf("%d %s %s %d %s\n",est.codEstacao, est.nomeEstacao, est.nomeLinha, est.codProxEstacao, nome);
            deubom = true;
            free(nome);
        }
        
        liberar_memoria_estacao(&est);
    }

    // Não foi encontrado o registro
    if(!deubom){
        printf("Registro inexistente.\n");
    }
    fclose(arquivo1);
    fclose(arquivo2);
    fclose(arquivo_ar);
}