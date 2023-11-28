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

// arv: ponteiro para a arvore patricia
// cod: chave a ser avaliada
// a: resultado da busca
// k: tamanho da chave (cod)
// a = 1 se chegar a uma folha, faltando comparar com o rotulo do no para concluir que a chave foi encontrada
void busca(ArvPat* arv, int cod, int* a, int k);

//
void insere(ArvPat* arv, int cod);

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

void busca(ArvPat* arv, int cod, int* a, int k) {
    int mask = 1<<arv->rotulo-1;

    if (arv->esquerda == NULL)
    {
        *a = 1;
    }
    else if (k < arv->rotulo)
    {
        *a = 2;
    }
    else if ((cod & mask) == 0 )
    {
        arv = arv->esquerda;
        busca(arv, cod, a, k);
    }
    else
    {
        arv = arv->direita;
        busca(arv, cod, a, k);
    }
    
}

void insere(ArvPat* arv, int cod) {

}