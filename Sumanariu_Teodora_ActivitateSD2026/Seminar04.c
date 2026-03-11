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

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua)
{
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = malloc(sizeof(Nod));//alocam memorie pentru un nod nou
	nou->info = masinaNoua;//completam campul info cu masina primita ca parametru
	//facem shallow copy ca sa dezalocam o singura data, nu de 2 ori in czul in  care faceam deepcpoy
	nou->next = NULL;
	Nod* aux = *lista;//auxiliar pentru a parcurge lista
	if ((*lista) != NULL)//daca lista nu e vida
	{
		while (aux->next != NULL)
		{
			aux = aux->next;
		}
		aux->next = nou;//legam ultimul nod de nodul nou creat
	}
	else
	{
		*lista = nou;//daca lista e vida, nodul nou devine primul nod din lista
		return;
	}
}
//cum identificam o lista?: dam adresa primului nod sau adresa unei structuri care contine adresa primului nod

void afisareListaMasini(Nod* cap)
{
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->next;//trebuie sa trecem la urmatorul nod
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
///void* returneaza un pointer

void dezalocareListaMasini(Nod** lista)
{
	//sunt dezalocate toate masinile si lista de elemente
	Nod* head = *lista;
	Nod* next = NULL;
	while (head != NULL)
	{
		next = head->next;
		free(head->info.model);
		free(head->info.numeSofer);
		free(head);
		head = next;
	}
	*lista = NULL;//lista devine vida
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

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
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