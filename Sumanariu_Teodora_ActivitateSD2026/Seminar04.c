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

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;

struct Nod
{
	Masina info;
	Nod* next;
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

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua)
{
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* temp = malloc(sizeof(Nod));//alocam memorie pentru un nod nou
	temp->info = masinaNoua;//completam campul info cu masina primita ca parametru
	//facem shallow copy ca sa dezalocam o singura data, nu de 2 ori in czul in  care faceam deepcpoy
	temp->next = NULL;
	if ((*cap) != NULL)//daca lista nu e vida
	{
		Nod* p = *cap;
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = temp;//legam ultimul nod de nodul nou creat
	}
	else
	{
		*cap = temp;//daca lista e vida, nodul nou devine primul nod din lista
		return;
	}
}
//cum identificam o lista?: dam adresa primului nod sau adresa unei structuri care contine adresa primului nod

void afisareListaMasini(Nod* cap)
{
	while (cap)
	{
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaLaInceputInLista(Masina masinaNoua)
{
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}

Nod* citireListaMasiniDinFisier(const char* numeFisier)
{
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(file))
	{
		//Masina masina=citireMasinaDinFisier(file);
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));//sau masina
	}
	fclose(file);
	return lista;
}
//void pointer <=> void* = pointer la orice tip de data
//stramosul lui template
//void* returneaza un pointer

void dezalocareListaMasini(Nod** aux)
{
	//sunt dezalocate toate masinile si lista de elemente
	Nod* head = *aux;
	Nod* next = NULL;
	while (head != NULL)
	{
		next = head->next;
		free(head->info.model);
		free(head->info.numeSofer);
		free(head);
		head = next;
	}
	*aux = NULL;//lista devine vida
}

float calculeazaPretMediu(Nod* lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int nrMasini = 0;
	while (lista != NULL)
	{
		suma += lista->info.pret;
		nrMasini++;
		lista = lista->next;
	}
	if (nrMasini != 0) return suma / nrMasini;
	else
		return 0;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	Nod* curent = *lista;
	Nod* anterior = NULL;

	while (curent != NULL) {
		if (curent->info.serie == serieCautata) {
			Nod* deSters = curent;
			curent = curent->next;
			if (anterior == NULL)
				*lista = curent;
			else
				anterior->next = curent;

			free(deSters->info.model);
			free(deSters->info.numeSofer);
			free(deSters);
		}
		else {
			anterior = curent;
			curent = curent->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
			lista = lista->next;
		}
		else {
			lista = lista->next;
		}
	}
	return 0;
}

int main()
{
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	/*dezalocareListaMasini(&lista);
	printf("Dupa dezalocare:\n");
	afisareListaMasini(lista);*/
	float pretMediu = calculeazaPretMediu(lista);
	printf("Pretul mediu al masinilor din lista este: %.2f\n", pretMediu);
	return 0;
}

//vector=ocupa o zona de memorie continua