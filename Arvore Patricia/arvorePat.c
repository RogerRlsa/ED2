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

// a = 1 se chegar a uma folha, faltando comparar com o rotulo do no para concluir que a chave foi encontrada
void busca(ArvPat* arv, int info, int* a, int k);

//
void insere(ArvPat* arv, int info);

//
void delete(ArvPat*, int info);

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

void busca(ArvPat* arv, int info, int* a, int k) {
    int mask = 1<<arv->rotulo;

    if (arv->esquerda == NULL)
    {
        *a = 1;
    }
    else if (k < arv->rotulo)
    {
        *a = 2;
    }
    else if ((info & mask) == 0 )
    {
        arv = arv->esquerda;
        busca(arv, info, a, k);
    }
    else
    {
        arv = arv->direita;
        busca(arv, info, a, k);
    }
    
}
