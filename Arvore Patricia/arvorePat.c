#include <stdlib.h>

#define LAMBDA -1         

//-------------------------------------------

//
typedef struct arvorePat
{
    int rotulo;
    struct arvorePat* direita;
    struct arvorePat* esquerda;
    
} ArvPat;

//-------------------------------------------

//
ArvPat* arvPat();

//
void liberaArvPat(ArvPat* arv);

// arv: ponteiro para a arvore patricia
// cod: chave a ser avaliada
// a: resultado da busca
// k: tamanho da chave (cod)
// a = 1 se chegar a uma folha, faltando comparar com o rotulo do no para concluir que a chave foi encontrada
// atualiza result com o valor do rótulo do nó folha, caso seja alcançado
void busca(ArvPat* arv, int cod, int* a, int k, int* result);

//
void insere(ArvPat* arv, int cod, int k);

//
void delete(ArvPat*, int cod);

//
void imprimeArvPat(ArvPat*);

//-------------------------------------------

ArvPat* arvPat() {
    ArvPat* new = (ArvPat*) malloc(sizeof(ArvPat));
    new->rotulo = LAMBDA;
    new->direita = NULL;
    new->esquerda = NULL;
    return new;
}

void liberaArvPat(ArvPat* arv) {
    
    if (arv->esquerda != NULL)  liberaArvPat(arv->esquerda);
    if (arv->direita != NULL)   liberaArvPat(arv->direita);

    //printf("\nLiberado: %d\n", arv->rotulo);

    free(arv);
}

void busca(ArvPat* arv, int cod, int* a, int k, int* result) {
    int mask = 1<<((arv->rotulo)-1);

    //printf("\nAlcancado: %d\n", arv->rotulo);

    if (arv->esquerda == NULL)
    {
        *a = 1;
        *result = arv->rotulo;
    }
    else if (k < arv->rotulo)
    {
        *a = 2;
    }
    else if ((cod & mask) == 0 )
    {
        arv = arv->esquerda;
        busca(arv, cod, a, k, result);
    }
    else
    {
        arv = arv->direita;
        busca(arv, cod, a, k, result);
    }
}

void insere(ArvPat* arv, int cod, int k) {
    int a, r = LAMBDA;
    busca(arv, cod, &a, k, &r);

    
}

