#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "clientes.c"

int *clientesNaHash;
int qtdClientesNaHash = 0;

void initClienteNaHash(int n) {
    for (int i=0; i<n; i++) {
        clientesNaHash[i] = -1;
    }
}

void addClientesNaHash(int codigo, int n) {
    for (int i = 0; i < n; i++)
    {
        if (clientesNaHash[i] == -1) {
            clientesNaHash[i] = codigo;
            if (i > qtdClientesNaHash) qtdClientesNaHash++;
            return ;
        }
    }
}

FILE* abrirArq(char* nome) {
    FILE* arq;
    if ((arq = fopen(nome, "r+b")) == NULL) {
        printf("\nErro ao abrir arquivo: %s\n", nome);
        exit(1);
    }
    return arq;
}

int inserirRandom (FILE *tab, int n) {
    int aleatorio = rand();
    Cliente cl = {.cod = aleatorio, .nome = "nome"};
    inserir(tab, &cl);

    addClientesNaHash(aleatorio, n);
    return qtdDeColisoes;
}

void deletarRandom (FILE *tab) {
    //printf("\n1\n");
    //printf(", %d", qtdClientesNaHash);
    int aleatorio = rand() % (qtdClientesNaHash);
    //printf("\n2\n");
    deletar(tab, clientesNaHash[aleatorio]);
    //printf("\n3\n");
    clientesNaHash[aleatorio] = -1;
    //if (aleatorio == qtdClientesNaHash) qtdClientesNaHash--;
}

double buscarRandom (FILE *tab) {
    int aleatorio = rand() % qtdClientesNaHash;
    int end, a, colisoes;
    
    clock_t comeco = clock();
    busca(tab, aleatorio, &end, &a, &colisoes);
    clock_t final = clock();

    return (final-comeco) / CLOCKS_PER_SEC;
}

int main(int argc, char const *argv[])
{
    
    FILE *clientes = abrirArq("tabela.dat");
    int n = 10;

    for (int i=0; i<3; i++) {
        tipoHash = i;               // Inicia tipo da hash como i
        n *= 10;                    // Tamanho da hash 10 vezes maior
        hashInit(clientes, n);      // Inicia a hash e altera seu tamanho
        clientesNaHash = (int*) malloc(n*sizeof(int));
        initClienteNaHash(n);
        
        printf("\nHash de tamanho: %d.\n", n);

        for(int j=0; j<3; j++) {
            float cargaAprox = (j==0)? 0.7:(j==1)? 0.8:0.9;
            printf("\n\tFator de carga aproximado: %f.\n", cargaAprox);
            
            while (fatorDeCarga() < cargaAprox)
            {
                inserirRandom(clientes, n);
            }
            //printf("\n%d\n", qtdClientesNaHash);
            //imprimiClientes(clientes);
            int colisoes = 0, insersoes = 0;
            
            for (int k=0; k<1400; k++)
            {
                if (fatorDeCarga() >= cargaAprox) {
                    
                    deletarRandom(clientes);
                    
                }else {
                    int tempColisoes= inserirRandom(clientes, n);
                    if (tempColisoes != -1)
                    {
                        colisoes += tempColisoes;
                        insersoes++;
                    }
                    
                }
            }
            
            double tempoGasto = 0.0;
            for (int b = 0; b < n/2; b++)
            {
                tempoGasto += buscarRandom(clientes);
            }
            double mediaTempoGasto = tempoGasto / (n/2);
            double mediaColisoes = colisoes / insersoes;
            
            printf("\n\t\tMedia de colisoes (%d exemplos): %lf.\n", insersoes, mediaColisoes);
            printf("\n\t\tTempo medio de CPU para busca (%d exemplos): %lf.\n", (n/2), mediaTempoGasto);
        }
        free(clientesNaHash);
    }
    fclose(clientes);

    return 0;
}
