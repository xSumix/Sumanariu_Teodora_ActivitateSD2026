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
//ldi 
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
	printf("%-4d | %-20s | %-8s | %-10s | %c | %5.2f",
		card.ID, card.Name, card.Type, card.Character, card.Rarity, card.Price);
	if (card.Damage > 0) printf(" | DMG: %3d", card.Damage);
	if (card.Block > 0) printf(" | BLK: %3d", card.Block);
	printf("\n");
}

//----------------------funcii basic din fiecare sablon------------------------
// stiva e lifo
// STIVA CU LSI
struct nod {
	card thing;
	nod* next;
};

void pushStackLSI(nod** stiva, card carte) {
	nod* aux = (nod*)malloc(sizeof(nod));
	aux->thing = carte;
	aux->next = *stiva;
	*stiva = aux;
}
card popStackLSI(nod** stiva) {
	card rez;
	if ((*stiva) == NULL) {
		rez.ID = -1;
		return rez;
	}
	rez = (*stiva)->thing;
	nod* aux = (*stiva)->next;
	free(*stiva);
	*stiva = aux;
	return rez;
}

int emptyStackLSI(nod* stiva) {
	return stiva == NULL;
}

void* citireStackCardsDinFisierLSI(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("ehhh nu file found loserrrr");
		return;
	}
	nod* stiva = NULL;
	while (!feof(file)) {
		pushStackLSI(&stiva, citireCardDinFisier(file));
	}
	fclose(file);
	return stiva;
}

void afisareStackCardsLSI(nod* stiva) {
	if (!stiva) {
		printf("nu e nimic in stiva");
	}
	while (stiva) {
		afisareCard(stiva->thing);
		stiva = stiva->next;
	}
}

void dezalocareStivaDeMasiniLSI(nod** stiva) {
	while (*stiva) {
		nod* aux = (*stiva)->next;
		free((*stiva)->thing.Name);
		free((*stiva)->thing.Type);
		free((*stiva)->thing.Character);
		free(*stiva);
		*stiva = aux;
	}
}

int sizeLSI(nod** stiva) {
	//returneaza numarul de elemente din stiva
	int nrElem = 0;
	nod* aux = NULL;
	while (*stiva) {
		pushStackLSI(&aux, popStackLSI(stiva));
		nrElem++;
	}
	while (aux) {
		pushStackLSI(stiva, popStackLSI(&aux));
	}
	return nrElem;
}

int main() {
	nod* stiva = citireStackCardsDinFisierLSI("cards.txt");
	afisareStackCardsLSI(stiva);
	printf("\n");
	printf("in stiva sunt %d de elemente\n", sizeLSI(&stiva));
	card c = popStackLSI(&stiva);
	printf("\n");
	afisareCard(c);
	printf("\n");
	pushStackLSI(&stiva, c);
	afisareStackCardsLSI(stiva);
	printf("\n");
	dezalocareStivaDeMasiniLSI(&stiva);
	afisareStackCardsLSI(stiva);
	return 0;
}

