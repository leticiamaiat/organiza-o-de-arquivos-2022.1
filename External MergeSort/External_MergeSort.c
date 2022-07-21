#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

int compare(const void *e1, const void *e2)
{
    return strncmp(((Endereco *)e1)->cep, ((Endereco *)e2)->cep, 8);
}

int main(int argc, char **argv)
{
    FILE *f;
    Endereco *end;
    int qntdArq;
    long posicao, tamPart, tamReg;
    printf("O arquivo sera dividido em quantas partes?\n");
    scanf("%d", &qntdArq);
    f = fopen("cep.dat", "r");
    fseek(f, 0, SEEK_END);
    posicao = ftell(f);
    tamReg = posicao / sizeof(Endereco);
    tamPart = tamReg / qntdArq;
    char tamCep[15];

    rewind(f);

    for (int i = 0; i <= qntdArq; i++)
    {
        end = (Endereco *)malloc(tamPart * sizeof(Endereco));
        fread(end, sizeof(Endereco), tamPart, f);
        FILE *f2 = fopen(tamCep, "wb");
        qsort(end, tamPart, sizeof(Endereco), compare);
        sprintf(tamCep, "Arquivo_%d.dat", i+1);
        fwrite(end, sizeof(Endereco) , tamPart, f2);
        printf("Bloco %d ordenado e separado\n", i);
        fclose(f2);
        free(end);
    }

    
    fclose(f);
}

