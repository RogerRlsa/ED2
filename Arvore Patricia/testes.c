#include <stdio.h>
#include "arvorePat.c"

ArvPat* arvTeste() {

    ArvPat* no3_ = (ArvPat*) malloc(sizeof(ArvPat));
    no3_->chave.r = 31;
    no3_->direita = NULL;
    no3_->esquerda = NULL;

    ArvPat* no3 = (ArvPat*) malloc(sizeof(ArvPat));
    no3->chave.r = 32;
    no3->direita = NULL;
    no3->esquerda = NULL;

    ArvPat* no2_ = (ArvPat*) malloc(sizeof(ArvPat));
    no2_->chave.r = 2;
    no2_->direita = no3_;
    no2_->esquerda = no3;

    ArvPat* no2 = (ArvPat*) malloc(sizeof(ArvPat));
    no2->chave.r = 22;
    no2->direita = NULL;
    no2->esquerda = NULL;

    ArvPat* no1 = (ArvPat*) malloc(sizeof(ArvPat));
    no1->chave.r = 1;
    no1->direita = no2_;
    no1->esquerda = no2;
    
    return no1;
}

int main(){

    //ArvPat* arv = arvTeste();
    //int a;
    //ArvPat* r;
    //busca(arv, 0b110, &a, 3, &r);

    //if (arv != NULL)    printf("\nRotulo do resultado de busca: %d\n", r->chave.r);

    

    //Rotulo rot = {0b0100011, 8};
    //Rotulo rot2 = {0b0100111, 7};
    //printf("%d", (tamanhoMaiorPrefComum(&rot,&rot2)));

    //ArvPat* raiz123 = (arv->direita);
    //selecDescendente(&raiz123);
    //printf("%d", raiz123->chave);

    ArvPat* T = arvPat();

    imprimeArvPat(T);

    T = insere(T, 0b1111, 4);
    imprimeArvPat(T);
    T = insere(T, 0b1110, 4);
    imprimeArvPat(T);
    printf("\n %d \n", T->esquerda->chave.r);
    T = insere(T, 0b1001, 4);
    imprimeArvPat(T);
    T = insere(T, 0b0111, 4);
    imprimeArvPat(T);
    T = insere(T, 0b0011, 4);
    imprimeArvPat(T);
    T = insere(T, 0b1011, 4);

    imprimeArvPat(T);
    //printf("\n %d \n", T->direita->direita->esquerda->pai->chave.r);
    //T = delete(T, 11, 4);
    //printf(tamanhoMaiorPrefComum(T->, &(no->chave)));
    //imprimeArvPat(T);
    


    //liberaArvPat(arv);
    liberaArvPat(T);
    return 0;
}