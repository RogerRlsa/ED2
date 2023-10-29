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
    int aleatorio = rand(), i=0;
    while (clientesNaHash[i] != -1 && clientesNaHash[i] < qtdClientesNaHash) {
        if(clientesNaHash[i] == aleatorio) {
            aleatorio = rand();
            i=0;
        }

        i++;
    }

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
    clientesNaHash[aleatorio] = clientesNaHash[qtdClientesNaHash];
    clientesNaHash[qtdClientesNaHash] = -1;
    qtdClientesNaHash--;
    //if (aleatorio == qtdClientesNaHash) qtdClientesNaHash--;
}

double buscarRandom (FILE *tab) {
    int aleatorio = (rand() % qtdClientesNaHash), i=0;

    
    while (clientesNaHash[aleatorio] == -1 || i > 500) {
        aleatorio = (rand() % qtdClientesNaHash); 
        i++;
    }

    int end, a, colisoes;
    
    clock_t comeco = clock();
    busca(tab, clientesNaHash[aleatorio], &end, &a, &colisoes);
    clock_t final = clock();
    //printf("%d",a);
    return (a==1) ? (double) (final-comeco) / (double) CLOCKS_PER_SEC : -1.0;
}

int main(int argc, char const *argv[])
{
    
    FILE *clientes = abrirArq("tabela.dat");
    
    for (int t = 0; t<3; t++) {
        tipoHash = t;               // Inicia tipo da hash como t
        int n = 1000;
        printf("\nTipo da Hash: %d.\n", t);
        for (int i=0; i<3; i++) {
            hashInit(clientes, n);      // Inicia a hash e altera seu tamanho
            clientesNaHash = (int*) malloc(n*sizeof(int));
            qtdClientesNaHash=0;
            initClienteNaHash(n);
            //printf("\n%f\n", fatorDeCarga());
            
            printf("\nHash de tamanho: %d.\n", n);

            for(int j=0; j<3; j++) {
                
                float cargaAprox = (j==0)? 0.7:(j==1)? 0.8:0.9;
                printf("\n\tFator de carga aproximado: %f.\n", cargaAprox);
                
                while (fatorDeCarga() < cargaAprox)
                {
                    inserirRandom(clientes, n);
                    //printf("\n%f\n", fatorDeCarga());
                }
                //printf("\n%d\n", qtdClientesNaHash);
                //imprimiClientes(clientes);
                //printf("\n%f\n", fatorDeCarga());
                int colisoes = 0, insersoes = 0;
                //return 0;
                for (int k=0; k<n/3; k++)
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
                double tempTempoGasto = 0.0;
                int buscas = 0;
                for (int b = 0; b < n/2; b++)
                {
                    tempTempoGasto = buscarRandom(clientes);
                    if (tempTempoGasto>0.0)
                    {
                        tempoGasto += tempTempoGasto;
                        buscas++;
                    } else {
                        b--;
                    }
                    
                }
                double mediaTempoGasto = tempoGasto / (double) buscas;
                double mediaColisoes = (double) colisoes / (double) insersoes;
                
                printf("\n\t\tMedia de colisoes (%d exemplos): %.lf.\n", insersoes, mediaColisoes);
                printf("\n\t\tTempo medio de CPU para busca (%d exemplos): %.10lf.\n", buscas, mediaTempoGasto);
            }
            n += 2000;                  // Tamanho da hash + 2000
            free(clientesNaHash);
        }
    }
    fclose(clientes);

    return 0;
}
