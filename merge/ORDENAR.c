#include "ORDENAR.h"
#include "../TAD/util.h"
#include "../TAD/fornecidas.h"


// Compara pelo codEstacao. Começa usando void pq tem q ser genérico

// Retorna < 0 se chaveA < chaveB, 0 se for igual e > 0 se chaveA > chaveB
int compara_codEstacao(const void *a, const void *b) {
    // Transformação de tipos
    estacao *estA = (estacao *)a;
    estacao *estB = (estacao *)b;
    int chaveA = estA->codEstacao;
    int chaveB = estB->codEstacao;

    // Se alguma chave nao for valida, deixamos o outro na frente
    if (chaveA == -1 && chaveB == -1) return 0;
    if (chaveA == -1) return 1;
    if (chaveB == -1) return -1;

    // Retorna < 0 se chaveA < chaveB, 0 se for igual e > 0 se chaveA > chaveB
    return chaveA - chaveB;
}

// Compara pelo codProxEstacao. Começa usando void pq tem q ser genérico

// Retorna < 0 se chaveA < chaveB, 0 se for igual e > 0 se chaveA > chaveB
int compara_codProxEstacao(const void *a, const void *b) {
    estacao *estA = (estacao *)a;
    estacao *estB = (estacao *)b;
    int chaveA = estA->codProxEstacao;
    int chaveB = estB->codProxEstacao;

    // Se alguma chave nao for valida, deixamos o outro na frente
    if (chaveA == -1 && chaveB == -1) return 0;
    if (chaveA == -1) return 1;
    if (chaveB == -1) return -1;

    // Retorna < 0 se chaveA < chaveB, 0 se for igual e > 0 se chaveA > chaveB
    return chaveA - chaveB;
}

// Le todos as estacoes validas, traz para RAM e faz a ordenação usando o qsort do C

// FILE* arquivo_en: Arquivo de entrada de onde deve sair o sort
// bool qual: 0 se for ordenar pelo codEstacao, e 1 se for pelo codProxEstacao
// int* ultimo: Ao final retorna a quantidade de itens no vetor
// Retorno: Retorna o ponteiro para a lista de estações ordenadas
estacao *ordem(FILE *arquivo_en, bool qual, int* ultimo){
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

    // Faz o sort usando o qsort do stlib.h
    if (qual == 0) {
        qsort(lista, topo, sizeof(estacao), compara_codEstacao);
    } else {
        qsort(lista, topo, sizeof(estacao), compara_codProxEstacao);
    }
    *ultimo = topo;
    return lista;
}

// Recebe o arquivo para ordenar e manda para outro arquivo

// char* bin1: Arquivo para ordenar
// char* bin2: Arquivo que recebe a lista ordenada
// char* Stipo: Qual campo deve receber a ordenação(codEstacao ou codProxEstacao)
void ordenacao(char *bin1, char *bin2, char* Stipo) {
    bool tipo;

    // Verifica qual ordenação tem q fazer
    if(strcmp(Stipo,"codEstacao") == 0) tipo = false;
    else if(strcmp(Stipo,"codProxEstacao") == 0) tipo = true;
    else{
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *arquivo1 = abrir_arquivo_validar(bin1, "rb");
    if(arquivo1 == NULL) return;

    // Chama a função de ordenar.
    int topo;
    estacao* lista = ordem(arquivo1,tipo,&topo);

    FILE *arquivo2 = fopen(bin2, "wb");

    fwrite(&c_zero,sizeof(char),1,arquivo2); // Demarcar o arquivo como instável no começo

    for(int i = 0; i < 16; i++) {
        // Inicializar o cabeçalho todo com zero para podermos colocar as estações corretamente
        fwrite(&zero, 1, 1, arquivo2); 
    }

    // Conta quantos pares de estação tem.
    int qtdpares = 0;
    for(int i = 0; i < topo; i++){
        if(lista[i].codProxEstacao != -1) qtdpares++;
        lista[i].removido = '0';
        estacao_para_binario_apontado(&(lista[i]),arquivo2);
        liberar_memoria_estacao(&(lista[i]));
    }

    free(lista);

    // Salvando os dados nos arquivos
    fseek(arquivo2,1,SEEK_SET);
    int menosum = -1;
    fwrite(&menosum,sizeof(int),1,arquivo2);
    fwrite(&topo,sizeof(int),1,arquivo2);
    
    int qtdest,qtdparesest;
    fseek(arquivo1,9,SEEK_SET);
    fread(&qtdest, sizeof(int), 1, arquivo1);
    fread(&qtdparesest, sizeof(int), 1, arquivo1);
    fwrite(&qtdest,sizeof(int),1,arquivo2);
    fwrite(&qtdparesest,sizeof(int),1,arquivo2);

    //marcar como estável
    fseek(arquivo2,0,SEEK_SET);
    fwrite(&c_um,sizeof(char),1,arquivo2);
    fclose(arquivo2);

    fclose(arquivo1);
    BinarioNaTela(bin2);

}