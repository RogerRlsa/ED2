#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define TAM_NOME_CLIENTE 100
#define TAM_NOME_ARQ 50

int tamHash = 0;

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
// Imprime uma mensagem de erro caso o cliente cl já exista (cod do cliente igual)
// Retorna 0 para erro e 1 caso contrario
void inserir(FILE *clientes, Cliente *cl);
// Escreve um cliente do arquivo clientes na posicao atual do cursor
void escreveCliente(FILE *clientes, Cliente *cl);
// Busca um Cliente (cod do cliente) no arquivo cliente, segundo a tabela hash no arquivo tabH e atribui a cl
// Retorna 0 para cliente não encontrado, caso exista pelo menos 1 cliente na posição, 1 caso o cliente seja encontrado e 2 para não encontrado e posição não possui cliente
void busca(FILE *clientes, int cod, int *end, int *a);
// Deleta um CLiente, alterando seu status para 1 (liberado)
void deletar(FILE *clientes, int cod);
// Le um cliente do arquivo clientes na posicao atual do cursor
// Retorna 0 para erro e 1 para sucesso
int le(FILE *clientes, Cliente * cl);
// Inicializa o arquivo passado com -1 para todos os n elementos
void hashInit(FILE *clientes, int n);

// Imprime o arquivo hash do arquivo
//void imprimiHash(FILE *in);

// Imprime todos os clientes e seus metadados do arquivo
void imprimiClientes(FILE *in);
// Retorna o tamanho da estrutura Cliente em bytes
int tamanhoMetaCliente();

// Atualiza o final no arquivo de metadados
//void atualizaMetadados(FILE* meta);



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

void inserir(FILE *clientes, Cliente *cl) {
    int end, a, i, j;
    Cliente clTemp;
    busca(clientes, cl->cod, &end, &a);

    if (a != 1) {
        if (end != -1) {
            j = end;
        } else {
            i = 1;
            j = hash(cl->cod);
            
            while (i <= tamHash)
            {
                
                fseek(clientes, j * tamanhoMetaCliente(), SEEK_SET);
                le(clientes, &clTemp);

                if (clTemp.status == 0) {
                    j = (j+1) % tamHash;
                    i++;
                } else {
                    i = tamHash + 2;
                }
            }
            
            if (i==(tamHash+1)) {
                printf("\nInsercao invalida!!! (Overflow)\n");
                return;
            }

            fseek(clientes, hash(cl->cod) * tamanhoMetaCliente(), SEEK_SET);
            le(clientes, &clTemp);

            int temp = clTemp.prox;
            clTemp.prox = j;
            cl->prox = temp;

            fseek(clientes, -tamanhoMetaCliente(), SEEK_CUR);
            escreveCliente(clientes, &clTemp);
        }

        fseek(clientes, j * tamanhoMetaCliente(), SEEK_SET);
        escreveCliente(clientes, cl);

        printf("\nInsercao realizada com sucesso!!!\n");
    } else {
        printf("\nInsercao invalida!!! (Chave ja existe)\n");
    }
}

void escreveCliente(FILE *clientes, Cliente *cl) {
    fwrite(&cl->cod, sizeof(int), 1, clientes);
    //cl->nome ao invés de &cl->nome, pois string já é ponteiro
    fwrite(cl->nome, sizeof(char), sizeof(cl->nome), clientes);
    fwrite(&cl->prox, sizeof(int), 1, clientes);
    fwrite(&cl->status, sizeof(int), 1, clientes);
}

void busca(FILE *clientes, int cod, int *end, int *a) {
    *end = hash(cod);
    *a = 0;
    int j = -1;
    while (*a == 0)
    {
        Cliente cl;
        fseek(clientes, (*end) * tamanhoMetaCliente(), SEEK_SET);
        le(clientes, &cl);

        if (cl.status == 1) {
            j = *end;
        }
        if (cl.cod == cod && cl.status == 0) {
            *a = 1; // Cliente encontrado
        } else {
            if (*end == cl.prox) {
                *a = 2; // Cliente não encontrado
                *end = j;
            } else {
                *end = cl.prox;
            }
        }
    }
}

void deletar(FILE *clientes, int cod) {
    int end, a;
    busca(clientes, cod, &end, &a);

    if (a == 1) {
        Cliente cl;
        fseek(clientes, end * tamanhoMetaCliente(), SEEK_SET);
        le(clientes, &cl);

        cl.status = 1;

        fseek(clientes, -tamanhoMetaCliente(), SEEK_CUR);
        escreveCliente(clientes, &cl);
        printf("\nExclusao realizada com sucesso!!!\n");
    } else {
        printf("\nExclusao invalida!!! (Chave nao encontrada)\n");
    }
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

void hashInit(FILE *clientes, int n) {
    Cliente *init = cliente(-1,"z");
    init->status = 1;

    tamHash = n;

    for (int i = 0; i < n; i++)
    {
        init->prox = i;
        escreveCliente(clientes, init);
    }

    free(init);
}
/*
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
*/
void imprimiClientes(FILE *in) {
    Cliente clienteTeste;
    rewind(in);
    for (int i = 0; i < tamHash; i++)
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
/*
void atualizaMetadados(FILE* meta) {
    //printf("%d", final);
    fseek(meta, sizeof(int), SEEK_SET);
    fwrite(&final, sizeof(int), 1, meta);
}
*/