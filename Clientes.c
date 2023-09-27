#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define TAM_NOME 100

int tamHash = 0;
int final = 0;

// Estrutura dados cliente + metadados
typedef struct cliente
{
    int cod;
    char nome[TAM_NOME];
    int prox;
    int status;
} Cliente;

// Retorna a posição do cod, segundo a função hash (cod mod n)
int hash(int cod);
// Imprime o metadado do Cliente
void imprime(Cliente *cl);
// Cria um metadado Cliente
// Lembrar de usar free(cliente)
Cliente *cliente(int cod, char *nome);
// Guarda um Cliente cl no arquivo cliente e atualiza o arquivo tabH quando necessário
// Imprime uma mensagem de Erro caso o cliente cl já exista (cod do cliente igual)
void salva(FILE *clientes, FILE *tabH, Cliente *cl);
// Escreve um cliente do arquivo clientes na posicao atual do cursor
void escreveCliente(FILE *clientes, Cliente *cl);
// Busca um Cliente (cod do cliente) no arquivo cliente, segundo a tabela hash no arquivo tabH e atribui a cl
// Retorna 0 para cliente não encontrado, caso exista pelo menos 1 cliente na posição, 1 caso o cliente seja encontrado e 2 para não encontrado e posição não possui cliente
int busca(FILE *clientes, FILE *tabH, int cod, Cliente* cl);
// Le um cliente do arquivo clientes na posicao atual do cursor
// Retorna 0 para erro e 1 para sucesso
int le(FILE *clientes, Cliente * cl);
// Inicializa o arquivo passado com -1 para todos os n elementos
void hashInit(FILE *hash, int n);
// Imprime o arquivo hash do arquivo
void imprimiHash(FILE *in);
// Imprime todos os clientes e seus metadados do arquivo
void imprimiClientes(FILE *in);
// Retorna o tamanho da estrutura Cliente em bytes
int tamanhoMetaCliente();



int hash(int cod) {
    return cod%tamHash;
}

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

void salva(FILE *clientes, FILE *tabH, Cliente *cl) {
    int end = hash(cl->cod);
    Cliente temp;
    
    switch (busca(clientes, tabH, cl->cod, &temp))
    {
    case 0: // Percorre a lista até o fim e não acha o cliente
        fseek(clientes, -tamanhoMetaCliente(), SEEK_CUR);
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
    
    fseek(clientes, endCl * tamanhoMetaCliente(), SEEK_SET);
    le(clientes, cl);

    while(cl->prox != -1 && cl->cod != cod){
        fseek(clientes, cl->prox * tamanhoMetaCliente(), SEEK_SET);
        le(clientes, cl);
    }
    
    if (cl->cod == cod) return 1;

    return 0;
}

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

void hashInit(FILE *hash, int n) {
    int init = -1;
    tamHash = n;
    for (int i = 0; i < n; i++)
    {
        fwrite(&init, sizeof(int), 1, hash);
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

int tamanhoMetaCliente() {
    return (sizeof(int) * 3)  //cod, prox, status
            + sizeof(char) * TAM_NOME; //nome
}
