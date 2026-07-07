#include "util.h"
#include "buscar.h"

// Imprime todos os registros não removidos do arquivo.

// arquivoBin: Nome do arquivo a ser lido.
void SELECT_ALL(char *arquivoBIN) {
    // Abrindo o arquivo
    FILE *arquivo = abrir_arquivo_validar(arquivoBIN, "rb");
    if(arquivo == NULL) return;

    estacao temp;
    nulifica_estacao(&temp);

    // Note que como temp foi "nulificado", qualquer estação do registro vai satisfazer a base(temp), logo, todos vão ser impressos.
    iterar_estacao(&temp, arquivo, F_SELECT, NULL);
    
    fclose(arquivo);

    return;
}