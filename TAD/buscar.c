#include "buscar.h"

void fazer_select(estacao* temp);

void fazer_delete(FILE* arquivo, int RRN, int* topo);

void UPDATE_ONE(estacao* mudar, estacao* mudanca);
void fazer_update(FILE* arquivo, int RRN, estacao* transformar);

// Checa se a estação temp satisfaz as condições para a busca.

// base: Representa as condições que devem ser satisfeitas.
//       Caso um campo seja NULO, ele é representado por -2
//       Caso um campo não seja especificado (não é uma condição para buscar), ele é representado por -1
// temp: Representa a estação em que estamos checando se satisfaz a base.
//       Caso um campo seja NULO, ele é representado por -1
bool satisfazBase(estacao* base, estacao* temp) {
    // Passamos por cada campo do registro checando se é igual
    if(base->codEstacao != -1 && base->codEstacao != temp->codEstacao) {
        return false; 
    }

    if(base->codLinha != -1 && base->codLinha != temp->codLinha) {
        return false;
    }

    if(base->codProxEstacao != -1) {
        if(base->codProxEstacao == -2) { // NULO
            if(temp->codProxEstacao != -1) return false;
        }else{
            if(temp->codProxEstacao != base->codProxEstacao) return false;
        }
    }

    if(base->distProxEstacao != -1 && base->distProxEstacao != temp->distProxEstacao) {
        if(base->distProxEstacao == -2) { // NULO
            if(temp->distProxEstacao != -1) return false;
        }else{
            if(temp->distProxEstacao != base->distProxEstacao) return false;
        }
    }

    if(base->codLinhaIntegra != -1) {
        if(base->codLinhaIntegra == -2) { // NULO
            if(temp->codLinhaIntegra != -1) return false;
        }else{
            if(temp->codLinhaIntegra != base->codLinhaIntegra) return false;
        }
    }

    if(base->codEstIntegra != -1) {
        if(base->codEstIntegra == -2){
            if(temp->codEstIntegra != -1) return false;
        }else{
            if(temp->codEstIntegra != base->codEstIntegra) return false;
        }
    }

    if(base->nomeEstacao != NULL && strcmp(base->nomeEstacao, temp->nomeEstacao) != 0) {
        return false;
    }

    if(base->nomeLinha != NULL && strcmp(base->nomeLinha, temp->nomeLinha) != 0) {
        return false;
    }

    return true; // A estação temp satisfaz as condições para busca
}

// Itera pelo arquivo de registros e Imprime, Deleta ou Atualiza registros que satisfazerem a base

// base: Representa as condições que devem ser satisfeitas.
//       Caso um campo seja NULO, ele é representado por -2
//       Caso um campo não seja especificado (não é uma condição para buscar), ele é representado por -1
// arquivo: Arquivo binário dos registros onde será feito a busca
// funcionalidade: Ve qual funcionalidade está chamando essa função
    // F_SELECT: funciona tanto para SELECT quanto SELECT_ALL
    // F_DELETE
    // F_UPDATE
// transformar: Usado no update, representa a estação em q a original deve ser transformada.

// Caso Funcionalidade seja de deletar, retorna a quantidade de pares de estações deletadas. Se não retorna 0.
int iterar_estacao(estacao* base, FILE* arquivo, Funcionalidade funcionalidade, estacao* transformar) {
    // Lê o topo da pilha    
    int topo;
    fseek(arquivo, 1, SEEK_SET);
    fread(&topo,sizeof(int),1,arquivo);

    int cont = 0; // Quantidades de estações que satisfazem a base
    int parestirados = 0;
    
    // Lë a quantidade de registros no arquivo
    int tam;
    fseek(arquivo, 5, SEEK_SET);
    fread(&tam,sizeof(int),1,arquivo);


    fseek(arquivo, 17, SEEK_SET); //Coloca o ponteiro no final do cabeçalho
    for(int i = 0; i < tam; i++) { // Iterando sobre cada registro

        // Colocando o registro atual em 'temp'
        estacao temp;
        fread(&temp.removido,sizeof(char),1,arquivo);
        if(temp.removido == '1'){
            if(i != tam-1) fseek(arquivo,17 + ((i+1) * 80),SEEK_SET); // O cabeçalho tem 17 bytes e cada registro tem 80 bytes.
            continue;
        } 

        binario_para_estacao_apontado(&temp,arquivo);
        
        if(satisfazBase(base, &temp)) { // Caso a estação deva ser buscada, podemos imprimi-la, e colocamos o seu RRN na lista de encontrados.
            cont++; // Quantidades de estações buscadas.
            if(funcionalidade == F_SELECT) { // precisamos imprimir
                fazer_select(&temp);
            } else if(funcionalidade == F_DELETE) {
                if(temp.codEstacao != -1 && temp.codProxEstacao != -1){
                    parestirados++; // Se codProxEstacao não for inválido(-1), significa que tiramos um par de estação.
                }
                fazer_delete(arquivo, i, &topo);
                fseek(arquivo,17 + ((i+1) * 80),SEEK_SET);
            } else if(funcionalidade == F_UPDATE) {
                fazer_update(arquivo, i, transformar);
            }
            if(base->codEstacao != -1){
                liberar_memoria_estacao(&temp);
                break;
            } 
        }

        // Liberando memória.
        liberar_memoria_estacao(&temp);
    }
    
    if(funcionalidade == F_SELECT && cont == 0) printf("Registro inexistente.\n");


    if(funcionalidade == F_DELETE){
        fseek(arquivo,1,SEEK_SET);
        fwrite(&topo, sizeof(int), 1, arquivo);
        return parestirados; // Retorna a quantidade de pares de estações tirados. 
    }    
    else return 0;

}

// No comando select, após encontrar uma estação que satisfaz a base, basta imprimi-lo.
void fazer_select(estacao* temp) {
    printar_estacao(temp);
    liberar_memoria_estacao(temp);

    return;
}

// No comando delete, após encontrar uma estação que satisfaz a base, basta marcar "removidos" como 1.
void fazer_delete(FILE* arquivo, int RRN, int* topo) {
    fseek(arquivo, 17 + (80) * RRN, SEEK_SET);        // Cabeçalho tem 17 bytes, cada registro tem tamanho 80.
    fwrite(&c_um,sizeof(char),1,arquivo);                  // Troca o removidos para 1
                                                            // Atualiza o campo "próximo" para o último registro removido (armazenado em topo)
    fwrite(topo,sizeof(int),1,arquivo);
                                                            // Atualiza o topo da pilha de removidos para o registro atual.
    (*topo) = RRN;
    
}

// Coloca as informações de uma estacao(mudanca) para outra(mudar).
// Se um campo da struct for -1, significa que não precisa mudar esse campo
// Se o campo da struct for -2, significa que esse campo deve ser trocado para NULO (que seria -1).
void UPDATE_ONE(estacao* mudar, estacao* mudanca){

    // Passa por cada campo, atualizando-o.
    if(mudanca->codEstacao != -1) {
        mudar->codEstacao = mudanca->codEstacao;
    }

    if(mudanca->codLinha != -1) {
        mudar->codLinha = mudanca->codLinha;
    }

    if(mudanca->codProxEstacao != -1){
        if(mudanca->codProxEstacao == -2) {
            mudar->codProxEstacao = -1;
        } else {
            mudar->codProxEstacao = mudanca->codProxEstacao;
        }
    }

    if(mudanca->distProxEstacao != -1) {
        if(mudanca->distProxEstacao == -2) {
            mudar->distProxEstacao = -1;
        } else {
            mudar->distProxEstacao = mudanca->distProxEstacao;
        }
    }

    if(mudanca->codLinhaIntegra != -1){
        if(mudanca->codLinhaIntegra == -2){
            mudar->codLinhaIntegra = -1;
        } else {
            mudar->codLinhaIntegra = mudanca->codLinhaIntegra;
        }
    }

    if(mudanca->codEstIntegra != -1){
        if(mudanca->codEstIntegra == -2){
            mudar->codEstIntegra = -1;
        }else{
            mudar->codEstIntegra = mudanca->codEstIntegra;
        }
    }

    if(mudanca->tamNomeEstacao != -1){
        mudar->tamNomeEstacao = mudanca->tamNomeEstacao;
        if(mudar->nomeEstacao != NULL) free(mudar->nomeEstacao);
        mudar->nomeEstacao = malloc(sizeof(char) * (mudanca->tamNomeEstacao + 1));
        for(int i = 0; i <= mudanca->tamNomeEstacao;i++){
            mudar->nomeEstacao[i] = mudanca->nomeEstacao[i];
        }
    }

    if(mudanca->tamNomeLinha != -1){
        mudar->tamNomeLinha = mudanca->tamNomeLinha;
        if(mudar->nomeLinha != NULL) free(mudar->nomeLinha);
        mudar->nomeLinha = malloc(sizeof(char) * (mudanca->tamNomeLinha + 1));
        
        for(int i = 0; i <= mudanca->tamNomeLinha;i++){
            mudar->nomeLinha[i] = mudanca->nomeLinha[i];
        }
    }
}

// No comando update, após encontrar uma estação que satisfaz a base, precisamos
// encontrar o RRN no arquivo binário, atualizar a estação e colocar de volta no arquivo.

void fazer_update(FILE* arquivo, int RRN, estacao* transformar) {
    estacao mudar;

    // Usando o RRN da estação, colocamos os dados do arquivo em "mudar"
    binario_para_estacao(&mudar,RRN,arquivo);
    UPDATE_ONE(&mudar,transformar);
    
    // Após a struct ser atualizada, colocamos-o novamente de volta para o arquivo binário.
    estacao_para_binario(&mudar,RRN,arquivo);
    
    // Desalocando memória.
    liberar_memoria_estacao(&mudar);
}