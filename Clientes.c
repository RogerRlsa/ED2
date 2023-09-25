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
void escreveCliente(FILE *out, Cliente *cl);
int busca(FILE *tabH, FILE *clientes, int cod, Cliente* cl);
Cliente *le(FILE *in);



int hash(int cod) {
    return cod%tamHash;
}

// Imprime Cliente
void imprime(Cliente *cl) {
    printf("**********************************************");
    printf("\nCódigo ");
    printf("%d", cl->cod);
    printf("\nNome: ");
    printf("%s", cl->nome);
    printf("\n**********************************************");
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
void salva(FILE *out, FILE *tabH, Cliente *cl) {
    int end = hash(cl->cod);
    Cliente *temp = (Cliente*) malloc(sizeof(Cliente));
    
    switch (busca(tabH, out, cl->cod, temp))
    {
    case 0: // Percorre a lista até o fim e não acha o cliente
        fseek(out, -1 * tamanhoDataCliente(), SEEK_CUR);
        temp->prox = final;
        escreveCliente(out, temp);
        break;
    
    case 1: // Encontra o cliente
        printf("ERRO, cliente já existe!!");
        free(temp);
        return ;
    case 2: // Lista vazia
        fseek(out, 0, SEEK_END);
        escreveCliente(out, cl);
        
        fseek(tabH, end * sizeof(int), SEEK_SET);
        fwrite(&final, sizeof(int), 1, tabH);
        break;
    }


    final++;
    free(temp);
}

void escreveCliente(FILE *out, Cliente *cl) {
    fwrite(&cl->cod, sizeof(int), 1, out);
    //cl->nome ao invés de &cl->nome, pois string já é ponteiro
    fwrite(cl->nome, sizeof(char), sizeof(cl->nome), out);
    fwrite(&cl->prox, sizeof(int), 1, out);
    fwrite(&cl->status, sizeof(int), 1, out);
}

int busca(FILE *tabH, FILE *clientes, int cod, Cliente* cl) {
    int end = hash(cod);
    fseek(tabH, end * sizeof(int), SEEK_SET);

    int endCl = -1;
    fread(&endCl, sizeof(int), 1, tabH);

    if (endCl == -1){
        //printf("Cliente não encontrado!!");
        return 2;
    }
    
    fseek(clientes, endCl * tamanhoDataCliente(), SEEK_SET);
    fread(cl, tamanhoDataCliente(), 1, clientes);

    while(cl->prox != -1 && cl->cod != cod){
        fseek(clientes, cl->prox * tamanhoDataCliente(), SEEK_SET);
        fread(cl, tamanhoDataCliente(), 1, clientes);
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

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
Cliente *le(FILE *in) {
    Cliente *cl = (Cliente *) malloc(sizeof(Cliente));
    if (0 >= fread(&cl->cod, sizeof(int), 1, in)) {
	free(cl);
	return NULL;
    }
    fread(cl->nome, sizeof(char), sizeof(cl->nome), in);
    fread(&cl->prox, sizeof(int), 1, in);
    fread(&cl->status, sizeof(int), 1, in);
    return cl;
}

void hashInit(FILE *out, int n) {
    int init = -1;
    tamHash = n;
    for (int i = 0; i < n; i++)
    {
        fwrite(&init, sizeof(int), 1, out);
    }
    
}

void imprimirHash(FILE *in) {
    int val=0;
    rewind(in);
    for (int i = 0; i < tamHash; i++)
    {
        fread(&val, sizeof(int), 1, in);
        printf("%d ",val);
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
            + sizeof(char) * 50; //nome
}
