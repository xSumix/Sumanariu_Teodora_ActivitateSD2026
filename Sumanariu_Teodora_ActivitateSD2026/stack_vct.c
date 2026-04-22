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
// STIVA CU vector

struct vct {
	card* items;
	int top;
	int capacitate;
};

vct creareStivaVector(int capacitate) {
	vct sv;
	sv.items = malloc(sizeof(card) * capacitate);
	sv.top = -1;
	sv.capacitate = capacitate;
	return sv;
}

void push(vct* sv, card c) {
	if (sv->top < sv->capacitate - 1) {
		sv->items[++sv->top] = c;
	}
}
card pop(vct* sv) {
	card rez;
	rez.ID = -1;
	if (sv->top >= 0) {
		rez = sv->items[sv->top--];
	}
	return rez;
}

int emptyStack(vct* sv) {
	return sv->top == -1;
}

vct citireStackCardsDinFisier(const char numeFisier, int capacitate) {
	FILE* file = fopen(numeFisier, "r");
	vct sv = creareStivaVector(capacitate);
	while (!feof(file)) {
		push(&sv, citireCardDinFisier(file));
	}
	fclose(file);
	return sv;
}

void dezalocareStivaDeCard(vct* sv) {
	for (int i = 0; i <= sv->top; i++) {
		free(sv->items[i].Name);
		free(sv->items[i].Type);
		free(sv->items[i].Character);
	}
	free(sv->items);
	sv->top = -1;
}

//metode de  procesare
card getCardByID(vct sv, int id) {
	card rez;
	rez.ID = -1;
	for (int i = 0; i <= sv.top; i++) {
		if (sv.items[i].ID == id)
			rez = sv.items[i];
	}
	return rez;
};

void afisareStackCards(vct sv) {
	for (int i = 0; i <= sv.top; i++) {
		afisareCard(sv.items[i]);
	}
}


int main() {
	return 0;
}

