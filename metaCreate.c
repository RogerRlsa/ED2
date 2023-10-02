#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE* control;

    if ((control = fopen("meta.dat", "r+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    int tam = 0, final = 0;
    char hash[50];
    char cliente[50];

    printf("Informe o tamanho da hash, o nome do arquivo hash e do cliente:\n");
    scanf("%d %s %s", &tam, &hash, &cliente);

    fwrite(&tam, sizeof(int), 1, control);

    return 0;
}