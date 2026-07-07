#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Checa se o usuário passou os dois arquivos como argumento
    if (argc != 3) {
        printf("Uso correto: %s <arquivo1.bin> <arquivo2.bin>\n", argv[0]);
        return 1;
    }

    FILE *f1 = fopen(argv[1], "rb");
    FILE *f2 = fopen(argv[2], "rb");

    // Validação de abertura dos arquivos
    if (f1 == NULL || f2 == NULL) {
        printf("Erro: Nao foi possivel abrir um ou ambos os arquivos.\n");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 1;
    }

    int byte1, byte2;
    long offset = 0;
    long linha, coluna;

    while (1) {
        byte1 = fgetc(f1);
        byte2 = fgetc(f2);

        // Se ambos chegaram ao fim do arquivo ao mesmo tempo
        if (byte1 == EOF && byte2 == EOF) {
            printf("Sucesso: Os arquivos sao EXATAMENTE identicos!\n");
            break;
        }

        // Se encontrou uma diferença (ou um arquivo terminou antes do outro)
        if (byte1 != byte2) {
            linha = (offset / 16) + 1;
            coluna = (offset % 16) + 1;

            printf("--- DIFERENCA ENCONTRADA ---\n");
            printf("Linha: %ld\n", linha);
            printf("Coluna: %ld (Byte %ld da linha)\n", coluna, coluna);
            printf("Offset Total: %ld bytes\n\n", offset);

            if (byte1 == EOF) {
                printf("O '%s' acabou mais cedo (eh menor que o segundo arquivo).\n", argv[1]);
            } else if (byte2 == EOF) {
                printf("O '%s' acabou mais cedo (eh menor que o primeiro arquivo).\n", argv[2]);
            } else {
                // Imprime os bytes diferentes em Hexadecimal para facilitar a leitura
                printf("Valor no %s : %02X\n", argv[1], byte1);
                printf("Valor no %s : %02X\n", argv[2], byte2);
            }
            break;
        }
        offset++;
    }

    fclose(f1);
    fclose(f2);
    return 0;
}