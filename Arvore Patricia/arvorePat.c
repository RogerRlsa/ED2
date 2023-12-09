#include <stdio.h>
#include <stdlib.h>

#define LAMBDA -1         

/*
    NOTE: 
        1 - a chave é um valor inteiro
        2 - a chave 0 (decimal) é interpretada como 0 (binário)
        3 - a chave é lida do bit menos significativo para o mais significativo
*/

//-------------------------------------------

//
typedef struct rotulo
{
    int r;
    short tamanho;
} Rotulo;


//
typedef struct arvorePat
{
    struct arvorePat* pai;    
    Rotulo chave;
    struct arvorePat* direita;
    struct arvorePat* esquerda;
    
} ArvPat;

//-------------------------------------------

// Cria um nó da arvore
ArvPat* arvPat();

// libera todos so nós da árvore patrícia
void liberaArvPat(ArvPat* arv);

// retorna 1 se r for no folha e 0 caso contrário 
int eFolha(ArvPat* r);

// 
short tamanhoMaiorPrefComum(Rotulo* cod, Rotulo* chave);

// retorna o tamanho da chave
short tamanhoChave(Rotulo* chave);

// 
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
ArvPat* insere(ArvPat* arv, int cod, short k);

// 
ArvPat* insereValida(ArvPat* arv, ArvPat* y, Rotulo* cod, short l);

//
void determinarNoDeInsercao(ArvPat* y, short l, ArvPat** noDeInsercao);

// 
int noDireito(ArvPat* no, int caminho);

// 
ArvPat* delete(ArvPat* arv, int cod, short k);

// Imprime a árvore patricia
// Notação:
// raiz < filho esquerdo, filho direito >
// asteristíco (*) após o rotulo do nó indica que este é folha
void imprimeArvPat(ArvPat* arv);

//
void imprimeArvPatRecursiva(ArvPat *arv);

//-------------------------------------------

ArvPat* arvPat() {
    ArvPat* no = (ArvPat*) malloc(sizeof(ArvPat));
    no->chave.r = LAMBDA;
    no->chave.tamanho = LAMBDA;
    no->pai = NULL;
    no->direita = NULL;
    no->esquerda = NULL;
    return no;
}

void liberaArvPat(ArvPat* arv) {
    
    if (arv->esquerda != NULL) {
        liberaArvPat(arv->esquerda);
        liberaArvPat(arv->direita);
    }

    //printf("\nLiberado: %d\n", arv->rotulo);

    free(arv);
}

int eFolha(ArvPat* r) {
    return (r->esquerda == NULL)? 1 : 0;
}

short tamanhoMaiorPrefComum(Rotulo* cod, Rotulo* chave) {
    short tam = 0;
    int mask = 1;

    short tamCod = tamanhoChave(cod);
    short tamChave = tamanhoChave(chave);

    int limite = (tamCod < tamChave)? tamCod : tamChave;

    if (cod->r == chave->r) return limite;

    // xor -> 0 para bits com valores iguais entre cod e chave
    int comparacao = (cod->r ^ chave->r);

    if ((comparacao & mask) == 0)
    {
        tam++;
    } else {
        limite = 0;
    }
    
    for (int i = 1; i <= limite; i++)
    {
        mask = mask << 1;
        if ((comparacao & mask) != 0) {
            tam += i-1;
            break;
        }
    }
    return tam;
}

short tamanhoChave(Rotulo* chave) {
    return chave->tamanho;
}

void selecDescendente(ArvPat **no)
{
    ArvPat* folha = *no;
    while (folha->esquerda != NULL)
    {
        *no = folha->esquerda;
        folha = folha->esquerda;
    }
}

void busca(ArvPat* arv, int cod, int* a, int k, ArvPat** result) {
    if (arv->chave.r == LAMBDA) {
        //printf("\nNão ha nenhum no na arvore!!!\n");
        *result = arv;
        return;
    }

    int mask = 1<<((arv->chave.r)-1);

    //printf("\nAlcancado: %d\n", arv->rotulo);

    if (arv->esquerda == NULL)
    {
        *a = 1;
        *result = arv;
    }
    else if (k < arv->chave.r)
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

ArvPat* insere(ArvPat* arv, int cod, short k) {
    // a -> 1 se busca chegou em nó folha e 2 caso contrário
    // no -> nó alcançado pela busca
    int a;
    ArvPat* no;
    Rotulo in = {cod, k};

    busca(arv, cod, &a, k, &no);
    if (no->chave.r==LAMBDA) {
        no->chave.r = cod;
        no->chave.tamanho = k;

        return no;
    }
    //printf("%d, %d",no->chave.r, cod);
    if (a == 1 && no->chave.r == cod) {
        printf("\nChave ja existe na arvore!!!\n");
        return arv;
    }

    if (!eFolha(no)) {
        // Selecionar descendente do no -> resultado da busca
        selecDescendente(&no);
        // caso no for folha, então retorna o próprio nó
    }
    
    // l -> tamanho do maior prefixo comum entre cod e a chave de no

    short l = tamanhoMaiorPrefComum(&in, &(no->chave));

    // c -> tamanho da chave do no
    short c = tamanhoChave(&(no->chave));

    // inserção inválida
    if (l == k || l == c) {
        printf("\nA chave e invalida, insira uma chave valida!!!\n");
        return arv;
    }
    
    return insereValida(arv, no, &in, l);
}

ArvPat* insereValida(ArvPat* arv, ArvPat* y, Rotulo* cod, short l) {

    ArvPat* retorno = arv;
    ArvPat* noDeInsercao = NULL;
    // Determinar nó de inserção
    determinarNoDeInsercao(y, l, &noDeInsercao);
    
    // Criar dois nós
    ArvPat* v = arvPat();
    v->chave.r = (l + 1);
    v->chave.tamanho = LAMBDA;
    v->pai = noDeInsercao->pai;
    
    if (noDeInsercao->pai != NULL)
    {
        if (noDireito(noDeInsercao, y->chave.r))
        {
            noDeInsercao->pai->direita = v;
        } else {
            noDeInsercao->pai->esquerda = v;
        }
    } else {
        retorno = v;
    }
    noDeInsercao->pai = v;

    ArvPat* w = arvPat();
    w->chave = *cod;
    w->pai = v;

    // "Inserindo" filhos do nó v na árvore
    if ((cod->r & (1<<l))== 0)
    {
        v->esquerda = w;
        v->direita = noDeInsercao;
    } else
    {
        v->direita = w;
        v->esquerda = noDeInsercao;
    }
    return retorno;
}

void determinarNoDeInsercao(ArvPat* y, short l, ArvPat** noDeInsercao) {
    if (y->pai == NULL) {
        // y é folha, se y for a raiz há somente y na árvore, entao o nó de inserção é y 
    } else if (y->pai->chave.r <= l+1) {
        // Se o rotulo do pai de y for menor ou igual que o tamanho do maior prefixo comum + 1, então y é o nó de inserção
    } else {
        y = y->pai;
        // Caso contrário, o no de inserção será o nó mais proximo da raiz, tal que seu rótulo seja maior l+1
        while (y->pai!=NULL && y->pai->chave.r > l+1)
        {
            y = y->pai;
        }
    }
    *noDeInsercao = y;
}

int noDireito(ArvPat* no, int caminho) {

    ArvPat* pai = no->pai;

    int mask = 1 << ((pai->chave.r)-1);

    return ((caminho & mask) == 0)? 0 : 1;
}

ArvPat* delete(ArvPat* arv, int cod, short k) {
    int a;
    ArvPat* result;
    ArvPat* retorno = arv;

    busca(arv, cod, &a, k, &result);
    if (result->chave.r != cod || a == 2) {
        printf("\nChave nao encontrada!!!\n");
        return arv;
    }
    
    // Caso a chave exista na árvore
    if (result->pai == NULL) {
        // Resultado é raiz
        free(result);
        return arvPat();
    } else {
        if (noDireito(result, cod))
        {
            result->pai->esquerda->pai = result->pai->pai;
            if (result->pai->pai != NULL)
            {
                if (noDireito(result->pai, cod))
                {
                    result->pai->pai->direita = result->pai->esquerda;
                } else {
                    result->pai->pai->esquerda = result->pai->esquerda;
                }
            } else {
                retorno = result->pai->esquerda;
            }
        } else {
            result->pai->direita->pai = result->pai->pai;
            if (result->pai->pai != NULL)
            {
                if (noDireito(result->pai, cod))
                {
                    result->pai->pai->direita = result->pai->direita;
                } else {
                    result->pai->pai->esquerda = result->pai->direita;
                }
            } else {
                retorno = result->pai->direita;
            }
        }
    }
    free(result->pai);
    free(result);
    return retorno;
}

void imprimeArvPat(ArvPat *arv) {
    if (arv->chave.r==-1) {
        printf("\nArvore vazia\n");
        return;
    }
    printf("\n");
    imprimeArvPatRecursiva(arv);
    if (arv->esquerda == NULL) printf("*");
    printf("\n");
}

void imprimeArvPatRecursiva(ArvPat *arv) {
    printf("%d", arv->chave.r);
    if (arv->esquerda != NULL)
    {
        printf(" < ");
        imprimeArvPatRecursiva(arv->esquerda);
        if (arv->esquerda->esquerda==NULL) printf("*");
        printf(", ");
        imprimeArvPatRecursiva(arv->direita);
        if (arv->direita->esquerda==NULL) printf("*");
        printf(" > ");
    }
}
