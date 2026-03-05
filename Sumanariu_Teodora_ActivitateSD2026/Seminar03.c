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

void afisareMasina(Masina m) {
	//afiseaza toate atributele unei masini
	printf("ID: %d| NR USI: %d| PRET: %5.2f| MODEL: %s| NUME SOFER: %s| SERIE: %s\n",
		m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if (masini == NULL || nrMasini == 0)
	{
		printf("Vector de masini e gol!\n");
	}
	else
	{
		printf("\n");
		for (int i = 0; i < nrMasini; i++)
			afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//prioritizam diferentierea de aceea scriem (*masini)[i]; in loc de *masini[i];
	//aux[i] = (*masini)[i]; e shallow copy;
	Masina* aux = (Masina*)malloc((*nrMasini + 1) * sizeof(Masina));
	for (int i = 0; i < *nrMasini; i++) {
		aux[i] = (*masini)[i];
	}
	aux[*nrMasini] = masinaNoua;
	free(*masini);

	*masini = &aux;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	char* linie[256];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";
	Masina masina;
	masina.id = atoi(strtok(linie, delimitator));
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atoi(strtok(NULL, delimitator));
	char* aux = *(strtok(NULL, delimitator));
	masina.model = (char*)malloc((sizeof(char) * (strlen(aux) + 1));
	strcpy(masina.model, aux);
	aux = *(strtok(NULL, delimitator));
	masina.numeSofer = (char*)malloc((sizeof(char) * (strlen(aux) + 1));
	strcpy(masina.numeSofer, aux);
	masina.serie = strtok(NULL, delimitator)[0];

	return masina;
	//de facut un tabel cu ob.func.class.struct
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");

	if (!f) {
		printf(" ");
		return NULL;
	}
	else
	{
		Masina* vectorMasini = NULL;
		nrMasiniCitite = 0;
		while (!feof(f))
		{
			Masina m = citireMasinaFisier(f);
			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, m);
		}
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	/*for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	*nrMasini = 0;*/
}

int main() {

	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);

}