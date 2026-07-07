#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "buscar.h"

// Imprime todos os registros não removidos do arquivo que satisfaçam as 'm' condições.

// arquivoBin: Nome do arquivo a ser lido.
// n: Quantidade de buscas a ser feitas
void SELECT(char *arquivoBIN, int n) {
    // Abrindo o arquivo
    FILE *arquivo = abrir_arquivo_validar(arquivoBIN, "rb");
    if(arquivo == NULL) return;

    // Iterando por cada busca
    for(int i = 0; i < n; i++){
        int m; // Quantidade de condições
        estacao temp;
        nulifica_estacao(&temp);

        scanf("%d",&m);
        pegar_info_estacao(&temp,m); // Pega quais condições a estação tem q satisfazer e deixa no temp. 
        // Caso um campo seja NULO, ele é representado no 'temp' por -2
        // Caso um campo não seja especificado (não é uma condição para buscar), ele é representado por -1
        
        iterar_estacao(&temp, arquivo, F_SELECT, NULL);
        // tem q imprimir uma quebra de linha no final
        printf("\n");

        // Desalocando memória.
        liberar_memoria_estacao(&temp);
    }

    fclose(arquivo);

    return;
}