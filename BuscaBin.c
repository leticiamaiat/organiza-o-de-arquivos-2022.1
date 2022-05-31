#include <stdio.h>
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

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Erro ao utilizar %s\n", argv[0]);
		fprintf(stderr,"USO: %s [CEP PARA BUSCA]\n",argv[0]);
		return 1;
	}
	
	FILE *f;
	Endereco e;
	int total, cmp, tamarq, tamreg, inicio, meio, fim;
	
	f = fopen("cep_ordenado.dat","r");
	fseek(f,0,SEEK_END);
	tamarq = ftell(f);
	tamreg = sizeof(Endereco);
	inicio = 0;
	fim = (tamarq/tamreg) - 1;
	total = 0;
	
	while(inicio <= fim)
	{
		meio = (inicio+fim)/2;
		fseek(f,meio*tamreg,SEEK_SET);
		fread(&e,tamreg,1,f);
		total++;
		cmp = strncmp(argv[1],e.cep,8);
		if(cmp == 0)
		{
			printf("CEP encontrado! =)\n\n");
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
			printf("Total lidos: %d\n", total);
			fclose(f);
			return 0;		
		}
		else if(cmp > 0)
			inicio = meio + 1;
		else
			fim = meio - 1;
	}
	
	printf("CEP não encontrado! =(\n");
	fclose(f);
	return 1;	
}
