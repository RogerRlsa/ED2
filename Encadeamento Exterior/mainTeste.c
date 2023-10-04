#include <stdio.h>
#include <stdlib.h>
#include "Clientes.c"

FILE* abrirArq(char* nome) {
    FILE* arq;
    if ((arq = fopen(nome, "r+b")) == NULL) {
        printf("\nErro ao abrir arquivo: %s\n", nome);
        exit(1);
    }
    return arq;
}

int main(void) {

    FILE* tabHash;
    FILE* clientes;
    FILE* control;
    
    control = abrirArq("meta.dat");

    int tam = 0, fim = 0;
    char arqHash[TAM_NOME_ARQ] = "", arqClientes[TAM_NOME_ARQ] = "";

    if (0 >= fread(&tam, sizeof(int), 1, control)) {
	return 0;
    }
    
    fread(&fim, sizeof(int), 1, control);
    fread(arqHash, sizeof(char), sizeof(arqHash), control);
    fread(arqClientes, sizeof(char), sizeof(arqClientes), control);

    //printf("%s\n%s", arqHash, arqClientes);
    printf("\n");

    tabHash = abrirArq(arqHash);
    clientes = abrirArq(arqClientes);

    hashInit(tabHash, tam, fim);

    // Hash inicializada
    Cliente* cl;
    Cliente clBusca;
    int sair = 0;
    do
    {
        int escolha = -1;
        unsigned int cod;
        char nome[TAM_NOME_CLIENTE];
        printf("\nInforme a acao (0 -> encerrar loop, 1 -> guardar cliente, 2 -> buscar cliente, 3 -> deletar cliente):\n");
        scanf("%d", &escolha);
        getchar();
        switch (escolha)
        {
        case 0:
            sair = 1;
            break;
        case 1:
            printf("\nInforme o codigo do cliente e o nome que deseja guardar: \n");
            scanf("%u %s", &cod, nome);
            cl = cliente(cod, nome);
            salva(clientes, tabHash, cl);
            printf("\nCliente salvo.\n");
            free(cl);
            break;
        case 2:
            printf("\nInforme o codigo do cliente que deseja buscar: \n");
            scanf("%u", &cod);
            if (busca(clientes, tabHash, cod, &clBusca) == 1) {
                printf("\nDados do cliente:\nCodigo: %u.\nNome: %s.\n", clBusca.cod, clBusca.nome);
            }
            break;
        case 3:
            printf("\nInforme o codigo do cliente que deseja deletar: \n");
            scanf("%d", &cod);
            if (delete(clientes, tabHash, cod) == 1) printf("\nCliente deletado com sucesso.\n");
            break;
        }
    } while (!sair);
    printf("\n");

    //Cliente* cl = cliente(49, "Teste");
    //salva(clientes,tabHash,cl);
    //printf("\n----------------\n");
    //delete(clientes, tabHash, 49);
    //printf("\n----------------\n");
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

    //free(cl);
    //free(cl1);
    //free(cl2);

    atualizaMetadados(control);

    fclose(control);
    fclose(tabHash);
    fclose(clientes);

    return 0;
}
