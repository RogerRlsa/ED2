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

// Cria um nó da arvore
ArvPat* arvPat();

//
void liberaArvPat(ArvPat* arv);

// retorna 1 se r for no folha e 0 caso contrário 
int eFolha(ArvPat* r);

// W.I.P
int tamanhoMaiorPrefComum(int cod, int chave);

// W.I.P
int tamanhoChave(int chave);

// W.I.P
void selecDescendente(ArvPat** no);

// arv: ponteiro para a arvore patricia
// cod: chave a ser avaliada
// a: resultado da busca
// k: tamanho da chave (cod)
// a = 1 se chegar a uma folha, faltando comparar com o rotulo do no para concluir que a chave foi encontrada
// a = 2 se o tamanho da chave (cod) < rotulo do nó corrente
// atualiza result com o valor do rótulo do nó folha, caso seja alcançado
void busca(ArvPat* arv, int cod, int* a, int k, ArvPat** result);

//
void insere(ArvPat* arv, int cod, int k);

// W.I.P
void insereValida(ArvPat* arv, int cod);

// W.I.P
void delete(ArvPat* arv, int cod);

// W.I.P
void imprimeArvPat(ArvPat* arv);

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

int eFolha(ArvPat* r) {
    return (r->esquerda == NULL)? 1:0;
}

int tamanhoMaiorPrefComum(int cod, int chave) {
    return 0;
}

int tamanhoChave(int chave) {
    return 0;
}

void selecDescendente(ArvPat **no)
{
}

void busca(ArvPat* arv, int cod, int* a, int k, ArvPat** result) {
    int mask = 1<<((arv->rotulo)-1);

    //printf("\nAlcancado: %d\n", arv->rotulo);

    if (arv->esquerda == NULL)
    {
        *a = 1;
        *result = arv;
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
    
    // a -> 1 se busca chegou em nó folha e 2 caso contrário
    // r -> nó alcançado pela busca
    int a;
    ArvPat* r;

    busca(arv, cod, &a, k, &r);

    if (!eFolha(r)) {
        // Selecionar y descendente do no r -> resultado da busca
        selecDescendente(&r);
        // caso r for folha, então y = r
    }
    
    // l -> tamanho do maior prefixo comum entre cod e a chave de y
    int l = tamanhoMaiorPrefComum(cod, r->rotulo);

    // c -> tamanho da chave do no
    int c = tamanhoChave(r->rotulo);

    // inserção inválida
    if (l == k || l == c) {
        printf("\nA chave é inválida, insira uma chave válida!!!\n");
        return ;
    }
    
    insereValida(arv, cod);
}

void insereValida(ArvPat *arv, int cod)
{
}
