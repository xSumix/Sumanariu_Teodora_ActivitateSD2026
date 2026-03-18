#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

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

void afisareListaMasini(ListaDubla lista) {
	//cum se face cu for?
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareInversaListaMasini(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* ld, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = ld->ultim;
	if (ld->ultim) {
		ld->ultim->next = nou;
	}
	else {
		ld->prim = nou;
	}
	ld->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* ld, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = ld->prim;
	nou->prev = NULL;
	if (ld->prim) {
		ld->prim->prev = nou;
	}
	else {
		ld->ultim = nou;
	}
	ld->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
		return ld;
	}
}

void dezalocareLDMasini(ListaDubla* ld) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	while (ld->prim && ld->prim->next) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);
	}
	ld->prim = ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla ld) {
	//calculeaza pretul mediu al masinilor din lista.
	Nod* p = ld.prim;
	float medie = 0;
	int nrMasini = 0;
	while (p) {
		medie += p->info.pret;
		nrMasini++;
		p = p->next;
	}
	medie /= nrMasini;
	return medie;
}

void stergeMasinaDupaID(ListaDubla* ld, int id) {
	/*int foundMasina = 1;
	Nod* NodCurent = ld->prim;
	while (NodCurent) {
		if (NodCurent->info.id == id)
		{
			foundMasina = 0;
			if (!NodCurent->prev)
			{
				ld->prim = ld->prim->next;
				ld->prim->prev = NULL;
			}
			else if(!NodCurent->next)
			{

			}
		}
		NodCurent = NodCurent->next;
	}
	if (foundMasina == 1) {
		printf("Nu exista masina cu acest id");
	}*/
	/*
	* stergem asta ptc o incorporam in while
	if (ld->prim && ld->prim->info.id == id) {
		free(ld->prim->info.model);
		free(ld->prim->info.numeSofer);
		ld->prim = ld->prim->next;
		if (ld->prim) {
			free(ld->prim->prev);
		}
		else {
			free(ld->ultim);
			ld->ultim = NULL;
		}
		return;
	}*/
	Nod* p = ld->prim;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p) {
		if (p->prev)
		{
			p->prev->next = p->next;
		}
		else
		{
			ld->prim = p->next;
		}

		if (p->next)
		{
			p->next->prev = p->prev;
		}
		else
		{
			p->prev->next = NULL;
			ld->ultim = p->prev;
		}

		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla ld) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	Nod* p = ld.prim;
	float pretMax = 0;
	while (p) {
		if (p->info.pret < pretMax)
			pretMax = p->info.pret;
		p = p->next;
	}
	while (p) {
		if (p->info.pret == pretMax)
		{
			char* numeSoferCuMasinaScumpa = malloc(sizeof(p->info.numeSofer));
			strcpy(numeSoferCuMasinaScumpa, p->info.numeSofer);
			return *numeSoferCuMasinaScumpa;
		}
		p = p->next;
	}
	return NULL;
}

int main() {

	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	//lista.prim = NULL;
	//lista.ultim = NULL;
	stergeMasinaDupaID(&lista, 1);
	printf("Afisare lista dupa stergere de la inceput\n");
	afisareListaMasini(lista);

	stergeMasinaDupaID(&lista, 6);
	printf("Afisare lista dupa stergere de la mijloc\n");
	afisareListaMasini(lista);

	stergeMasinaDupaID(&lista, 10);
	printf("Afisare lista dupa stergere de la inceput\n");
	afisareListaMasini(lista);
	return 0;
}