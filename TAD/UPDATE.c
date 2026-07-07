#include "UPDATE.h"

void UPDATE(char *arquivoBIN,int vezes) {
    FILE *arquivo = abrir_arquivo_validar(arquivoBIN, "rb+");
    if(arquivo == NULL) return;

    // Marca o arquivo como instável
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&c_zero,sizeof(char),1,arquivo);

    for(int i = 0; i < vezes; i++) {
        int m;

        // definição das estações antiga que deve ser atualizada
        estacao temp;
        nulifica_estacao(&temp);
        scanf("%d",&m);
        pegar_info_estacao(&temp,m);

        // transformar é as novas informações a serem colocandos sobrepostas na estação antiga
        estacao transformar;
        scanf("%d",&m);
        nulifica_estacao(&transformar);
        pegar_info_estacao(&transformar,m);

        // Passa por cada estação e faz o update. Função feita em util.h
        iterar_estacao(&temp, arquivo, F_UPDATE, &transformar);

        // liberando memória
        liberar_memoria_estacao(&temp);
        liberar_memoria_estacao(&transformar);
    }

    // Note que não está sendo feita a atualização de qtd de estação e pares, pois foi especificado que o comando de UPDATE não irá alterar esses valores.
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&c_um,sizeof(char),1,arquivo);
    fclose(arquivo);
    BinarioNaTela(arquivoBIN);
}