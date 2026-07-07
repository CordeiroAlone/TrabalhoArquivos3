#include "DELETE.h"
#include "buscar.h"
// Funcionalidade de deletar.
// 
// arquivoBIN: Nome do arquivo binario
// n: Quantidade de vezes que ocorre a deleção
int DELETE(char *arquivoBIN, int n){
    // Abrindo o arquivo
    FILE *escrever = abrir_arquivo_validar(arquivoBIN, "rb+");
    if(escrever == NULL) return -1;
    
    // Mudando o status do arquivo para instável.
    fseek(escrever, 0, SEEK_SET);
    fwrite(&c_zero, sizeof(char), 1, escrever);
    fseek(escrever, 1, SEEK_SET);                         //de acordo com o padrão do C não pode dar um fread e um fwrite seguidos.

    int paresTirados = 0;
    
    for(int i = 0; i < n; i++) {                // Loop para cada deleção
        int m;
        estacao temp;
        nulifica_estacao(&temp);                // Presente em util.c
        scanf("%d",&m);
        pegar_info_estacao(&temp,m);            // Presente em util.c

        paresTirados += iterar_estacao(&temp, escrever, F_DELETE, NULL);   // Presente em util.c
        
        // Desalocando memória
        liberar_memoria_estacao(&temp);
    }
    
    int novoTamEstacao = conta_estacao(escrever);
    
    int novoTamPares;
    // Le a quantidade de pares existentes e subtrai pela quantidade de par tirado pela remoção.
    fseek(escrever, 13, SEEK_SET);
    fread(&novoTamPares, sizeof(int), 1, escrever);
    novoTamPares -= paresTirados;
    // Reescrevendo o cabeçalho
    fseek(escrever,9,SEEK_SET);
    fwrite(&novoTamEstacao, sizeof(int), 1, escrever);
    fwrite(&novoTamPares, sizeof(int), 1, escrever);
    fseek(escrever, 0, SEEK_SET);
    fwrite(&c_um, sizeof(char), 1, escrever);          // Marca como arquivo estavel
    fclose(escrever);

    BinarioNaTela(arquivoBIN);
}