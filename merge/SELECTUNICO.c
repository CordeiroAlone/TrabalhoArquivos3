#include "../TAD/util.h"
#include "../arvoreB/BUSCAR.h"
#include "SELECTUNICO.h"

void buscaUnico(char *bin1, char *bin2, char *bin3){
    FILE *arquivo1 = fopen(bin1, "rb");
    FILE *arquivo2 = fopen(bin2, "rb");
    FILE *arquivo_ar = fopen(bin3, "rb");
   
    if (arquivo1 == NULL || arquivo2 == NULL || arquivo_ar == NULL) {
        if(arquivo1 != NULL) fclose(arquivo1);
        if(arquivo2 != NULL) fclose(arquivo2);
        if(arquivo_ar != NULL) fclose(arquivo_ar);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char status1,status2,status3;
    fread(&status1, sizeof(char), 1, arquivo1);
    fread(&status2, sizeof(char), 1, arquivo2);
    fread(&status3, sizeof(char), 1, arquivo_ar);
    if(status1 == '0' || status2 == '0' || status3 == '0') {
        fclose(arquivo1);
        fclose(arquivo2);
        fclose(arquivo_ar);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int tam1,raiz;
    fseek(arquivo1,5,SEEK_SET);
    fread(&tam1, sizeof(int), 1, arquivo1);
    fread(&raiz, sizeof(int), 1, arquivo_ar);
    bool deubom = false;

    fseek(arquivo1, 17,SEEK_SET);

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
        achou = busca_arvoreB(arquivo_ar, raiz, est.codProxEstacao, &rrnencontrada, &posicaoencontrada);

        if(achou){
            //printf("%d\n",posicaoencontrada);
            fseek(arquivo_ar,17 + 53 * (rrnencontrada) + 17 + (8 * posicaoencontrada),SEEK_SET);
            int index;
            fread(&index,sizeof(int),1,arquivo_ar);

            //fseek(arquivo2,17 + (80 * index),SEEK_SET);
            pegaNomeEst(&nome, arquivo2, (index-17)/80);

            printf("%d %s %s %d %s\n",est.codEstacao, est.nomeEstacao, est.nomeLinha, est.codProxEstacao, nome);
            deubom = true;
            free(nome);
        }
        
        liberar_memoria_estacao(&est);
    }

    if(!deubom){
        printf("Registro inexistente.\n");
    }
    fclose(arquivo1);
    fclose(arquivo2);
    fclose(arquivo_ar);
}