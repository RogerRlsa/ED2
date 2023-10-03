#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME_ARQ 50

int main(int argc, char* argv[]) {

    FILE* control;

    if ((control = fopen("meta.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    int tam = atoi(argv[1]);
    int final = 0;
    
    char hash[TAM_NOME_ARQ];
    strcpy(hash,argv[2]);

    char cliente[TAM_NOME_ARQ];
    strcpy(cliente,argv[3]);

    strcat(hash,".dat");
    strcat(cliente,".dat");
    //printf("%s\n%s", hash, cliente);

    fwrite(&tam, sizeof(int), 1, control);
    fwrite(&final, sizeof(int), 1, control);
    fwrite(hash, sizeof(char), sizeof(hash), control);
    fwrite(cliente, sizeof(char), sizeof(cliente), control);

    FILE* tabHash;
    FILE* tabCliente;

    if ((tabHash = fopen(hash, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    if ((tabCliente = fopen(cliente, "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    fclose(control);
    fclose(tabHash);
    fclose(tabCliente);

    return 0;
}