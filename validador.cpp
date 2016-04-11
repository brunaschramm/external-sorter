#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

int main(int argv, char *argc[])
{
    if(argv != 2)
    {
        cout << "Execute: " << argc[0] << " <arquivo-entrada>" << endl;
        return 0;
    }

    cout << "Validando arquivo = [" << argc[1] << "]" << endl;

    FILE *arqBin = fopen(argc[1], "r");
    int valor, res, ult = -2147483648;
    bool invalido = false;
    while( (res = fread(&valor, sizeof(int), 1, arqBin) ) )
    {
        if (valor < ult)
        {
            invalido = true;
            break;
        }
        else
        {
            ult = valor;
        }
    }

    fclose(arqBin);

    if(invalido)
        cout << "Arquivo invalido :(" << endl;
    else
        cout << "Arquivo validado com sucesso!" << endl;

    return 0;
}
