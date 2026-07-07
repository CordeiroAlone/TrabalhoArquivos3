#ifndef AR_DELETE_H
#define AR_DELETE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "CABECALHO.h"
#include "DELETE_ONE.h"
#include "../TAD/buscar.h" 
#include "../TAD/util.h"

void lerlinha_ar(estacao* est);

// Realiza a deleção de registros de estações e seus índices associados.
// Essa função abre os arquivos de dados e de índice, valida o status de consistência de ambos, e realiza deleção várias vezes.
void DELETE_AR(char *arquivoBIN, char *arquivoBIN_ar, int vezes);

// Verifica e atualiza a raiz da árvore-B caso ela fique sem chaves após deleções.
// Se a raiz atual ficar vazia (0 chaves) devido a operações de concatenação, a função a marca como removida (adicionando-a à pilha de nós livres no topo do cabeçalho) e promove seu primeiro filho (se existir) como a nova raiz da árvore.
void atualizar_raiz_se_vazia(FILE* indice_arq, CabecalhoArvore* cabecalho_ar);

// Itera sequencialmente pelo arquivo de dados e deleta os registros que satisfazem as condições de busca.
// Para cada registro compatível encontrado, a função aciona a remoção de sua respectiva chave na Árvore-B e atualiza a estrutura do índice.
// Essa função é análoga a função iterar_estacao feita no trabalho introdutório, com a diferença de servir somente para deleção e deletar na árvore após conseguir o código da estação.
int deletar_sequencial(estacao* base, FILE* arquivo, FILE* arquivo_ar, CabecalhoArvore* cabecalho_ar);

#endif