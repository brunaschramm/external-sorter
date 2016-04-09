#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void limparAmbiente()
{
	system("rm -rf tmp/");
}

void prepararAmbiente()
{
	limparAmbiente();
	system("mkdir tmp/");
}

//Retorna UNIX epoch, com preciso at nanossegundos
double getUnixTime()
{
    struct timespec tv;
    if(clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;
    return (((double) tv.tv_sec) + (double) (tv.tv_nsec / 1000000000.0));
}

int compareInt (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void salvarArquivoOrdenadoBinario( char *buffer, int *array, int &tamanho)
{
	if(tamanho == 0) return;
	
    cout << "Salvando " << buffer << endl;
	FILE *arq = fopen(buffer,"w");
	fwrite (array, sizeof(int), tamanho, arq);
	fclose(arq);
}

void salvarArquivoOrdenadoTexto( char *buffer, int *array, int &tamanho)
{
	if(tamanho == 0) return;
	
    cout << "Salvando " << buffer << endl;
	FILE *arq = fopen(buffer,"w");
	for(int i = 0; i < tamanho; i++)
	{
		fprintf (arq, "%d\n", array[i]);
	}
	fclose(arq);
}

/*
void salvarArquivoOrdenado( Heap &heap, char *buffer)
{
    cout << "Salvando " << buffer << endl;

	FILE *arq = fopen(buffer,"w");
	while(!heap.empty()) 
	{
		fprintf (arq, "%d\n", heap.extrairMin());
	}
	fclose(arq);
}
*/
