#include <stdio.h>
#include <stdlib.h>
#include "Clientes.c"

int main(void) {

    FILE* tabHash;
    FILE* clientes;
    FILE* control;
    if ((control = fopen("meta.dat", "r+b")) == NULL) {
        printf("Erro ao abrir arquivo meta\n");
        exit(1);
    }

    int tam = 0, fim = 0;
    char arqHash[TAM_NOME_ARQ] = "", arqClientes[TAM_NOME_ARQ] = "";

    if (0 >= fread(&tam, sizeof(int), 1, control)) {
	return 0;
    }
    
    fread(&fim, sizeof(int), 1, control);
    fread(arqHash, sizeof(char), sizeof(arqHash), control);
    fread(arqClientes, sizeof(char), sizeof(arqClientes), control);

    printf("%s\n%s", arqHash, arqClientes);
    printf("\n");

    if ((tabHash = fopen(arqHash, "r+b")) == NULL) {
        printf("Erro ao abrir arquivo hash\n");
        exit(1);
    }

    if ((clientes = fopen(arqClientes, "r+b")) == NULL) {
        printf("Erro ao abrir arquivo clientes\n");
        exit(1);
    }

    hashInit(tabHash, tam, fim);

    //leMetadadosHash(control, "meta.dat", tabHash, clientes);

    Cliente* cl = cliente(49, "Teste");
    salva(clientes,tabHash,cl);
    printf("\n----------------\n");
    //delete(clientes, tabHash, 49);
    printf("\n----------------\n");
/*
    Cliente* cl1 = cliente(3, "Algo");
    salva(clientes, tabHash, cl1);
    printf("\n----------------\n");

    Cliente* cl2 = cliente(7, "Sla");
    salva(clientes, tabHash, cl2);
    delete(clientes,tabHash,7);
    printf("\n----------------\n");
*/

    imprimiHash(tabHash);
    
    imprimiClientes(clientes);

    //Cliente a;
    //int cod = -1;
    //scanf(" %d", &cod);
    //printf("\nBusca do cliente de cod %d: \n%s\n", cod, (busca(clientes,tabHash,cod,&a)==1)? "True": "False");

    free(cl);
    //free(cl1);
    //free(cl2);

    atualizaMetadados(control);

    fclose(control);
    fclose(tabHash);
    fclose(clientes);

    return 0;
}