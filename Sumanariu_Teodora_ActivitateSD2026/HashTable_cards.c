#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

typedef struct card card;
typedef struct nod nod;
typedef struct dnod dnod;
typedef struct ldi ldi;
typedef struct vct vct;

struct card {
	int id;
	char* name;
	char rarity;
	float price;
};

struct nod {
	card info;
	nod* next;
};

struct dnod {
	card info;
	dnod* next;
	dnod* prev;
};

struct ldi {
	dnod* head;
	dnod* tail;
};

struct vct {
	card* deck;
	int size;
	int cap;
};

//STRUCTURI--------------------------------------------------------

void afisareCard(card c) {
	if (c.id == -1) printf("something went wrong");
	else
		printf("%d| %-10s | %c | %5.2f\n",
			c.id, c.name, c.rarity, c.price);
}

card citireCardDinFisier(FILE* f) {
	card c;
	c.id = -1;
	char linie[256];
	char delim[3] = ",\n";
	fgets(linie, 255, f);
	if (f) {
		char* aux = strtok(linie, delim);
		if (!aux) return c;

		c.id = atoi(aux);
		aux = strtok(NULL, delim);
		//c.name = (char*)malloc((strlen(aux) + 1) * sizeof(char));
		//strcpy(c.name, aux);
		c.name = _strdup(aux);
		c.rarity = (strtok(NULL, delim))[0];
		c.price = atof(strtok(NULL, delim));
	}
	return c;
}
int calculeazaHash(int id, int dim) {
	return id % dim;
}

//ACCES SI AFISARE--------------------------------------------------
typedef struct SLL_HT SLL_HT;
struct SLL_HT {
	nod** list;
	int dim;
};
typedef struct DLL_HT DLL_HT;
struct DLL_HT {
	ldi* list;
	int dim;
};
typedef struct VHT VHT;
struct VHT {
	vct* list;
	int dim;
};
//HASH TABLES-----------------------------------------------

//INITIALIZARE
SLL_HT initSLL_HT(int dim) {
	SLL_HT ht;
	ht.dim = dim;
	ht.list = (nod**)calloc(dim, sizeof(nod*));
	return ht;
}
DLL_HT initDLL_HT(int dim) {
	DLL_HT ht;
	ht.dim = dim;
	ht.list = (ldi*)calloc(dim, sizeof(ldi));
	return ht;
}

void initVCT(vct* v, int cap) {
	v->cap = cap;
	v->size = 0;
	v->deck = (card*)malloc(v->cap * sizeof(card));
}

VHT initVHT(int dim) {
	VHT ht;
	ht.dim = dim;
	ht.list = (vct*)malloc(dim * sizeof(vct));
	for (int i = 0; i < dim; i++)
		initVCT(&ht.list[i], 10);
	return ht;
}

//CITIRE DIN FISIER
void addInListSLLHT(nod** list, card c) {
	nod* aux = (nod*)malloc(sizeof(nod));
	aux->info = c;
	aux->info.name = _strdup(c.name);
	aux->next = NULL;
	if (!(*list)) *list = aux;
	else {
		nod* p = *list;
		while (p->next) p = p->next;
		p->next = aux;
	}
}
void inserCardSLLHT(SLL_HT ht, card c) {
	int hashcod = calculeazaHash(c.id, ht.dim);
	addInListSLLHT(&ht.list[hashcod], c);
}
SLL_HT citireSLL_HT(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	SLL_HT ht = initSLL_HT(10);
	if (file) {
		while (!feof(file)) {
			inserCardSLLHT(ht, citireCardDinFisier(file));
		}
	}
	fclose(file);
	return ht;
}

void addInListDLLHT(ldi* list, card c) {
	dnod* aux = (dnod*)malloc(sizeof(dnod));
	aux->info = c;
	aux->info.name = _strdup(c.name);
	aux->next = NULL;
	aux->prev = list->tail;

	if (!list->head) list->head = aux;
	if (list->tail) list->tail->next = aux;
	else
		list->head = aux;
	list->tail = aux;
}
void inserCardDLLHT(DLL_HT ht, card c) {
	int hashcod = calculeazaHash(c.id, ht.dim);
	addInListDLLHT(&ht.list[hashcod], c);
}
DLL_HT citireDLL_HT(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	DLL_HT ht = initDLL_HT(10);
	if (file) {
		while (!feof(file)) inserCardDLLHT(ht, citireCardDinFisier(file));
	}
	fclose(file);
	return ht;
}

void addInListVHT(vct* list, card c) {
	if (list->size < list->cap) {
		list->deck[list->size++] = c;
	}
	else printf("list is full :( wtf");
}
void inserCardVHT(VHT ht, card c) {
	int hashcod = calculeazaHash(c.id, ht.dim);
	addInListVHT(&ht.list[hashcod], c);
}
VHT citireVHT(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	VHT ht = initVHT(10);
	if (file) {
		while (!feof(file))inserCardVHT(ht, citireCardDinFisier(file));
	}
	fclose(file);
	return ht;
}

//AFISARE
void afisareSLL_HT(SLL_HT ht) {
	if (!ht.list) printf("nu e nmk aici bre");
	else
		for (int i = 0; i < ht.dim; i++) {
			printf("CLUSTER [%d]: ", i);
			nod* aux = ht.list[i];
			while (aux) {
				afisareCard(aux->info);
				aux = aux->next;
			}
			printf("\n");
		}
}
void afisareDLL_HT(DLL_HT ht) {
	if (!ht.list) printf("nu e nmk aici bre");
	else
		for (int i = 0; i < ht.dim; i++) {
			printf("CLUSTER [%d]: ", i);
			dnod* aux = ht.list[i].head;
			while (aux) {
				afisareCard(aux->info);
				aux = aux->next;
			}
			printf("\n");
		}
}
void afisareVHT(VHT ht) {
	if (!ht.list) printf("nu e nmk aici bre");
	else
		for (int i = 0; i < ht.dim; i++) {
			printf("CLUSTER [%d]: ", i);
			vct aux = ht.list[i];
			for (int j = 0; j < aux.size; j++) {
				afisareCard(aux.deck[j]);
			}
			printf("\n");
		}
}
//cautare=====================================
card getCardById_SLLHT(SLL_HT ht, int id) {
	card rez;
	rez.id = -1;
	int index = calculeazaHash(id, ht.dim);
	nod* aux = ht.list[index];
	while (aux)
	{
		if (aux->info.id == id) {
			rez = aux->info;
			break;
		}
		aux = aux->next;
	}
	return rez;
}
card getCardById_DLLHT(DLL_HT ht, int id) {
	card rez;
	rez.id = -1;
	int index = calculeazaHash(id, ht.dim);
	dnod* aux = (dnod*)malloc(sizeof(dnod));
	*aux = *ht.list[index].head;
	while (aux)
	{
		if (aux->info.id == id) {
			rez = aux->info;
			break;
		}
		aux = aux->next;
	}
	return rez;
}
card getCardById_VHT(VHT ht, int id) {
	card rez;
	rez.id = -1;
	int index = calculeazaHash(id, ht.dim);
	vct aux;
	initVCT(&aux, 10);
	aux = ht.list[index];
	for (int i = 0; i < aux.size; i++)
		if (aux.deck[i].id == id) {
			rez = aux.deck[i];
			break;
		}
	return rez;
}
//DEZALOCARE--------------------------------------------
void dezalocareSLLHT(SLL_HT* ht) {
	for (int i = 0; i < ht->dim; i++) {
		nod* aux = ht->list[i];
		while (aux) {
			nod* dez = aux;
			free(aux->info.name);
			aux = aux->next;
			free(dez);
		}
	}
	free(ht->list);
	ht->list = NULL;
}
void dezalocareDLLHT(DLL_HT* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dnod* aux = ht->list[i].head;
		while (aux) {
			dnod* dez = aux;
			free(aux->info.name);
			aux = aux->next;
			free(dez);
		}
	}
	free(ht->list);
	ht->list = NULL;
}
void dezalocareVHT(VHT* ht) {
	for (int i = 0; i < ht->dim; i++) {
		vct* aux = &ht->list[i];
		for (int j = 0; j < aux->size; j++) {
			free(aux->deck[j].name);
		}
		free(aux->deck);
	}
	free(ht->list);
	ht->list = NULL;
}

int main() {
	/* //testare citire si afisare carte
	FILE* f = fopen("scards.txt", "r");
	card test = citireCardDinFisier(f);
	afisareCard(test);
	fclose(f);*/

	// 47 89 6
	//SLL--------------------------------
	SLL_HT sht = initSLL_HT(10);
	sht = citireSLL_HT("scards.txt");
	afisareSLL_HT(sht);
	card test = getCardById_SLLHT(sht, 47);
	afisareCard(test);
	test = getCardById_SLLHT(sht, 89);
	afisareCard(test);
	test = getCardById_SLLHT(sht, 6);
	afisareCard(test);
	dezalocareSLLHT(&sht);
	afisareSLL_HT(sht);

	////DLL--------------------------------
	DLL_HT dht = initDLL_HT(10);
	dht = citireDLL_HT("scards.txt");
	afisareDLL_HT(dht);
	test = getCardById_DLLHT(dht, 47);
	afisareCard(test);
	test = getCardById_DLLHT(dht, 89);
	afisareCard(test);
	test = getCardById_DLLHT(dht, 6);
	afisareCard(test);
	dezalocareDLLHT(&dht);
	afisareDLL_HT(dht);

	////VCT--------------------------------
	VHT vht = initVHT(10);
	vht = citireVHT("scards.txt");
	afisareVHT(vht);
	test = getCardById_VHT(vht, 47);
	afisareCard(test);
	test = getCardById_VHT(vht, 89);
	afisareCard(test);
	test = getCardById_VHT(vht, 6);
	afisareCard(test);
	dezalocareVHT(&vht);
	afisareVHT(vht);

	return 0;
}