#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct cliente
{
    int cod;
    char nome[100]
} Cliente;

typedef struct dataCliente
{
    Cliente cl;
    int prox;
    int status;
} DataCliente;

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
    return cl;
}
/*
// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(Funcionario *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invés de &func->nome, pois string já é ponteiro
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
Funcionario *le(FILE *in) {
    Funcionario *func = (Funcionario *) malloc(sizeof(Funcionario));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
	free(func);
	return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}
*/
// Retorna tamanho do funcionario em bytes
int tamanhoCliente() {
    return sizeof(int)  //cod
            + sizeof(char) * 50; //nome
}

int tamanhoDataCliente() {
    return (sizeof(int) * 3)  //cod, prox, status
            + sizeof(char) * 50; //nome
}
