#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../TAD/util.h"
#include "../TAD/buscar.h"
#include "BUSCAR.h"

// Imprime todos os registros não removidos do arquivo que satisfaçam as 'm' condições.

// arquivoBin: Nome do arquivo a ser lido.
// arquivoBin_ar: Nome do arquivo da árvore a ser lido.
// n: Quantidade de buscas a ser feitas
void SELECT_AR(char *arquivoBIN, char *arquivoBIN_ar ,int n) { //O arquivo select da árvore é parecido com o do trabalho anterior, a única mudança é que caso a chave de busca tenha codEstacao, usaremos a árvore.
    
    FILE *arquivo = abrir_arquivo_validar(arquivoBIN, "rb");
    if (arquivo == NULL) return;

    FILE *arquivo_ar = abrir_arquivo_validar(arquivoBIN_ar, "rb");
    if (arquivo_ar == NULL) {
        fclose(arquivo);
        return;
    }

    int raizAr; //Raiz da árvore(serve pra saber onde começar a busca)
    fseek(arquivo_ar,1,SEEK_SET);
    fread(&raizAr,sizeof(int),1,arquivo_ar);

    // Iterando por cada busca
    for(int i = 0; i < n; i++){
        int m; // Quantidade de condições
        estacao temp;
        nulifica_estacao(&temp);

        scanf("%d",&m);
        pegar_info_estacao(&temp,m); // Pega quais condições a estação tem q satisfazer e deixa no temp. 
        // Caso um campo seja NULO, ele é representado no 'temp' por -2
        // Caso um campo não seja especificado (não é uma condição para buscar), ele é representado por -1
        if(temp.codEstacao != -1){ //Se a chave de busca tiver codEstacao vamos usar a árvore.
            int rrnencontrado,posencontrado;
            bool encontrado = busca_arvoreB(arquivo_ar,raizAr,temp.codEstacao,&rrnencontrado,&posencontrado); //se foi encontrado retorna true.
            if(!encontrado){
                printf("Registro inexistente.\n");
            }else{
                estacao possivel_estacao; //Verificação se o campo encontrado bate inteiramente com a busca, não apenas o codEstacao.
                fseek(arquivo_ar,17 + (53 * rrnencontrado) + 17 + (8 * posencontrado),SEEK_SET);
                int index;
                fread(&index,sizeof(int),1,arquivo_ar);
                fseek(arquivo,index + 1,SEEK_SET);
                binario_para_estacao_apontado(&possivel_estacao, arquivo);
                bool satisfez = satisfazBase(&temp,&possivel_estacao);
                if(satisfez){
                    printar_estacao(&possivel_estacao);
                }else{
                    printf("Registro inexistente.\n");
                }

                liberar_memoria_estacao(&possivel_estacao);
            }
        }else{
            iterar_estacao(&temp, arquivo, F_SELECT, NULL);
        }

        // tem q imprimir uma quebra de linha no final
        printf("\n");

        // Desalocando memória.
        liberar_memoria_estacao(&temp);
    }

    //Fechamento de arquivos
    fclose(arquivo);
    fclose(arquivo_ar);

    return;
}