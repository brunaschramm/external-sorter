#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

/* Geracao de inteiro aleatorio fonte:
 * http://www.cplusplus.com/reference/cstdlib/rand/
 */

const static unsigned long int GB = 1024*1024*1024;

int main(int argv, char *argc[])
{
	if(argv != 2)
	{
		cout << "Faltou informar o tamanho em GB do arquivo. Execute: " << argc[0] << " <qntd-em-GB>" << endl;
		exit(0);
	}
	int tam = atoi(argc[1]);

	cout << "Gerando entrada de tamanho = [" << tam << "] GB" << endl;

	if(tam < 0)
	{
		cout << "Tamanho do arquivo de ser pelo menos de [1 GB]" << endl;
		exit(0);
	}

	int randValue = 0;

	/* initialize random seed: */
	srand (time(NULL));

	unsigned long int QTD_VALORES = (tam * GB) / 4;

	FILE *arqBin = fopen("input.bin", "w");
	unsigned long int i;
	for (i = 0; i < QTD_VALORES; i++)
	{
		randValue = rand();
		fwrite (&randValue, sizeof(unsigned int), 1, arqBin);
	}

	fclose(arqBin);

	cout << "Arquivo [input.bin] gerado com sucesso! Total de valores = " << i << endl;

	return 0;
}
