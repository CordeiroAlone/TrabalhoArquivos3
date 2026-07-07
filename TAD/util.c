#include "util.h"
#include "fornecidas.h"

// Constantes frequentemente usados para dar fwrite nos arquivos.
const int um = 1;
const int zero = 0;
const char c_zero = '0';
const char c_um = '1';
const char cifrao = '$';

// Pega dois ponteiros e troca seus valores.
void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

// Imprime uma estação na formatação correta, considerando os campos NULOs.

// e: Ponteiro para a estação a ser impressa
void printar_estacao(estacao* e) {

    printf("%d ", e->codEstacao);
    if(e->nomeEstacao != NULL) {
        printf("%s ", e->nomeEstacao);
    } else printf("NULO ");
    printf("%d ", e->codLinha);
    if(e->nomeLinha != NULL) {
        printf("%s ", e->nomeLinha);
    } else printf("NULO ");
    
    if(e->codProxEstacao != -1) {
        printf("%d ", e->codProxEstacao);
    } else printf("NULO ");
    if(e->distProxEstacao != -1) {
        printf("%d ", e->distProxEstacao);
    } else printf("NULO ");
    if(e->codLinhaIntegra != -1) {
        printf("%d ", e->codLinhaIntegra);
    } else printf("NULO ");
    if(e->codEstIntegra != -1) {
        printf("%d\n", e->codEstIntegra);
    } else printf("NULO\n");

    return;
}

// Preenche a struct com valores nulos (-1 para inteiros e NULL para strings)

// temp: estação a ser realizado o preenchimento
void nulifica_estacao(estacao* temp){
    temp->codEstacao = -1;
    temp->codLinha = -1;
    temp->codProxEstacao = -1;
    temp->distProxEstacao = -1;
    temp->codLinhaIntegra = -1;
    temp->codEstIntegra = -1;
    temp->tamNomeEstacao = -1;
    temp->nomeEstacao = NULL;
    temp->tamNomeLinha = -1;
    temp->nomeLinha = NULL;
}

// Conta a quantidade de estações DIFERENTES que tem

// arquivo: Arquivo binário contendo os registros.
int conta_estacao(FILE* arquivo) {

    fseek(arquivo,5,SEEK_SET);
    int registros; // Quantidade de registros existentes no arquivo
    fread(&registros,sizeof(int),1,arquivo);
    
    // Salva todos os nomes encontrados nos registros
    char** lista = malloc(registros * sizeof(char*));
    int diferentes = 0; // Contador de quantos nomes diferentes tem nos registros

    // Iterando por cada registro
    for(int i = 0; i < registros; i++) {
        fseek(arquivo,17 + (80 * i),SEEK_SET); // O cabeçalho tem 17 bytes e cada registro tem 80 bytes

        char removido;
        fread(&removido,sizeof(char),1,arquivo);
        if(removido == '1') continue; // Ignora estações marcadas como removidas

        // Le o nome da estação e armazena na frase.
        int tamstring;
        fseek(arquivo,17 + 29 + (80 * i),SEEK_SET);
        fread(&tamstring,sizeof(int),1,arquivo);
        char frase[tamstring + 1];                  //temos que sempre declarar um caractere a mais pois a função strcmp não funciona sem o \0 no final
        fread(frase,tamstring,1,arquivo);
        frase[tamstring] = '\0'; // Adicionando o terminador da string

        bool novo = true; // flag representando se o nome ainda não apareceu no registro.
        for(int j = 0; j < diferentes; j++) {
            if(strcmp(frase,lista[j]) == 0) { // Passa por cada nome já visto e marca o flag como falso se já existir
                novo = false;
                break;
            }
        }
        if(novo) {
            // Caso o nome não tenha aparecido ainda, precisamos salva-lo na lista.
            lista[diferentes] = malloc((tamstring + 1) * sizeof(char));
            for(int j = 0; j <= tamstring; j++){
                lista[diferentes][j] = frase[j];
            }
            diferentes++; // Incrementando o contador
        }
    }
    // Desalocando a memória
    for(int i = 0; i < diferentes; i++){
        free(lista[i]);
    }

    free(lista);
    lista = NULL;
    return diferentes;
}

// Recebe uma linha do terminal, contendo as informações de uma estação, e atualiza a "estacao nova",
// rep: Quantidade de campos que será especificado para a estação.

// O campo da estação se tornará -1 se não for especificado e -2 se deve ser NULO. 
void pegar_info_estacao(estacao *nova,int rep){
    for(int i = 0; i < rep; i++){ // Iterando por cada campo
        char s[20];
        scanf(" %s",s);

        // Checamos qual campo deve ser atualizado e o atualizamos.
        if(strcmp(s, "codEstacao") == 0) {
            scanf(" %d",&nova->codEstacao); // Recebe o valor do campo.
        } else if(strcmp(s,"codLinha") == 0) {
            scanf(" %d",&nova->codLinha);
        } else if(strcmp(s,"codProxEstacao") == 0) {
            char* temp = malloc(sizeof(char) * 20);
            scanf("%s",temp);
            if(strcmp(temp,"NULO") == 0){ // caso NULO;
                nova->codProxEstacao = -2;
                free(temp);
                continue;
            }
            nova->codProxEstacao = atoi(temp); // atoi transforma um string para inteiro
            free(temp);
        } else if(strcmp(s,"distProxEstacao") == 0) {
            char* temp = malloc(sizeof(char) * 20);
            scanf("%s",temp);
            if(strcmp(temp,"NULO") == 0){
                nova->distProxEstacao = -2;
                free(temp);
                continue;
            }
            nova->distProxEstacao = atoi(temp);
            free(temp);
        } else if(strcmp(s,"codLinhaIntegra") == 0) {
            char* temp = malloc(sizeof(char) * 20);
            scanf("%s",temp);
            if(strcmp(temp,"NULO") == 0){
                nova->codLinhaIntegra = -2;
                free(temp);
                continue;
            }
            nova->codLinhaIntegra = atoi(temp);
            free(temp);
        } else if(strcmp(s,"codEstIntegra") == 0) {
            char* temp = malloc(sizeof(char) * 20);
            scanf("%s",temp);
            if(strcmp(temp,"NULO") == 0){
                nova->codEstIntegra = -2;
                free(temp);
                continue;
            }
            nova->codEstIntegra = atoi(temp);
            free(temp);
        } else if(strcmp(s,"nomeEstacao") == 0) {
            nova->nomeEstacao = malloc(sizeof(char) * 50);
            ScanQuoteString(nova->nomeEstacao);
            int tam = 0;
            while(nova->nomeEstacao[tam] != '\0') tam++;
            nova->tamNomeEstacao = tam;
        }else if(strcmp(s,"nomeLinha") == 0) {
            nova->nomeLinha = malloc(sizeof(char) * 50);
            ScanQuoteString(nova->nomeLinha);
            int tam = 0;
            while(nova->nomeLinha[tam] != '\0') tam++;
            nova->tamNomeLinha = tam;
        } else {
            printf("Campo invalido\n");
        }
    }
}

// Recebe uma estação e sua RRN e atualiza o seu registro no arquivo binario.

// estacao *est: Struct da estação a ser transformado para binário e atualizado no arquivo.
// int index: Posição(índice) do arquivo para atualizar
// FILE* arquivo: Arquivo onde deve ser feito a atualização
void estacao_para_binario(estacao *est, int index ,FILE* arquivo){
    fseek(arquivo,17 + (80 * index),SEEK_SET); // 17 bytes para o cabeçalho + 80 para cada registro
    int bytesusados = 37; // A parte de tamanho fixo do registro tem 37 bytes. (Sem contar com tamanho das strings)

    // Atualiza cada campo do registro.
    fwrite(&est->removido,sizeof(char),1,arquivo);
    fwrite(&est->proximo,sizeof(int),1,arquivo);
    fwrite(&est->codEstacao,sizeof(int),1,arquivo);
    fwrite(&est->codLinha,sizeof(int),1,arquivo);
    fwrite(&est->codProxEstacao,sizeof(int),1,arquivo);
    fwrite(&est->distProxEstacao,sizeof(int),1,arquivo);
    fwrite(&est->codLinhaIntegra,sizeof(int),1,arquivo);
    fwrite(&est->codEstIntegra,sizeof(int),1,arquivo);

    // Atualizando nomeEstacao e nomeLinha
    int tam = 0;
    while(est->nomeEstacao[tam] != 0) tam++; // Conta a quantidade de caracteres do nomeEstacao (parando antes do \0)
    fwrite(&tam,sizeof(int),1,arquivo);
    bytesusados += tam;
    fwrite(est->nomeEstacao,sizeof(char) * tam,1,arquivo);
    tam = 0;
    while(est->nomeLinha[tam] != 0) tam++; // Conta a quantidade de caracteres do nomeLinha (parando antes do \0)
    fwrite(&tam,sizeof(int),1,arquivo);
    bytesusados += tam;
    fwrite(est->nomeLinha,sizeof(char) * tam,1,arquivo);
    for(int i = bytesusados; i < 80; i++){ // Preenche o lixo com cifrao ($)
        fwrite(&cifrao,sizeof(char),1,arquivo);
    }
}

// Função contrária do estacao_para_binario. Recebe um arquivo e uma RRN e atualiza esses dados na estacao "est".

// estacao *est: Struct da estação que no final representará o mesmo que o binario do arquivo.
// int index: Posição(índice) do arquivo de onde vem a atualização
// FILE* arquivo: Arquivo onde deve ser conseguidos os dados para atualizar.
void binario_para_estacao(estacao *est, int index,FILE* arquivo){
    fseek(arquivo,17 + (80 * index),SEEK_SET);

    // Realiza a leitura dos campos de tamanho fixo e atualiza a estação.
    fread(&est->removido,sizeof(char),1,arquivo);
    fread(&est->proximo,sizeof(int),1,arquivo);
    fread(&est->codEstacao,sizeof(int),1,arquivo);
    fread(&est->codLinha,sizeof(int),1,arquivo);
    fread(&est->codProxEstacao,sizeof(int),1,arquivo);
    fread(&est->distProxEstacao,sizeof(int),1,arquivo);
    fread(&est->codLinhaIntegra,sizeof(int),1,arquivo);
    fread(&est->codEstIntegra,sizeof(int),1,arquivo);
    
    // Lê o tamanho das strings, aloca o tamanho certo e atualiza a estacao.
    fread(&est->tamNomeEstacao,sizeof(int),1,arquivo);
    est->nomeEstacao = malloc((est->tamNomeEstacao + 1) * sizeof(char));
    fread(est->nomeEstacao,sizeof(char),est->tamNomeEstacao,arquivo);
    est->nomeEstacao[est->tamNomeEstacao] = '\0'; // Inserindo o terminador de string pois no arquivo binário não tem.
    
    fread(&est->tamNomeLinha,sizeof(int),1,arquivo);
    est->nomeLinha = malloc((est->tamNomeLinha + 1) * sizeof(char));
    fread(est->nomeLinha,sizeof(char),est->tamNomeLinha,arquivo);
    est->nomeLinha[est->tamNomeLinha] = '\0';
}

//Função similar a de cima só que com peculiaridades para encaixar no buscar. (começa a busca de onde o ponteiro parou e consome o lixo)

// estacao *est: Struct da estação que no final representará o mesmo que o binario do arquivo.
// FILE* arquivo: Arquivo onde deve ser conseguidos os dados para atualizar.
void binario_para_estacao_apontado(estacao *est,FILE* arquivo){

    // Realiza a leitura dos campos de tamanho fixo e atualiza a estação.
    est->removido = 0;
    fread(&est->proximo,sizeof(int),1,arquivo);
    fread(&est->codEstacao,sizeof(int),1,arquivo);
    fread(&est->codLinha,sizeof(int),1,arquivo);
    fread(&est->codProxEstacao,sizeof(int),1,arquivo);
    fread(&est->distProxEstacao,sizeof(int),1,arquivo);
    fread(&est->codLinhaIntegra,sizeof(int),1,arquivo);
    fread(&est->codEstIntegra,sizeof(int),1,arquivo);
    
    // Lê o tamanho das strings, aloca o tamanho certo e atualiza a estacao.
    fread(&est->tamNomeEstacao,sizeof(int),1,arquivo);
    est->nomeEstacao = malloc((est->tamNomeEstacao + 1) * sizeof(char));
    fread(est->nomeEstacao,sizeof(char),est->tamNomeEstacao,arquivo);
    est->nomeEstacao[est->tamNomeEstacao] = '\0'; // Inserindo o terminador de string pois no arquivo binário não tem.
    
    fread(&est->tamNomeLinha,sizeof(int),1,arquivo);
    est->nomeLinha = malloc((est->tamNomeLinha + 1) * sizeof(char));
    fread(est->nomeLinha,sizeof(char),est->tamNomeLinha,arquivo);
    est->nomeLinha[est->tamNomeLinha] = '\0';
    int bytesl = 37 + est->tamNomeEstacao + est->tamNomeLinha;
    int lixo = 80 - bytesl;
    if(lixo > 0){
        char lixos[lixo];
        fread(lixos,sizeof(char),lixo,arquivo);
    }
}

// Retorna o nome de uma estação a partir do indice e atualiza na variavel nome.

// char **nome: Variavel a ser atualizado com o nome da estação encontrada.
// FILE* arquivo: Arquivo onde a busca ocorre.
// int index: indice da estação de onde o nome deve ser buscado.
void pegaNomeEst(char **nome,FILE* arquivo, int index){

    fseek(arquivo,17 + (80 * index) + 29,SEEK_SET);
    
    // Lê o tamanho das strings, aloca o tamanho certo e atualiza a estacao.
    int tam;
    fread(&tam,sizeof(int),1,arquivo);
    *nome = malloc((tam + 1) * sizeof(char));
    fread(*nome,sizeof(char),tam,arquivo);
    (*nome)[tam] = '\0'; // Inserindo o terminador de string pois no arquivo binário não tem.

}

// Abre um arquivo usando a string do seu nome e verifica se ele existe e a sua estabilidade.
// char *arquivoBIN: Nome do arquivo
// char *modo: Modo de abertura do arquivo
FILE* abrir_arquivo_validar(char *arquivoBIN, char* modo) {
    // Abertura de arquivos e verificação de validade
    FILE *arquivo = fopen(arquivoBIN, modo);
    if(arquivo == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    // Checa se o arquivo é instável
    char status = '0';
    fread(&status, sizeof(char), 1, arquivo);
    if(status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivo);
        return NULL;
    }

    return arquivo;
}

// Recebe uma estação e desaloca a memória dos strings presente.
// estacao* est: estacao de onde as strings devem ser desalocadas.
void liberar_memoria_estacao(estacao* est) {
    if(est->nomeEstacao != NULL) {
        free(est->nomeEstacao);
        est->nomeEstacao = NULL; 
    }
    
    if(est->nomeLinha != NULL) {
        free(est->nomeLinha);
        est->nomeLinha = NULL;
    }
}