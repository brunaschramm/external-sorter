#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

//Implementação baseada no Heap de Máxima do Cormen
static int PRIM = 0;

class Heap
{
	private:
		
		int indexHeap;
		int capacity;
		
		inline int getParent(int &index){ return index/2; }
		
		inline int getLeft(int &index){ return (index*2) + 1; }
		
		inline int getRight(int &index){ return (index*2) + 2; }
		
		void minHeapify(int &index)
		{			
			int l = getLeft(index);
			
			int r = getRight(index);
			
			int menor = index;
			
			if( l < indexHeap && array[l] < array[index] ) menor = l;
			
			if (r < indexHeap && array[r] < array[menor]) menor = r;
			
			if ( menor != index )
			{
				int aux = array[index];
				array[index] = array[menor];
				array[menor] = aux;
				minHeapify(menor);
			}
		}
		
	public:
		Heap(int &tam)
		{
			array = (int*) malloc (tam * sizeof (int));
			indexHeap = 0;
			capacity = tam;
		};
		
		int *array;
		
		inline bool empty() { return indexHeap == 0; }
		
		void clear() { indexHeap = 0; }
		
		void deletar() 
		{ 
			free(array);
			indexHeap = 0;
		}
		
		void adicionaValor(int &valor)
		{
			if(indexHeap < capacity) 
			{
				array[indexHeap] = valor;
				indexHeap++;
			}
		}
		
		int extrairMin()
		{
			int min = array[PRIM];
			indexHeap--;
			array[PRIM] = array[indexHeap];
			minHeapify(PRIM);
			return min;
		}
		
		void buildMinHeap()
		{
			if (indexHeap > 1) 
				for(int i = (indexHeap-1)/2; i >= 0; i--)
					minHeapify(i);
		}
};

class HeapOfPair
{
	private:
		
		int indexHeap;
		int capacity;
		
		inline int getParent(int &index){ return (index/2) - 1; }
		
		inline int getLeft(int &index){ return (index*2) + 1; }
		
		inline int getRight(int &index){ return (index*2) + 2; }
		
	public:
		HeapOfPair(int &tam)
		{
			array = (pair<int, FILE*>*) malloc (tam * sizeof (pair<int, FILE*>));
			indexHeap = 0;
			capacity = tam;
		};
		
		pair<int, FILE*> *array;
		
		inline bool empty() { return indexHeap == 0; }
		
		void clear() { indexHeap = 0; }
		
		void deletar() 
		{ 
			free(array);
			indexHeap = 0;
		}
		
		void adicionaValor(pair<int, FILE*> valor)
		{
			if(indexHeap < capacity) 
			{
				array[indexHeap] = valor;
				indexHeap++;
			}
		}

		pair<int, FILE*> extrairMin()
		{
			pair<int, FILE*> min = array[PRIM];
			indexHeap--;
			array[PRIM] = array[indexHeap];
			minHeapify(PRIM);
			return min;
		}

		pair<int, FILE*> consultaMin()
		{
			return array[PRIM];
		}
		
		void removeMin()
		{
			indexHeap--;
			array[PRIM] = array[indexHeap];
			minHeapify(PRIM);
		}
		
		void adicionaValorInicio(pair<int, FILE*> valor)
		{
			array[0] = valor;
			minHeapify(0);
		}
		
		void buildMinHeap()
		{
			if (indexHeap > 1) 
			{
				for(int i = (indexHeap-1)/2; i >= 0; i--) 
				{
					minHeapify(i);
				}
			}
		}
		
		void minHeapify(int index)
		{			
			int l = getLeft(index);
			
			int r = getRight(index);
			
			int menor = index;
			
			if( l < indexHeap && array[l].first < array[index].first ) menor = l;
			
			if (r < indexHeap && array[r].first < array[menor].first) menor = r;
			
			if ( menor != index )
			{
				pair<int, FILE*> aux = array[index];
				array[index] = array[menor];
				array[menor] = aux;
				minHeapify(menor);
			}
		}
};
