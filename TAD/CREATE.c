#include "CREATE.h"
#include "util.h"

void ate_virgula (char *s,char *linha,int *index) { 
    //A função vai lendo todos os caracteres até chegar numa vírgula ou \n ou \r. Ela retorna essa string
    int i = 0;
    while(linha[*index] != ',' && linha[*index] != '\n' && linha[*index] != '\r'){
        s[i] = linha[*index];
        (*index)++;
        i++;
    }
    s[i] = '\0';
    (*index)++;
}

void CREATE_TABLE (char *arquivoCSV, char *arquivoBIN) {
    FILE *ler = NULL;
    FILE *escrever = NULL;
    ler = fopen(arquivoCSV, "r");
    escrever = fopen(arquivoBIN,"wb");
    
    if(ler == NULL || escrever == NULL) {

        // Um pode ser NULL e o outro nao, entao precisamos checar e desalocar
        if(ler != NULL) fclose(ler);
        if(escrever != NULL) fclose(escrever);

        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    fwrite(&c_zero,sizeof(char),1,escrever); //Demarcar o arquivo como instável no começo

    for(int i = 0; i < 16; i++) {
        // Inicializar o cabeçalho todo com zero para podermos colocar as estações corretamente
        fwrite(&zero, 1, 1, escrever); 
    }

    char str[105];

    fgets(str,105,ler); //pular a primeira linha
    
    int proxRRN = 0;
    int nroEstacoes = 0;
    int nroParesDeEstacoes = 0;
    
    int limite = 100;
    char ** nomesDiferentes = malloc(limite*sizeof(char*));
    
    while(fgets(str,105,ler)) {
        proxRRN++;
        
        estacao temp;
        int index = 0;
        char strtemp[60];
        
        ate_virgula(strtemp,str,&index);        //lê até a proxima virgula e coloca a quantidade inteira
        temp.codEstacao = atoi(strtemp);

        ate_virgula(strtemp,str,&index);        //lê até a proxima virgula e coloca a string inteira no nome
        temp.nomeEstacao = malloc(50 * sizeof(char));
        strcpy(temp.nomeEstacao,strtemp);   
        temp.tamNomeEstacao = strlen(strtemp);  //pega o tamanho do nome da estação 
        
        ate_virgula(strtemp,str,&index);        //lê até a proxima virgula e coloca a quantidade inteira
        temp.codLinha = atoi(strtemp);
        
        ate_virgula(strtemp,str,&index);        //lê até a proxima virgula e coloca a string inteira no nome
        temp.nomeLinha = malloc(50 * sizeof(char));
        strcpy(temp.nomeLinha,strtemp);     
        temp.tamNomeLinha = strlen(strtemp);    //pega o tamanho do nome da estação 
        
        ate_virgula(strtemp,str,&index);        //lê a proxima virgula e verifica se não tem nada, se não tiver nada é -1, se não é o numero que ele deu
        if(strcmp(strtemp,"") == 0){
            temp.codProxEstacao = -1;
        } else {
            temp.codProxEstacao = atoi(strtemp);
        }

        ate_virgula(strtemp,str,&index);        //lê a proxima virgula e verifica se não tem nada, se não tiver nada é -1, se não é o numero que ele deu
        if(strcmp(strtemp,"") == 0){
            temp.distProxEstacao = -1;
        } else {
            temp.distProxEstacao = atoi(strtemp);
        }

        ate_virgula(strtemp,str,&index);        //lê a proxima virgula e verifica se não tem nada, se não tiver nada é -1, se não é o numero que ele deu
        if(strcmp(strtemp,"") == 0){
            temp.codLinhaIntegra = -1;
        } else {
            temp.codLinhaIntegra = atoi(strtemp);
        }

        ate_virgula(strtemp,str,&index);        //lê a proxima virgula e verifica se não tem nada, se não tiver nada é -1, se não é o numero que ele deu
        if(strcmp(strtemp,"") == 0) {
            temp.codEstIntegra = -1;
        } else {
            temp.codEstIntegra = atoi(strtemp);
        }
        
        char frase[(temp.tamNomeEstacao) + 1];  //Duplica o nome estação só que com o \0 no final, o que permite usar as funções de string
        for(int i = 0; i < temp.tamNomeEstacao; i++) frase[i] = temp.nomeEstacao[i];
        frase[temp.tamNomeEstacao] = '\0';

        bool novoNome = true;
        for(int i = 0; i < nroEstacoes; i++) {
            if(strcmp(nomesDiferentes[i], frase) == 0){ //Verifica se o nome de estação já foi usado antes
                novoNome = false;
                break;
            } 
        }

        if(novoNome) {  //Se não foi usado antes, alocamos a memória necessária para alocar o novo nome e colocamos o novo nome lá
            if(nroEstacoes > limite-10) {
                limite *= 2;
                nomesDiferentes = realloc(nomesDiferentes, limite*sizeof(char*));
            }
            nomesDiferentes[nroEstacoes] = malloc((temp.tamNomeEstacao+1) * sizeof(char));
            strcpy(nomesDiferentes[nroEstacoes], frase);
            nroEstacoes++;
        }
        if(temp.codProxEstacao != -1) nroParesDeEstacoes++; // Os pares de estações são únicos
        
        temp.removido = '0';                //Termina de definir as propriedades das estações que não foram definidas
        temp.proximo = -1;

        estacao_para_binario(&temp,proxRRN-1,escrever);
        liberar_memoria_estacao(&temp);
    }

    //Libera a memória de tudo que foi alocado
    for(int i = 0; i < nroEstacoes; i++) {
        free(nomesDiferentes[i]);
        nomesDiferentes[i] = NULL;
    }
    free(nomesDiferentes);
    nomesDiferentes = NULL;

    fseek(escrever, 1, SEEK_SET); // volta p comeco

    int topo = -1;                                  //Escreve o cabeçalho 
    fwrite(&topo, sizeof(int), 1, escrever);
    fwrite(&proxRRN, sizeof(int), 1, escrever);
    
    fwrite(&nroEstacoes, sizeof(int), 1, escrever);
    fwrite(&nroParesDeEstacoes, sizeof(int), 1, escrever);

    fseek(escrever, 0, SEEK_SET);
    fwrite(&c_um,sizeof(char),1,escrever);

    fclose(escrever);
    fclose(ler);
    
    BinarioNaTela(arquivoBIN);

    return;
}