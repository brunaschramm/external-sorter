#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

/* Geracao de inteiro aleatorio fonte:
 * http://www.cplusplus.com/reference/cstdlib/rand/
 */

const static unsigned long int GB = 1024*1024*1024;

void prepararAmbiente()
{
	system("mkdir input/");
}

int main(int argv, char *argc[])
{
	int tam = 4;
	if(argv != 2)
	{
		cout << "Gerando entrada de tamanho = [4] GB" << endl;
		cout << "Para gerar outro valor : " << argc[0] << " <qntd-em-GB>" << endl;
	}
	else
            {
                tam = atoi(argc[1]);
                cout << "Gerando entrada de tamanho = [" << tam << "] GB" << endl;
            }

	if(tam < 0)
	{
		cout << "Tamanho do arquivo de ser pelo menos de [1 GB]" << endl;
		exit(0);
	}

	prepararAmbiente();

	int randValue = 0;

	/* initialize random seed: */
	srand (time(NULL));

	unsigned long int QTD_VALORES = (tam * GB) / 4;

	FILE *arqBin = fopen("input/input.bin", "w");
	unsigned long int i;
	for (i = 0; i < QTD_VALORES; i++)
	{
		randValue = rand();
		fwrite (&randValue, sizeof(unsigned int), 1, arqBin);
	}

	fclose(arqBin);

	cout << "Arquivo gerado com sucesso! Total de valores = " << i << endl;

	return 0;
}
