#include "msort.h"

//deprecated - somente para testar carregamento
void ExternalSorter::contarValoresArquivo()
{
	string line;

	m_totalValores = 0;

    streampos begin,end;
	ifstream arq ( m_arqEntrada.c_str(), ios::binary);
	begin = arq.tellg();
	arq.seekg (0, ios::end);
	end = arq.tellg();
	arq.close();
	//cout << "size is: " << (end-begin)/4 << " bytes.\n";
	m_totalValores = (end-begin)/4;
    cout <<"Total de elementos = " << m_totalValores << endl;
}

bool ExternalSorter::validarParametros()
{
	if(m_memoriaDisponivel < sizeof(int)*m_qtdVias ){
		cout << "[ERRO] Memória insuficiente para executar a ordenação - necessário no mínimo:" << endl;
		cout << "\t[" << m_qtdVias * sizeof(int) << "] bytes para [" << m_qtdVias << "] vias" << endl;
		cout << "\tOu então [" << m_memoriaDisponivel / sizeof(int) << "] vias para memória de [" << m_memoriaDisponivel << "] bytes" << endl;
		return false;
	}

	m_pFile = fopen(m_arqEntrada.c_str(), "r");
	if( !m_pFile ) {
		cout << "[ERRO] Arquivo [" << m_arqEntrada << "] não encontrado" << endl;
		return false;
	}

	return true;
}

int ExternalSorter::gerarArquivosBase()
{
	// Aloca vetor com tamanho máximo de elementos suportados
	int *array = ( int* ) malloc( sizeof(int) * m_valoresMemoria );

	while (!array)
	{
		// Não conseguiu alocar array. Memória disponivel informada deve ser maior que a máquina realmente tem.
		// Reduz os valores suportes na memória pela metade, até que caibam na memória e consiga alocar o vetor.
		m_valoresMemoria /= 2;
		array = ( int* ) malloc( sizeof(int) * m_valoresMemoria );
	}

	//cout << "Máximo de Valores na memoria = " << m_valoresMemoria << endl;

	// Guarda o contador usado para identificar os arquivos gerados
	int idArquivo = 1;

	// Guarda a quantidade de elementos lidos pelo fread, será : 0 <= lidos <= m_valoresMemoria
	int lidos = 0;

	// Buffer para o nome do arquivo
	char nomeArquivo[50];

	cout << "Iniciando geração dos arquivos parciais ordenados" << endl;

	// Enquanto tiver valor para carregar do arquivo de entrada
	while( lidos = fread (array, sizeof(int), m_valoresMemoria, m_pFile) )
	{
		//cout << "Lidos" << endl;
		//getchar();
		// Ordena valores
		qsort(array, lidos, sizeof(int), compareInt);
		//cout << "Ordenados" << endl;
		//getchar();
		// Salva valores ordenados em um arquivo parcial
		sprintf (nomeArquivo, "tmp/sorted_%d_%d", m_etapa, idArquivo);
		salvarArquivoBinario( nomeArquivo, array, lidos );

		// Incrementa identificador de arquivos parciais
		idArquivo++;
	}

	free(array);

	fclose(m_pFile);

	idArquivo--;

	cout << "Total de arquivo parciais gerados: " << idArquivo << endl;

	return idArquivo;
}

void ExternalSorter::mergeArquivosOrdenados( int &totalArquivos )
{
	cout << "Iniciando merge dos arquivos parciais ordenados" << endl;

	char buffer[100];
	FILE *arqAtual;

	int valorAtual, res = 0;

	// Variavel contém a quantidade máxima de elementos de cada arquivo que pode ser carregada na memória
	int valoresLidosPorArquivo = m_valoresMemoria / m_qtdVias;
	//pair<int, FILE*>* arrayFiles;

	// Guarda a quantidade de elementos lidos pelo fread, será : 0 <= lidos <= m_valoresMemoria
	int lidos = 0;

	// Buffer para o nome do arquivo
	char nomeArquivo[100];

	// Guarda o contador usado para identificar os arquivos gerados
	int idArquivo = 1;

	// Fica nesse loop enquanto tiver algum arquivo parcial a ser mergeado
	while(totalArquivos)
	{
		// Inicio de nova etapa de merge, volta a gerar arquivo desde o 1
		idArquivo = 1;

		// caso base - quando a quantidade de arquivos parciais gerados na ultima etapa pode ser mergeada em um único arquivo, usando K vias
		// chegou no merge final!!!
		if( totalArquivos <= m_qtdVias )
		{
			cout << "Total de etapas de merge necessárias = " << m_etapa+1 << endl;
			// Tem somente 1 arquivo, então basta salvá-lo como arquivo de texto
			if(totalArquivos == 1)
			{
				// caso onde só tem 1 arquivo, então basta renomeá-lo
				cout << "Caso 1" << endl;

				sprintf (nomeArquivo, "mv tmp/sorted_%d_%d %s", m_etapa, idArquivo, m_arqSaida.c_str());
				if( !system(nomeArquivo) )
				{
					cout << "Arquivo [" << m_arqSaida << "] gerado com sucesso!" << endl;
				}
				else
				{
					cout << "Erro ao gerar arquivo [" << m_arqSaida << "]" << endl;
				}
				
				// Sinaliza fim do loop principal
				totalArquivos = 0;
			}
			else
			{
				// caso onde tem [1 < qntd de arquivos parciais <= k], então é feito merge desses arquivos salvando a saida no arquivo final
				//cout << "Caso 2" << endl;

				FILE *arqFinal = fopen(m_arqSaida.c_str(), "w");

				HeapOfPair heap = HeapOfPair(totalArquivos);

				for(int i = 1; i <= totalArquivos; i++)
				{
					sprintf (nomeArquivo, "tmp/sorted_%d_%d", m_etapa, i);
					arqAtual = fopen(nomeArquivo, "r");

					if( res = fread (&valorAtual, sizeof(int), 1, arqAtual) )
					{
						heap.adicionaValor( make_pair<int, FILE*>(valorAtual, arqAtual) );
					}
					else
					{
						fclose(arqAtual);
					}
				}

				pair<int, FILE*> pairTmp;

				// Constroi heap de minima baseado no primeiro elemento do par
				heap.buildMinHeap();

				// quantidade de arquivos completados
				int arquivosCarregados = 0;

				// Enquanto ainda tiver algum arquivo para terminar de ler
				while(arquivosCarregados < totalArquivos)
				{
					pairTmp = heap.consultaMin();
					//fprintf (arqFinal, "%d\n", pairTmp.first);
					fwrite (&pairTmp.first, sizeof(int), 1, arqFinal);

					if( !(res = fread (&pairTmp.first, sizeof(int), 1, pairTmp.second)) )
					{
						// Se não tem mais elemento para ler do arquivo, fecha-o e incrementa contador de arquivos finalizados
						arquivosCarregados++;
						fclose( pairTmp.second );
						heap.removeMin();
					}
					else
					{
						heap.adicionaValorInicio(pairTmp);
						//heap.buildMinHeap();
					}
				}
				heap.deletar();

				// Sinaliza fim do loop principal
				totalArquivos = 0;

				fclose(arqFinal);
			}
		}
		else
		{
			int totalEtapaAnterior = totalArquivos;
			totalArquivos = 0;
			int indexLeituraArquivos = 1, indexEscritaArquivos = 1;

			while( totalEtapaAnterior )
			{
				// caso quando a quantidade de arquivos parciais gerados na ultima etapa é maior que a quantidade de vias
				// então devem ser feitas mais uma etapas parciais de merge
				//cout << "Caso 3" << endl;
				int completados = 0; // quantidade de arquivos completados

				// armazenará a contagem de quantos arquivos foram realmente carregados. Sendo i <= m_qtdVias.
				HeapOfPair heap = HeapOfPair(m_qtdVias);

				int arquivosCarregados = 0;
				for(int i = 1; i <= m_qtdVias and arquivosCarregados < totalEtapaAnterior; i++)
				{
					sprintf (nomeArquivo, "tmp/sorted_%d_%d", m_etapa, indexLeituraArquivos);

					indexLeituraArquivos++;

					arqAtual = fopen(nomeArquivo, "r");

					if(!(res = fread (&valorAtual, sizeof(int), 1, arqAtual)) )
					{
						fclose(arqAtual);
						break;
					}

					arquivosCarregados++;

					heap.adicionaValor( make_pair<int, FILE*>(valorAtual, arqAtual) );
				}

				totalEtapaAnterior -= arquivosCarregados;
				if( arquivosCarregados == 1)
				{
					// caso onde só sobrou 1 arquivo parcial, então ele é renomeado para ser processado na próxima etapa
					sprintf (nomeArquivo, "mv tmp/sorted_%d_%d tmp/sorted_%d_%d", m_etapa, indexEscritaArquivos-1, m_etapa+1, indexEscritaArquivos);
					int res = system(nomeArquivo);
				}
				else
				{
					pair<int, FILE*> pairTmp;
					heap.buildMinHeap();

					sprintf (nomeArquivo, "tmp/sorted_%d_%d", m_etapa+1, indexEscritaArquivos);
					indexEscritaArquivos++;
					// criar novo arquivo da etapa+1
					FILE *novoArq = fopen(nomeArquivo, "w");
					int aux = 0;
					while(completados < arquivosCarregados)
					{
						//pairTmp = heap.extrairMin();
						pairTmp = heap.consultaMin();
						fwrite (&pairTmp.first, sizeof(unsigned int), 1, novoArq);
						if(!(res = fread (&pairTmp.first, sizeof(int), 1, pairTmp.second)) )
						{
							completados++;
							fclose(pairTmp.second);
							heap.removeMin();
						}
						else
						{
							heap.adicionaValorInicio(pairTmp);
							//heap.adicionaValor(pairTmp);
							//heap.buildMinHeap();
						}
					}
					fclose(novoArq);
					heap.deletar();
				}
				totalArquivos++;
			}
			m_etapa++;
		}
	}

	return;
}

void ExternalSorter::executar()
{
	// Todo : remover funcao - somente para teste
	//contarValoresArquivo();

	if( validarParametros() )
	{
		// Passo 1 - Dividir o arquivo de entrada em N arquivos parciais ordenados,
		// onde o tamanho de cada um varia de : 0 < TAM <= memoriaDisponivel/sizeof(int)
		int qtdParciais = gerarArquivosBase();

		// Passo 2 - Faz merge dos arquivos parciais gerados na etapa anterior,
		// onde são mergeados de K em K arquivos, até que exista somente 1 arquivo
		mergeArquivosOrdenados( qtdParciais );
	}
}

int main(int argv, char *argc[])
{
	double start = getUnixTime();

	if(argv != 5){
		cout << "Usar: " << argc[0] << " <path-entrada> <path-saida> <memoria> <numero-de-vias>" << endl;
		exit(1);
	}

	string arquivoEntrada(argc[1]);
	cout << "- Arquivo de entrada = [" << arquivoEntrada << "]" << endl;
	string arquivoSaida(argc[2]);
	cout << "- Arquivo de saida = [" << argc[2] << "]" << endl;
	unsigned long int memoriaDisponivel = atol(argc[3]);
	cout << "- Memória disponível = [" << memoriaDisponivel << "] bytes" << endl;
	int qtdVias = atoi(argc[4]);
	cout << "- Número de vias = [" << qtdVias << "]" << endl;

	prepararAmbiente();

	ExternalSorter sorter = ExternalSorter( arquivoEntrada, arquivoSaida, memoriaDisponivel, qtdVias);

	sorter.executar();

	//limparAmbiente();

	cout << "Tempo total de processamento: " << getUnixTime() - start << endl;

	return 0;
}
