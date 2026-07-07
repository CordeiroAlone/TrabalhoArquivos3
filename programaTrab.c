#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "TAD/CREATE.h"
#include "TAD/SELECT.h"
#include "TAD/DELETE.h"
#include "TAD/fornecidas.h"
#include "TAD/util.h"
#include "TAD/SELECT_ALL.h"
#include "TAD/UPDATE.h"
#include "TAD/INSERT.h"
#include "arvoreB/CABECALHO.h"
#include "arvoreB/CREATE.h"
#include "arvoreB/INSERT.h"
#include "arvoreB/NO.h"
#include "arvoreB/BUSCAR.h"
#include "arvoreB/INSERT_INTO.h"
#include "arvoreB/SELECT.h"
#include "arvoreB/DELETE.h"
#include "merge/SELECTDUPLO.h"
#include "merge/SELECTUNICO.h"
#include "merge/ORDENAR.h"
#include "merge/ORDINTER.h"

int main(void) {
    int comando;
    scanf("%d",&comando);

    if(comando == 1) {
        char entrada[40];
        char saida[40];

        scanf(" %s",entrada);   //Pegar o nome do arquivo de entrada
        scanf(" %s",saida);     //Pegar o nome do arquivo de saída
        
        CREATE_TABLE(entrada,saida);
    } else if(comando == 2) {
        char entrada[40];

        scanf(" %s",entrada);   //Pegar o nome do arquivo de entrada
        
        SELECT_ALL(entrada);
    } else if(comando == 3) {
        char entrada[40];
        int n;

        scanf(" %s",entrada);   //Pegar o nome do arquivo de entrada
        scanf(" %d",&n);        //Quantidade de iterações

        SELECT(entrada,n);
    } else if(comando == 4) {
        char entrada[40];
        int n;

        scanf(" %s",entrada);   //Pegar o nome do arquivo de entrada
        scanf(" %d",&n);        //Quantidade de iterações
        
        DELETE(entrada,n);
    } else if(comando == 5) {
        char entrada[40];
        int n;

        scanf(" %s",entrada);   //Pegar o nome do arquivo de entrada
        scanf(" %d",&n);        //Quantidade de iterações
        
        INSERT(entrada,n);
    } else if(comando == 6) {
        char entrada[40];
        int n;
        
        scanf(" %s",entrada);   //Pegar o nome do arquivo de entrada
        scanf(" %d",&n);        //Quantidade de iterações
        
        UPDATE(entrada,n);
    }else if(comando == 7){
        char lista[40];
        char arvore[40];

        scanf(" %s",lista);   //Pegar o nome do arquivo de entrada
        scanf(" %s",arvore);

        criar_arvore(lista,arvore);
    }else if(comando == 8){
        char lista[40];
        char arvore[40];
        int n;

        scanf(" %s",lista);   //Pegar o nome do arquivo de entrada
        scanf(" %s",arvore);
        scanf(" %d",&n);

        SELECT_AR(lista,arvore,n);
    }else if(comando == 9){
        char lista[40];
        char arvore[40];
        int n;

        scanf(" %s",lista);   //Pegar o nome do arquivo de entrada
        scanf(" %s",arvore);
        scanf(" %d",&n);

        INSERT_INTO(lista,arvore,n);
    } else if(comando == 10) { char lista[40];
        char arvore[40];
        int n;

        scanf(" %s",lista);   //Pegar o nome do arquivo de entrada
        scanf(" %s",arvore);
        scanf(" %d",&n);
        
        DELETE_AR(lista,arvore,n);
    } else if(comando == 11){
        char lista1[40];
        char lista2[40];
        char temp[40];
        char temp2[40];

        scanf(" %s %s %s %s",lista1,temp,lista2,temp2);
        buscaDuplo(lista1,lista2);
    } else if(comando == 12){
        char lista1[40];
        char lista2[40];
        char temp[40];
        char temp2[40];
        char lista3[40];

        scanf(" %s %s %s %s %s",lista1,temp,lista2,temp2,lista3);
        buscaUnico(lista1,lista2,lista3);
    }else if(comando == 13){
        char desordenada[40];
        char ordenada[40];
        char Stipo[20];

        scanf(" %s %s %s",desordenada,Stipo,ordenada);
        ordenacao(desordenada,ordenada,Stipo);
    }else if(comando == 14){
        char lista1[40];
        char lista2[40];
        char temp[40];
        char temp2[40];

        scanf(" %s %s %s %s",lista1,temp,lista2,temp);
        ordintercala(lista1,lista2);
    }else {
        printf("Comando Inválido\n");
    }
}