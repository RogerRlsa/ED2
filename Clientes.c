#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

int tamHash;
int final = 0;

typedef struct cliente
{
    int cod;
    char nome[100];
    int prox;
    int status;
} Cliente;

int tamanhoDataCliente();
void escreveCliente(FILE *clientes, Cliente *cl);
int busca(FILE *clientes, FILE *tabH, int cod, Cliente* cl);
int le(FILE *clientes, Cliente * cl);



int hash(int cod) {
    return cod%tamHash;
}

// Imprime Cliente
void imprime(Cliente *cl) {
    printf("\n**********************************");
    printf("\nCódigo ");
    printf("%d", cl->cod);
    printf("\nNome: ");
    printf("%s", cl->nome);
    printf("\nProx: ");
    printf("%d", cl->prox);
    printf("\nStatus: ");
    printf("%d", cl->status);

}

// Cria cliente. Lembrar de usar free(cl)
Cliente *cliente(int cod, char *nome) {
    Cliente *cl = (Cliente *) malloc(sizeof(Cliente));
    //inicializa espaço de memória com ZEROS
    if (cl) memset(cl, 0, sizeof(Cliente));
    //copia valores para os campos de cl
    cl->cod = cod;
    strcpy(cl->nome, nome);
    cl->prox = -1;
    cl->status = 1;
    return cl;
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(FILE *clientes, FILE *tabH, Cliente *cl) {
    int end = hash(cl->cod);
    Cliente temp;
    
    switch (busca(clientes, tabH, cl->cod, &temp))
    {
    case 0: // Percorre a lista até o fim e não acha o cliente
        fseek(clientes, -tamanhoDataCliente(), SEEK_CUR);
        temp.prox = final;
        escreveCliente(clientes, &temp);
        break;
    
    case 1: // Encontra o cliente
        printf("ERRO, cliente já existe!!");
        return ;
    case 2: // Lista vazia        
        fseek(tabH, end * sizeof(int), SEEK_SET);
        fwrite(&final, sizeof(int), 1, tabH);
        break;
    }
    fseek(clientes, 0, SEEK_END);
    escreveCliente(clientes, cl);

    final++;
}

void escreveCliente(FILE *clientes, Cliente *cl) {
    fwrite(&cl->cod, sizeof(int), 1, clientes);
    //cl->nome ao invés de &cl->nome, pois string já é ponteiro
    fwrite(cl->nome, sizeof(char), sizeof(cl->nome), clientes);
    fwrite(&cl->prox, sizeof(int), 1, clientes);
    fwrite(&cl->status, sizeof(int), 1, clientes);
}

int busca(FILE *clientes, FILE *tabH, int cod, Cliente* cl) {
    int end = hash(cod);
    fseek(tabH, end * sizeof(int), SEEK_SET);

    int endCl = -1;
    fread(&endCl, sizeof(int), 1, tabH);

    if (endCl == -1){
        //printf("Cliente não encontrado!!");
        return 2;
    }
    
    fseek(clientes, endCl * tamanhoDataCliente(), SEEK_SET);
    le(clientes, cl);

    while(cl->prox != -1 && cl->cod != cod){
        fseek(clientes, cl->prox * tamanhoDataCliente(), SEEK_SET);
        le(clientes, cl);
    }
    
    if (cl->cod == cod) return 1;

    return 0;
}
/*
void salvaHash(Cliente *cl, FILE *out) {
    fwrite(&cl->cod, sizeof(int), 1, out);
    //cl->nome ao invés de &func->nome, pois string já é ponteiro
    fwrite(cl->nome, sizeof(char), sizeof(cl->nome), out);
}*/

// Le um cliente do arquivo clientes na posicao atual do cursor
// Retorna 0 para erro e 1 para sucesso
int le(FILE *clientes, Cliente *cl) {
    if (0 >= fread(&cl->cod, sizeof(int), 1, clientes)) {
	free(cl);
	return 0;
    }
    fread(cl->nome, sizeof(char), sizeof(cl->nome), clientes);
    fread(&cl->prox, sizeof(int), 1, clientes);
    fread(&cl->status, sizeof(int), 1, clientes);
    return 1;
}

void hashInit(FILE *out, int n) {
    int init = -1;
    tamHash = n;
    for (int i = 0; i < n; i++)
    {
        fwrite(&init, sizeof(int), 1, out);
    }
    
}

void imprimiHash(FILE *in) {
    int val=0;
    rewind(in);
    printf("Hash: \n");
    for (int i = 0; i < tamHash; i++)
    {
        printf("%5d |", i);
    }
    printf("\n");
    for (int i = 0; i < tamHash; i++)
    {
        fread(&val, sizeof(int), 1, in);
        printf("%5d |", val);
    }
    
}

void imprimiClientes(FILE *in) {
    Cliente clienteTeste;
    for (int i = 0; i < final; i++)
    {
        le(in, &clienteTeste);
        imprime(&clienteTeste);
    }
}

/*
// Retorna tamanho do cliente em bytes
int tamanhoCliente() {
    return sizeof(int)  //cod
            + sizeof(char) * 50; //nome
}*/

int tamanhoDataCliente() {
    return (sizeof(int) * 3)  //cod, prox, status
            + sizeof(char) * 100; //nome
}
