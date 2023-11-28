#include <stdio.h>
#include "arvorePat.c"

ArvPat* arvTeste() {

    ArvPat* no3_ = (ArvPat*) malloc(sizeof(ArvPat));
    no3_->rotulo = 31;
    no3_->direita = NULL;
    no3_->esquerda = NULL;

    ArvPat* no3 = (ArvPat*) malloc(sizeof(ArvPat));
    no3->rotulo = 32;
    no3->direita = NULL;
    no3->esquerda = NULL;

    ArvPat* no2_ = (ArvPat*) malloc(sizeof(ArvPat));
    no2_->rotulo = 2;
    no2_->direita = no3_;
    no2_->esquerda = no3;

    ArvPat* no2 = (ArvPat*) malloc(sizeof(ArvPat));
    no2->rotulo = 22;
    no2->direita = NULL;
    no2->esquerda = NULL;

    ArvPat* no1 = (ArvPat*) malloc(sizeof(ArvPat));
    no1->rotulo = 1;
    no1->direita = no2_;
    no1->esquerda = no2;
    
    return no1;
}

int main(){
    ArvPat* arv = arvTeste();
    int a, r =-1;
    busca(arv, 0b110, &a, 2, &r);

    if (arv != NULL)    printf("\nRotulo do resultado de busca: %d\n", r);

    liberaArvPat(arv);

    return 0;
}