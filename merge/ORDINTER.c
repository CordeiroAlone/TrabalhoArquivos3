#include "ORDENAR.h"
#include "../TAD/util.h"
#include "../TAD/fornecidas.h"

// Recebe dois arquivos desordenados, ordena usando codEstacao e codProxEstacao e faz a junção
// char *bin1: Arquivo a ser ordenado usando codProxEstacao
// char *bin2: Arquivo a ser ordenado usando codEstacao
void ordintercala(char *bin1, char *bin2){
    FILE *arquivo1 = abrir_arquivo_validar(bin1, "rb");
    if(arquivo1 == NULL) return;

    FILE *arquivo2 = abrir_arquivo_validar(bin2, "rb");
    if(arquivo2 == NULL){
        fclose(arquivo1);
        return;
    }

    int tam1,tam2;
    // lista1 vai receber a lista ordenada do arquivo 1 e lista2 recebe o ordenado do arquivo2
    estacao *lista1 = ordem(arquivo1,true,&tam1);
    estacao *lista2 = ordem(arquivo2,false,&tam2);

    bool encontrado = false;
    int id1 = 0,id2 = 0;

    // Durante esse while loop, se a chave for igual, faz a junção dos dois, se o chave da esqwuerda for maior, avança o indice da direita e vice-versa
    while(id1 < tam1 && id2 < tam2){
        if(lista1[id1].codProxEstacao == lista2[id2].codEstacao){
            encontrado = true;
            printf("%d %s %s %d %s\n",lista1[id1].codEstacao, lista1[id1].nomeEstacao, lista1[id1].nomeLinha, lista1[id1].codProxEstacao, lista2[id2].nomeEstacao);
            id1++;
            id2++; //considerando que é unico o proximo estação
        }else if(lista1[id1].codProxEstacao > lista2[id2].codEstacao) id2++;
        else id1++;
    }

    if(!encontrado){
        printf("Registro inexistente.\n");
    }

    // Liberando memória e fechando os arquivos
    for(int i = 0; i < tam1; i++) liberar_memoria_estacao(&(lista1[i]));
    for(int i = 0; i < tam2; i++) liberar_memoria_estacao(&(lista2[i]));
    free(lista1);
    free(lista2);
    fclose(arquivo1);
    fclose(arquivo2);
}