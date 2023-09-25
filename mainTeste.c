#include <stdio.h>
#include <stdlib.h>
#include "Clientes.c"


int main(void) {

    FILE* tabHash;
    FILE* clientes;

    if ((tabHash = fopen("tabHash.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    if ((clientes = fopen("clientes.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo 2\n");
        exit(1);
    }
    hashInit(tabHash, 7);

    Cliente* cl = cliente(49, "Teste");
    salva(clientes,tabHash,cl);
    Cliente* cl1 = cliente(3, "Algo");

    salva(clientes,tabHash,cl1);

    
    rewind(clientes);
    rewind(tabHash);

    imprimirHash(tabHash);
    
    imprime(le(clientes));
    imprime(le(clientes));

    free(cl);
    free(cl1);

    fclose(tabHash);
    fclose(clientes);
    return 0;
}