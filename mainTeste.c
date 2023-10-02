#include <stdio.h>
#include <stdlib.h>
#include "Clientes.c"

int main(void) {

    FILE* tabHash;
    FILE* clientes;
    FILE* control;

    if ((control = fopen("meta.dat", "r+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    if ((tabHash = fopen("tabHash.dat", "r+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    if ((clientes = fopen("clientes.dat", "r+b")) == NULL) {
        printf("Erro ao abrir arquivo 2\n");
        exit(1);
    }
    hashInit(tabHash, 7);

    Cliente* cl = cliente(49, "Teste");
    salva(clientes,tabHash,cl);
    printf("\n----------------\n");
    //delete(clientes, tabHash, 49);
    printf("\n----------------\n");
/*
    Cliente* cl1 = cliente(3, "Algo");
    salva(clientes, tabHash, cl1);
    printf("\n----------------\n");
*/
    Cliente* cl2 = cliente(7, "Sla");
    salva(clientes, tabHash, cl2);
    printf("\n----------------\n");

    rewind(clientes);
    rewind(tabHash);

    imprimiHash(tabHash);
    
    imprimiClientes(clientes);

    //Cliente a;
    //int cod = -1;
    //scanf(" %d", &cod);
    //printf("\nBusca do cliente de cod %d: \n%s\n", cod, (busca(clientes,tabHash,cod,&a)==1)? "True": "False");

    free(cl);
    //free(cl1);
    //free(cl2);

    fclose(tabHash);
    fclose(clientes);
    return 0;
}