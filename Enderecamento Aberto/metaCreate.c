#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.c"

#define TAM_NOME_ARQ 50

int main(int argc, char* argv[]) {

    FILE* control;

    if ((control = fopen("meta.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    int tam = atoi(argv[1]);

    char cliente[TAM_NOME_ARQ];
    strcpy(cliente,argv[2]);

    strcat(cliente,".dat");

    fwrite(&tam, sizeof(int), 1, control);

    fwrite(cliente, sizeof(char), sizeof(cliente), control);

    FILE* tabCliente;

    if ((tabCliente = fopen(cliente, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    hashInit(tabCliente, tam);

    fclose(control);
    fclose(tabCliente);

    return 0;
}