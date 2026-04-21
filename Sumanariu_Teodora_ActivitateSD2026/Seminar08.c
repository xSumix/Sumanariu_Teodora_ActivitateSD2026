#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru Heap
// heap e o strctura de date liniara, folosita pt cozi de prioritate
// dorim val min si max
// avem min heap si max heap
// 4 7 3 9 5 8 
// heapsort?
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrElemViz;
	Masina* vector;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int left = 2 * pozitieNod + 1;
	int right = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (heap.vector[left].pret > heap.vector[pozMax].pret && left < heap.nrElemViz) {
		pozMax = left;
	}
	if (heap.vector[right].pret > heap.vector[pozMax].pret && right < heap.nrElemViz) {
		pozMax = right;
	}
	if (pozMax != pozitieNod) {
		Masina maux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = maux;
		if (pozMax < (heap.nrElemViz - 2) / 2)
			filtreazaHeap(heap, pozMax);
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	if (file) {
		while (!feof(file)) {
			heap.vector[heap.nrElemViz++] = citireMasinaDinFisier(file);
		}
	}
	fclose(file);
	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--)
		filtreazaHeap(heap, i);
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemViz; i++)
		afisareMasina(heap.vector[i]);
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemViz; i < heap.lungime; i++)
		afisareMasina(heap.vector[i]);
}

Masina extrageMasina(Heap* heap) {
	//in ce conditii putem extrage? cand avem elemente vizibile
	Masina maux;
	maux.id = -1;
	if (heap->nrElemViz) {
		heap->nrElemViz--;
		maux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = maux;
		filtreazaHeap(*heap, 0);
		return maux;
	} // yes but
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].numeSofer);
		free(heap->vector[i].model);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrElemViz = 0;
	heap->lungime = 0;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("extrageri:   \n");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	printf("heap ascuns!!\n");
	afiseazaHeapAscuns(heap);
	dezalocareHeap(&heap);

	return 0;
}