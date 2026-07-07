#include "ORDENAR.h"
#include "../TAD/util.h"
#include "../TAD/fornecidas.h" //lembrar de atualizar o cabeçalho no final

void ordem(FILE *arquivo_en, char *destino, bool qual){ //se qual for 0 é codestação, se for 1 é codproxestação
    FILE *arquivo_sa = fopen(destino, "wb");

    fwrite(&c_zero,sizeof(char),1,arquivo_sa); //Demarcar o arquivo como instável no começo

    for(int i = 0; i < 16; i++) {
        // Inicializar o cabeçalho todo com zero para podermos colocar as estações corretamente
        fwrite(&zero, 1, 1, arquivo_sa); 
    }

    fseek(arquivo_en,5,SEEK_SET);
    int tam;
    fread(&tam, sizeof(int), 1, arquivo_en);
    
    estacao *lista = malloc(tam *sizeof(estacao));
    int topo = 0;

    fseek(arquivo_en,17,SEEK_SET);
    for(int i = 0; i < tam; i++){
        char status;
        fread(&status, sizeof(char),1,arquivo_en);
        if(status == '1'){
            if(i != tam-1) fseek(arquivo_en,(i+1)*80 + 17,SEEK_SET);
            continue;
        } 

        binario_para_estacao_apontado(&(lista[topo]),arquivo_en);
        topo++;
    }
    //bubble sort (lembra de colocar as otimizações)
    for(int i = 0; i < topo-1; i++){
        for(int j = 0; j < topo-1-i; j++){
            int chave1 = lista[j].codEstacao, chave2 = lista[j+1].codEstacao;
            if(qual){
                chave1 = lista[j].codProxEstacao;
                chave2 = lista[j+1].codProxEstacao;
            }
            //Estamos fazendo o tratamento de nulos assim pois a função considera que não tem repetidos, ou seja, no máximo 1 nulo(além de não ser especificado a ordem dos nulos)
            if(chave1 == -1) chave1 = chave2 + 1; 
            if(chave2 == -1) chave2 = chave1 + 1;
            
            if(chave2 < chave1){
                estacao temp = lista[j+1];
                lista[j+1] = lista[j];
                lista[j] = temp;
            }
        }
    }

    int qtdpares = 0;
    for(int i = 0; i < topo; i++){
        if(lista[i].codProxEstacao != -1) qtdpares++;
        lista[i].removido = '0';
        estacao_para_binario_apontado(&(lista[i]),arquivo_sa);
        liberar_memoria_estacao(&(lista[i]));
    }

    free(lista);

    fseek(arquivo_sa,1,SEEK_SET);
    int menosum = -1;
    fwrite(&menosum,sizeof(int),1,arquivo_sa);
    fwrite(&topo,sizeof(int),1,arquivo_sa);
    
    int qtdest,qtdparesest;
    fseek(arquivo_en,9,SEEK_SET);
    fread(&qtdest, sizeof(int), 1, arquivo_en);
    fread(&qtdparesest, sizeof(int), 1, arquivo_en);
    fwrite(&qtdest,sizeof(int),1,arquivo_sa);
    fwrite(&qtdparesest,sizeof(int),1,arquivo_sa);
    
    //fwrite(&topo,sizeof(int),1,arquivo_sa); (por algum motivo não dá certo)?
    //fwrite(&qtdpares,sizeof(int),1,arquivo_sa);

    //marcar como estável
    fseek(arquivo_sa,0,SEEK_SET);
    fwrite(&c_um,sizeof(char),1,arquivo_sa);
    fclose(arquivo_sa);
}

void ordenacao(char *bin1, char *bin2, char* Stipo){
    bool tipo;
    if(strcmp(Stipo,"codEstacao") == 0) tipo = false;
    else if(strcmp(Stipo,"codProxEstacao") == 0) tipo = true;
    else{
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *arquivo1 = abrir_arquivo_validar(bin1, "rb");
    if(arquivo1 == NULL) return;

    ordem(arquivo1,bin2,tipo);


    fclose(arquivo1);
    BinarioNaTela(bin2);

}