#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

typedef struct card card;
typedef struct nod nod;
typedef struct dnod dnod;
typedef struct ldi ldi;
typedef struct vct vct;

struct card {
	int ID;
	int Damage;
	int Block;
	char* Name;
	char* Type;
	char* Character;
	char Rarity;
	float Price;
};
struct vct {
	card* cvect;
	int top;
	int capacitate;
};

//----------------------------------------------
card citireCardDinFisier(FILE* file) {
	card c;
	c.ID = -1;
	char linie[256];
	fgets(linie, 255, file);
	char delim[3] = ",\n";
	if (!file) {
		printf("file not found\n");
		return;
	}

	c.ID = atoi(strtok(linie, delim));
	c.Damage = atoi(strtok(NULL, delim));
	c.Block = atoi(strtok(NULL, delim));

	char* aux = strtok(NULL, delim);
	c.Name = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(c.Name, aux);
	aux = strtok(NULL, delim);
	c.Type = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(c.Type, aux);
	aux = strtok(NULL, delim);
	c.Character = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(c.Character, aux);

	c.Rarity = (strtok(NULL, delim))[0];
	c.Price = atof(strtok(NULL, delim));

	return c;
}
void afisareCard(card card) {
	printf("%-4d | %-20s | %-8s | %-10s | %c | %5.2f |",
		card.ID, card.Name, card.Type, card.Character, card.Rarity, card.Price);
	if (card.Damage > 0) printf(" DMG: %3d | ", card.Damage);
	if (card.Block > 0) printf(" BLK: %3d", card.Block);
	printf("\n");
}

//----------------------funcii basic din fiecare sablon------------------------
// stiva e lifo
// STIVA CU LDI
struct dnod
{
	card thing;
	dnod* next;
	dnod* prev;
};
struct ldi {
	dnod* head;
	dnod* tail;
};
void pushStackLDI(ldi* stiva, card carte) {
	dnod* aux = (dnod*)malloc(sizeof(dnod));
	if (aux == NULL) {
		printf("malloc failed");
		return;
	}
	aux->thing = carte;
	aux->next = stiva->head;
	aux->prev = NULL;
	if (stiva->head) {
		stiva->head->prev = aux;
	}
	stiva->head = aux;
	if (!stiva->tail) {
		stiva->tail = aux;
	}
}
card popStackLDI(ldi* stiva) {
	card rez;
	rez.ID = -1;
	if (stiva->head == NULL) {
		rez.ID = -1;
		return rez;
	}
	rez = stiva->head->thing;
	dnod* aux = stiva->head;
	stiva->head = stiva->head->next;
	if (stiva->head) {
		stiva->head->prev = NULL;
	}
	else {
		stiva->tail = NULL;
	}
	free(aux);
	return rez;
}

int emptyStackLDI(ldi* stiva) {
	return stiva->head == NULL;
}

ldi citireStackCardsDinFisierLDI(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	ldi stiva;
	stiva.head = NULL;
	stiva.tail = NULL;
	while (!feof(file)) {
		pushStackLDI(&stiva, citireCardDinFisier(file));
	}
	fclose(file);
	return stiva;
}

void dezalocareStivaDeMasiniLDI(ldi* stiva) {
	while (stiva->head) {
		dnod* aux = stiva->head;
		free(stiva->head->thing.Name);
		free(stiva->head->thing.Type);
		free(stiva->head->thing.Character);
		stiva->head = stiva->head->next;
		free(aux);
	}
	stiva->tail = NULL;
}

//metode de  procesare
card getMasinaByID_LDI(ldi stiva, int id) {
	card rez;
	if (!stiva.head) {
		rez.ID = -1;
		return rez;
	}
	dnod* stivAux = NULL;
	while (stiva.head) {
		card caux = popStackLDI(&stiva);
		if (caux.ID == id) {
			rez = caux;
			break;
		}
		pushStackLDI(&stivAux, caux);
	}
	while (stivAux) {
		pushStackLDI(&stiva, popStackLDI(&stivAux));
	}
	return rez;
};

void afisareStackCardsLDI(ldi stiva) {
	if (!stiva.head) {
		printf("nu e nimic in stiva");
	}
	while (stiva.head) {
		afisareCard(stiva.head->thing);
		stiva.head = stiva.head->next;
	}
}

int sizeLDI(ldi stiva) {
	int nrelem = 0;
	while (stiva.head)
	{
		nrelem++;
		stiva.head = stiva.head->next;
	}
	return nrelem;
}


int main() {
	ldi stiva = citireStackCardsDinFisierLDI("cards.txt");
	afisareStackCardsLDI(stiva);
	printf("\n");
	printf("in stiva sunt %d de elemente\n", sizeLDI(stiva));
	card c = popStackLDI(&stiva);
	printf("\n");
	afisareCard(c);
	printf("\n");
	pushStackLDI(&stiva, c);
	afisareStackCardsLDI(stiva);
	printf("\n");
	dezalocareStivaDeMasiniLDI(&stiva);
	afisareStackCardsLDI(stiva);
	return 0;
}

