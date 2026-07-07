#include "../TAD/util.h"
#include "SELECTDUPLO.h" //implementar o negocio do loop com menos fseek

// Faz a auto-junção usando o loop aninhado, combinando quando o codProxEstacao do loop externo for igual ao codEstacao do loop interno.

// char *bin1: Nome do arquivo usado no loop de fora
// char *bin2: Nome do arquivo usado no loop de dentro
void buscaDuplo(char *bin1, char *bin2) {
    FILE *arquivo1 = abrir_arquivo_validar(bin1, "rb");
    if (arquivo1 == NULL) return;

    FILE *arquivo2 = abrir_arquivo_validar(bin2, "rb");
    if (arquivo2 == NULL) {
        fclose(arquivo1);
        return;
    }

    int tam1,tam2;
    fseek(arquivo1,5,SEEK_SET);
    fseek(arquivo2,5,SEEK_SET);
    fread(&tam1, sizeof(int), 1, arquivo1);
    fread(&tam2, sizeof(int), 1, arquivo2);
    bool deubom = false;

    fseek(arquivo1, 17,SEEK_SET);

    // loop de fora
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
        
        // loop de dentro
        for(int j = 0; j < tam2; j++){
            
            char delet2;
            fseek(arquivo2, 17 + (80 * j),SEEK_SET);
            fread(&delet2,sizeof(char),1,arquivo2);
            if(delet2 == '1') continue;

            int codest;
            fseek(arquivo2, 17 + (80 * j) + 5,SEEK_SET);
            fread(&codest,sizeof(int),1,arquivo2);
            
            if(codest != est.codProxEstacao) continue;

            achou = true;
            pegaNomeEst(&nome, arquivo2, j);
            break;
        }

        if(achou){
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
}