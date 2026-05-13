#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara
typedef struct NodP NodP;// principal
typedef struct NodS NodS; //S de la secundar

struct NodP {
	Masina info;
	NodP* next;
	NodS* vechini;
};

struct NodS
{
	NodP* id;
	NodS* next;
};


//2.
//functii de inserare in liste
//si in principala si in secundara

void AddinListaPrincipala(NodP** graf, Masina masina)
{
	NodP* nou = malloc(sizeof(NodP));
	nou->next = NULL;
	nou->vechini = NULL;
	nou->info = masina;

	if (!(*graf)) (*graf) = nou;
	NodP* p = (*graf);
	while (p->next) {
		p = p->next;
	}
	p->next = nou;
}

void AddinListaSecundara(NodS** cap, NodP* vechin)
{
	NodS* nou = malloc(sizeof(NodS));
	nou->next = NULL;
	nou->id = vechin;
	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		NodS* p = (*cap);
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}

//3.
//functie de cautarea in lista principala dupa ID
NodP* cautaNodDupaID(NodP* graf, int id)
{
	NodP* temp;
	while (graf && graf->info.id != id) {
		graf = graf->next;
	}
	return graf;
}

//4.
//inserare muchie
void inserareMuchie(NodP* graf, int idStart, int idStop) {
	NodP* nodStart = cautaNodDupaID(graf, idStart);
	NodP* nodStop = cautaNodDupaID(graf, idStop);

	if (nodStart && nodStop) {
		AddinListaSecundara(&nodStart->vechini, &nodStop);
		AddinListaSecundara(&nodStop->vechini, &nodStart);
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	NodP* graf = NULL;
	while (!feof(file))
	{
		AddinListaPrincipala(graf, citireMasinaDinFisier(file));
	}
	fclose(file);
	return graf;
}

void citireMuchiiDinFisier(NodP* graf, const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	int start, stop;
	while (!feof(file))
	{
		fscanf(file, "%d %d", &start, &stop);
		inserareMuchie(graf, start, stop);
	}
	fclose(file);
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste

}

void afisearaListaSecundara(int id, NodP* graf) {
	NodP* nou = cautaNodDupaID(graf, id);
	NodS* aux = nou->vechini;
	while (aux) {
		afisareMasina(aux->id->info);
		aux = aux->next;
	}
}

int main() {
	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");
	afisearaListaSecundara(graf, 3);
	return 0;
}