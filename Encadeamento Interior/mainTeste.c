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

    FILE* clientes;
    FILE* control;
    
    control = abrirArq("meta.dat");

    char arqClientes[TAM_NOME_ARQ] = "";

    if (0 >= fread(&tamHash, sizeof(int), 1, control)) {
	return 0;
    }
    
    fread(arqClientes, sizeof(char), sizeof(arqClientes), control);

    clientes = abrirArq(arqClientes);

    // Hash inicializada
    Cliente* cl;
    Cliente clBusca;
    int sair = 0;
    do
    {
        int escolha = -1;
        unsigned int cod;
        char nome[TAM_NOME_CLIENTE];
        printf("\nInforme a acao (0 -> encerrar loop, 1 -> inserir cliente, 2 -> buscar cliente, 3 -> deletar cliente, 4 -> imprimir tabela):\n");
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
            inserir(clientes, cl);
            free(cl);
            break;
        case 2:
            printf("\nInforme o codigo do cliente que deseja buscar: \n");
            scanf("%u", &cod);
            int end, a;
            busca(clientes, cod, &end, &a);
            if (a == 1) {
                fseek(clientes, end * tamanhoMetaCliente(), SEEK_SET);
                le(clientes, &clBusca);
                printf("\nDados do cliente:\nCodigo: %u.\nNome: %s.\n", clBusca.cod, clBusca.nome);
            } else {
                printf("\nCliente nao encontrado.\n");
            }
            break;
        case 3:
            printf("\nInforme o codigo do cliente que deseja deletar: \n");
            scanf("%d", &cod);
            deletar(clientes, cod);
            break;
        case 4:
            imprimiClientes(clientes);
            break;
        }
    } while (!sair);
    printf("\n");

    fclose(control);
    fclose(clientes);

    return 0;
}