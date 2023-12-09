#include <stdio.h>
#include "arvorePat.c"

int main(void)
{
    int parar = 0;
    while (!parar)
    {
        int acao = 0;
        ArvPat* arv = arvPat();
        printf("\nImforme a ação desejada (0 -> encerrar, 1-> , 2-> , 3->):\n");
        scanf(" %d", &acao);
        switch (acao)
        {
        case 0:
            parar = 1;
            break;
        case 1:
            if (arv!=)
            {
                arv = arvPat();
            }
            break;
        case 2:
            imprimeArvPat();
            break;
        default:
            printf("\nNúmero de acao invalido!!! Por favor impforme uma acao valida.\n");
            break;
        }
    }
    

    return 0;
}