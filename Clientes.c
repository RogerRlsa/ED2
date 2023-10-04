#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define TAM_NOME_CLIENTE 100
#define TAM_NOME_ARQ 50

int tamHash = 0;
int final = 0;
int endLivre = -1;

// Estrutura dados cliente + metadados
typedef struct cliente
{
    int cod;
    char nome[TAM_NOME_CLIENTE];
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
// Deleta um CLiente, alterando seu status para 1 (liberado)
int delete(FILE *clientes, FILE *tabH, int cod);
// Le um cliente do arquivo clientes na posicao atual do cursor
// Retorna 0 para erro e 1 para sucesso
int le(FILE *clientes, Cliente * cl);
// Inicializa o arquivo passado com -1 para todos os n elementos
void hashInit(FILE *hash, int n, int fim);
// Imprime o arquivo hash do arquivo
void imprimiHash(FILE *in);
// Imprime todos os clientes e seus metadados do arquivo
void imprimiClientes(FILE *in);
// Retorna o tamanho da estrutura Cliente em bytes
int tamanhoMetaCliente();
// Atualiza o final no arquivo de metadados
void atualizaMetadados(FILE* meta);



int hash(int cod) {
    return cod%tamHash;
}

void imprime(Cliente *cl) {
    printf("\n**********************************");
    printf("\nCodigo ");
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
    cl->status = 0;
    return cl;
}

void salva(FILE *clientes, FILE *tabH, Cliente *cl) {
    int end = hash(cl->cod);
    Cliente temp;
    
    switch (busca(clientes, tabH, cl->cod, &temp))
    {
    case 0: // Percorre a lista até o fim e não acha o cliente
        fseek(clientes, -tamanhoMetaCliente(), SEEK_CUR);
        if (endLivre == -1) {
            temp.prox = final;
            escreveCliente(clientes, &temp);
        }
        break;
    case 1: // Encontra o cliente
        printf("ERRO, cliente ja existe!!");
        return ;
    case 2: // Lista vazia
        fseek(tabH, end * sizeof(int), SEEK_SET);
        fwrite(&final, sizeof(int), 1, tabH);
        break;
    }
    
    if (endLivre == -1)
    {
        fseek(clientes, 0, SEEK_END);
        final++;
    } else {
        fseek(clientes, endLivre * tamanhoMetaCliente(), SEEK_SET);
    }
    escreveCliente(clientes, cl);
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
        printf("\nCliente nao encontrado, posicao vazia.\n");
        return 2; // Lista vazia 
    }
    
    fseek(clientes, endCl * tamanhoMetaCliente(), SEEK_SET);
    le(clientes, cl);

    endLivre = -1;
    if (endLivre == -1 && cl->status == 1) endLivre = endCl;

    while(cl->prox != -1 && (cl->cod != cod || cl->status == 1)){

        if (endLivre == -1 && cl->status == 1) endLivre = endCl;
        endCl = cl->prox;
        fseek(clientes, cl->prox * tamanhoMetaCliente(), SEEK_SET);
        le(clientes, cl);
    }
    
    if (cl->cod == cod && cl->status == 0) return 1; // Encontra o cliente
    cl->cod = -1;
    strcpy(cl->nome, "Not Found");
    printf("\nCliente nao encontrado.\n");
    return 0; // Percorre a lista até o fim e não acha o cliente
}

int delete(FILE *clientes, FILE *tabH, int cod) {
    Cliente cl;
    switch (busca(clientes, tabH, cod, &cl))
    {
    case 0: // Percorre a lista até o fim e não acha o cliente
        printf("ERRO, cliente nao encontrado!!");
        break;
    case 1: // Encontra o cliente
        cl.status = 1;
        fseek(clientes, -tamanhoMetaCliente(), SEEK_CUR);
        escreveCliente(clientes, &cl);
        return 1;
    case 2: // Lista vazia 
        printf("ERRO, cliente nao encontrado!!");
        break;
    }
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

void hashInit(FILE *hash, int n, int fim) {
    int init = -1;

    tamHash = n;
    final = fim;
    
    if (fim == 0)
    {
        for (int i = 0; i < n; i++)
        {
            fwrite(&init, sizeof(int), 1, hash);
        }
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
    printf("\n");
}

void imprimiClientes(FILE *in) {
    Cliente clienteTeste;
    rewind(in);
    for (int i = 0; i < final; i++)
    {
        le(in, &clienteTeste);
        imprime(&clienteTeste);
    }
    printf("\n");
}

int tamanhoMetaCliente() {
    return (sizeof(int) * 3)  //cod, prox, status
            + sizeof(char) * TAM_NOME_CLIENTE; //nome
}

void atualizaMetadados(FILE* meta) {
    //printf("%d", final);
    fseek(meta, sizeof(int), SEEK_SET);
    fwrite(&final, sizeof(int), 1, meta);
}
