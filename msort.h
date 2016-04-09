#include "utils.h"

using namespace std;

class ExternalSorter {
	public:
		ExternalSorter(string &entrada, string &saida, unsigned long int &memoria, int &vias ): 
		m_arqEntrada(entrada), m_arqSaida(saida), m_memoriaDisponivel(memoria), m_qtdVias(vias), m_etapa(0)
		{
			// Calcula o máximo de valores que cabem na memoria
			m_valoresMemoria = memoria / sizeof(int);
		};
		void executar();
		
	private:
		void contarValoresArquivo();
		bool validarParametros();
		int gerarArquivosBase();
		void mergeArquivosOrdenados(int &qtdParciais);
		
		string m_arqEntrada;
		string m_arqSaida;
		unsigned long int m_memoriaDisponivel;
		int m_qtdVias;
		
		int m_totalValores;
		// Variavel contem o máximos de elementos que cabem na memoria
		unsigned long int m_valoresMemoria;
		int m_etapa;
		FILE* m_pFile;
};
