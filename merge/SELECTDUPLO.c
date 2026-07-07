#include "../TAD/util.h"
#include "SELECTDUPLO.h"

void buscaDuplo(char *bin1, char *bin2){
    FILE *arquivo1 = fopen(bin1, "rb");
    FILE *arquivo2 = fopen(bin2, "rb");
   
    if (arquivo1 == NULL || arquivo2 == NULL) {
        if(arquivo1 != NULL) fclose(arquivo1);
        if(arquivo2 != NULL) fclose(arquivo2);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char status1,status2;
    fread(&status1, sizeof(char), 1, arquivo1);
    fread(&status2, sizeof(char), 1, arquivo2);
    if(status1 == '0' || status2 == '0') {
        fclose(arquivo1);
        fclose(arquivo2);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int tam1,tam2;
    fseek(arquivo1,5,SEEK_SET);
    fseek(arquivo2,5,SEEK_SET);
    fread(&tam1, sizeof(int), 1, arquivo1);
    fread(&tam2, sizeof(int), 1, arquivo2);

    for(int i = 0; i < tam1; i++){
        char delet;
        fseek(arquivo1, 17 + (80 * i),SEEK_SET);
        fread(&delet, sizeof(char),1,arquivo1);
        if(delet == '1') continue;
        
        estacao est;
        binario_para_estacao_apontado(&est,arquivo1);

        bool achou = false;
        char *nome;
        
        for(int j = 0; j < tam2; j++){
            //printf("cheguei no %d\n",j);
            char delet2;
            fseek(arquivo2, 17 + (80 * j),SEEK_SET);
            fread(&delet2,sizeof(char),1,arquivo2);
            if(delet2 == '1') continue;

            int codest;
            fseek(arquivo2, 17 + (80 * j) + 5,SEEK_SET);
            fread(&codest,sizeof(int),1,arquivo2);
            //printf("Encontrei o codigo %d\n",codest);
            if(codest != est.codProxEstacao) continue;

            achou = true;
            pegaNomeEst(&nome, arquivo2, j);
            break;
        }

        if(achou){
            printf("%d %s %s",est.codEstacao, est.nomeEstacao, est.nomeLinha);
            printf(" %d %s", est.codProxEstacao, nome);
            printf("\n");
            free(nome);
        }
        
        liberar_memoria_estacao(&est);
    }

    fclose(arquivo1);
    fclose(arquivo2);
}