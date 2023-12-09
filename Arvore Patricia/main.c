#include <stdio.h>
#include "arvorePat.c"

#define ARVORE_EXISTE(c, n, s)     \
            if (!c)                \
            {                      \
                n                  \
            } else {               \
                s                  \
            }                   

int main(void)
{
    int parar = 0;
    ArvPat* arv = arvPat();
    int arvExiste = 1;
    while (!parar)
    {
        int acao = 0;
        printf("\nImforme a acao desejada (0 -> encerrar, 1-> criar a arvore patricia, 2-> imprimir, 3-> inserir, 4-> buscar, 5-> deletar, 6-> liberar a arvore):\n");
        scanf(" %d", &acao);
        switch (acao)
        {
        case 0:
            // Encerra
            parar = 1;
            break;
        case 1:
            // Cria a arvore, caso não exista
            ARVORE_EXISTE(  arvExiste,
                            arv = arvPat();,
                            printf("\nA arvore ja existe!!!\n");
                            )
            break;
        case 2:
            // imprime a arvore caso ela exista
            ARVORE_EXISTE(  arvExiste,
                            printf("\nA arvore nao existe!!!\n");,
                            imprimeArvPat(arv);
                            )
            break;
        case 3:
            // insere na arvore, caso ela exista
            ARVORE_EXISTE(  arvExiste,
                            printf("\nA arvore nao existe!!!\n");,
                            //
                            printf("\nInforme o valor da chave (inteiro) e o tamanho (min.: 1, max.: 32).\n");
                            int chave = 0;
                            int tam = 0;
                            do
                            {
                                scanf("%d %d", &chave, &tam);
                            } while (tam>32 || tam<=0);
                            
                            arv = insere(arv, chave, tam);
                            //
                            )
            break;
        case 4:
            // busca a arvore, caso ela exista
            ARVORE_EXISTE(  arvExiste,
                            printf("\nA arvore nao existe!!!\n");,
                            //
                            printf("\nInforme o valor da chave (inteiro) e o tamanho (min.: 1, max.: 32).\n");
                            int chave = 0;
                            int tam = 0;
                            do
                            {
                                scanf("%d %d", &chave, &tam);
                            } while (tam>32 || tam<=0);
                            
                            int a = 0;
                            ArvPat* r = NULL;
                            busca(arv, chave, &a, tam, &r);
                            if (a==1 && (r->chave.r) == chave)
                            {
                                printf("\nChave: %d, tamanho: %d\n", r->chave.r, r->chave.tamanho);
                            } else {
                                printf("\nA chave nao esta na arvore!!!\n");
                            }
                            //
                            )
            break;
        case 5:
            // deleta a chave da arvore, caso ela exista
            ARVORE_EXISTE(  arvExiste,
                            printf("\nA arvore nao existe!!!\n");,
                            //
                            printf("\nInforme o valor da chave (inteiro) e o tamanho (min.: 1, max.: 32).\n");
                            int chave = 0;
                            int tam = 0;
                            do
                            {
                                scanf("%d %d", &chave, &tam);
                            } while (tam>32 || tam<=0);
                            
                            delete(arv, chave, tam);
                            //
                            )
            break;
        case 6:
            // libera a arvore, caso ela exista
            ARVORE_EXISTE(  arvExiste,
                            printf("\nA arvore nao existe!!!\n");,
                            liberaArvPat(arv);
                            arvExiste = 0;
                            printf("\nArvore liberada!\n");
                            )
            break;
        default:
            printf("\nNumero de acao invalido!!! Por favor imforme uma acao valida.\n");
            break;
        }
    }

    free(arv);
    return 0;
}