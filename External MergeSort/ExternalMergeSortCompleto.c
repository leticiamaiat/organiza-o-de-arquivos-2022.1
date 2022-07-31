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

void merge(char *arq_1, char *arq_2, char *output) // intercala
{
	FILE *a, *b, *saida;
	Endereco ea, eb;

	a = fopen(arq_1, "r");
	b = fopen(arq_2, "r");
	saida = fopen(output, "wb");

	fread(&ea, sizeof(Endereco), 1, a);
	fread(&eb, sizeof(Endereco), 1, b); 

	while (!feof(a) && !feof(b))
	{
		if (compare(&ea, &eb) < 0)
		{
			fwrite(&ea, sizeof(Endereco), 1, saida);
			fread(&ea, sizeof(Endereco), 1, a);
		}
		else
		{
			fwrite(&eb, sizeof(Endereco), 1, saida);
			fread(&eb, sizeof(Endereco), 1, b);
		}
	}

	while (!feof(a))
	{
		fwrite(&ea, sizeof(Endereco), 1, saida);
		fread(&ea, sizeof(Endereco), 1, a);
	}
	while (!feof(b))
	{
		fwrite(&eb, sizeof(Endereco), 1, saida);
		fread(&eb, sizeof(Endereco), 1, b);
	}

	fclose(a);
	fclose(b);
	fclose(saida);
}

int try(int argc, char** argv)
{
	FILE *a, *b, *saida;
	Endereco ea, eb;
  char arq_1[15]; 
  char arq_2[15];
  char output[15];
  char merged_name[15];

  int proximo = 0;
  int qntdArq;
  int contaPares = 1;
  int qntdjuntos;


  printf("Em quantas partes o arquivo foi dividido?"); 
	scanf("%d", &qntdArq);
  while (qntdArq >= 2) 
	{
    proximo = 1;
		while (contaPares <= qntdArq)
		{

			sprintf(arq_1, "Arquivo_%d.dat", proximo);
			sprintf(arq_2, "Arquivo_%d.dat", proximo + 1);
			sprintf(output, "Arquivo_%d.dat", proximo + qntdArq);
			printf("%s intercalado com o arquivo %s e transformando em %s\n", arq_1, arq_2, output);
			merge(arq_1, arq_2, output); 
			remove(arq_1);					
			remove(arq_2);				
			proximo++;
			contaPares += 2; 
		}
		qntdjuntos = qntdjuntos / 2; 
		qntdArq += qntdjuntos;
	}
	printf("Todos os %d blocos intercalados e passados para Arquivo_%d.dat", qntdArq, contaPares);


	printf("\nArquivo CEP_RJ%d.dat renomeado para CEP_ordenado.dat\n", contaPares);
	sprintf(merged_name, "Arquivo_%d.dat", contaPares);
	rename(merged_name, "CEP_ordenado.dat"); 
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
  try(argc, argv);
}
