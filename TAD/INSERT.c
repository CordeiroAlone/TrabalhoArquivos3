#include "INSERT.h"

void lerlinha(estacao* est) {
    est->removido = '0';
    est->proximo = -1;

    scanf("%d",&est->codEstacao);                   //Ler código estação
    est->nomeEstacao = malloc(50 * sizeof(char));
    
    ScanQuoteString(est->nomeEstacao);              //Ler nome estação e determinar seu tamanho
    int tam = 0;
    while(est->nomeEstacao[tam] != '\0')tam++;
    est->tamNomeEstacao = tam;
    
    scanf(" %d",&est->codLinha);                    //Ler código linha
    est->nomeLinha = malloc(50 * sizeof(char));     
    
    ScanQuoteString(est->nomeLinha);                //Ler nome linha e determinar seu tamanho
    tam = 0;
    while(est->nomeLinha[tam] != '\0')tam++;
    est->tamNomeLinha = tam;
    
    char temp[20];
    
    scanf("%s",temp);                                       //Ler código da próxima estação depois de verificar que não é nulo
    if(strcmp(temp,"NULO") == 0) est->codProxEstacao = -1;
    else est->codProxEstacao = atoi(temp);
    
    scanf("%s",temp);                                       //Ler distância da próxima estação depois de verificar que não é nulo
    if(strcmp(temp,"NULO") == 0) est->distProxEstacao = -1;
    else est->distProxEstacao = atoi(temp);
    
    scanf("%s",temp);                                       //Ler código da linha integra depois de verificar que não é nulo
    if(strcmp(temp,"NULO") == 0) est->codLinhaIntegra = -1;
    else est->codLinhaIntegra = atoi(temp);
    
    scanf("%s",temp);                                       //Ler código da estação integra depois de verificar que não é nulo
    if(strcmp(temp,"NULO") == 0) est->codEstIntegra = -1;
    else est->codEstIntegra = atoi(temp);
}

void INSERT(char *arquivoBIN,int vezes){
    FILE *arquivo = abrir_arquivo_validar(arquivoBIN, "rb+");
    if(arquivo == NULL) return;

    // Marca o arquivo instável
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&c_zero,sizeof(char),1,arquivo);

    int topo,proxRRN;
    fseek(arquivo,1,SEEK_SET);
    fread(&topo,sizeof(int),1,arquivo);     //Lê o topo da pilha e a quantidade de registros
    fread(&proxRRN,sizeof(int),1,arquivo);

    int paresAdicionados = 0;               //Conta quantos pares de estação foram adicionados

    for(int i = 0; i < vezes; i++){
        estacao est;

        lerlinha(&est);                     //Lê e interpreta a nova linha e coloca tudo na estação. Ou seja, a estação a ser inserida está em "est"

        if(est.codEstacao != -1 && est.codProxEstacao != -1) paresAdicionados++; //todo par de estação é único segundo o que nos foi dito

        //Como aprendemos em sala, caso o topo da pilha de removidos aponte para um RRN válido, colocaremos o novo registro lá. 
        //Caso o topo seja -1, ou seja, não existem espaços deletados disponíveis, aí é necessário abrir mais armazenamento no arquivo.
        if(topo == -1){                                     //Verificar se o topo aponta para algo
            estacao_para_binario(&est,proxRRN,arquivo);     //Caso não exista espaço válido, ele cria uma nova estação no próximo RRN disponível
            proxRRN++;                                      //A quantidade de registros aumenta
        }else{
            int atual = topo;
            fseek(arquivo,17 + 1 + (80 * topo),SEEK_SET);   
            fread(&topo,sizeof(int),1,arquivo);             //Topo muda para o próximo ponteiro, que está no registro da estação.
            estacao_para_binario(&est,atual,arquivo);       //Coloca a nova estação no local que estávamos.
        }

        //Liberar memória
        liberar_memoria_estacao(&est);
    }
    
    int qtdParEstacao;
    fseek(arquivo,13,SEEK_SET);
    fread(&qtdParEstacao,sizeof(int),1,arquivo);    //Lê a quantidade de pares de estação anterior
    qtdParEstacao += paresAdicionados;              //Adiciona as novas encontradas

    fseek(arquivo,1,SEEK_SET);
    fwrite(&topo,sizeof(int),1,arquivo);            //Escreve as mudanças no arquivo do topo e da quantidade de registros
    fwrite(&proxRRN,sizeof(int),1,arquivo);

    //int qtdEstacao = conta_estacao(arquivo); Segundo o que nos falaram, o insert não muda a quantidade de estações.
    fseek(arquivo,13,SEEK_SET);
    fwrite(&qtdParEstacao,sizeof(int),1,arquivo);   //Escreve as mudanças no arquivo da quantidade de pares de estação
    
    // Marcando o arquivo como estável.
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&c_um,sizeof(char),1,arquivo);
    fclose(arquivo);
    
    BinarioNaTela(arquivoBIN);
}