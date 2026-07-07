#include "CREATE.h"
#include "INSERT.h"
#include "../TAD/fornecidas.h"

// Função usada para criar uma árvore, no arquivo de escrita bin_arvore

// char *bin_lista: Nome do arquivo onde estão os dados sequenciais
// char *bin_arvore: Nome do arquivo onde deve ser guardado a árvore
void criar_arvore(char *bin_lista, char *bin_arvore) {
    //abertura dos arquivos e verificação se é valido
    FILE *arquivo_ar = fopen(bin_arvore, "wb+");
    FILE *arquivo_lista = abrir_arquivo_validar(bin_lista, "rb");
    if(arquivo_lista == NULL) {
        fclose(arquivo_ar);
        return;
    }
   
    //Criar cabeçalho da arvore e já definir ele como instavel
    CabecalhoArvore *cabecalho = criar_cabecalho();

    cabecalho->status = '0'; // instavel
    escrever_cabecalho(arquivo_ar, cabecalho);

    //pegamos o tamanho da lista e começamos a ciclar por ela.
    int tam = 0;
    fseek(arquivo_lista,5,SEEK_SET);
    fread(&tam,sizeof(int),1,arquivo_lista);

    for(int i = 0; i < tam; i++) { 
        int offset_registro = 17 + (i * 80);

        fseek(arquivo_lista,offset_registro,SEEK_SET); //vai no offset certo da lista
        char removido;
        fread(&removido,sizeof(char),1,arquivo_lista); //se estiver removido ignora
        if(removido == '1') continue;

        fseek(arquivo_lista, offset_registro + 5 , SEEK_SET);
        int chave;
        fread(&chave, sizeof(int), 1, arquivo_lista);
        if(chave == -1) continue; //se o registro não tiver codEstacao ele não é inserido na arvore

        int pr = offset_registro; 
        bool temp = INSERT_ARVORE(arquivo_ar, chave, pr,cabecalho); //Coloca o elemento na árvore
    }

    escrever_cabecalho(arquivo_ar,cabecalho); //escreve o cabeçalho final depois de todas as alterações
    
    //deixa tudo estável e fecha tudo
    fseek(arquivo_ar, 0, SEEK_SET);
    marcar_estavel(arquivo_ar);

    free(cabecalho);
    fclose(arquivo_ar);
    fclose(arquivo_lista);

    BinarioNaTela(bin_arvore);
}