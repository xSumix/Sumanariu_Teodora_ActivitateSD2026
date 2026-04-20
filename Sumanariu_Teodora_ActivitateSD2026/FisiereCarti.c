#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cards {
	int Id;
	int Damage;
	int Block;
	char* Name;
	char* Type;
	char* Character;
	char Rarity;
	float Price;
};
typedef struct Cards Cards;

Cards initializare(int id, int damage, int block, char* name, char* type, char* character, char rarity, float price) {
	Cards c;
	c.Id = id;
	c.Damage = damage;
	c.Block = block;
	c.Name = (char*)malloc((strlen(name) + 1) * sizeof(char));
	strcpy(c.Name, name);
	c.Type = (char*)malloc((strlen(type) + 1) * sizeof(char));
	strcpy(c.Type, type);
	c.Character = (char*)malloc((strlen(character) + 1) * sizeof(char));
	strcpy(c.Character, character);
	c.Rarity = rarity;
	c.Price = price;
	return c;
}
void afisareCard(Cards c) {
	//afisarea tuturor atributelor.
	printf("Id: %d \n", c.Id);
	if (c.Damage > 0) printf("Damage: %d \n", c.Damage);
	if (c.Block > 0) printf("Block: %d \n", c.Block);
	printf("Name: %s \n", c.Name);
	printf("Type: %s \n", c.Type);
	printf("Character: %s \n", c.Character);
	printf("Rarity: %c \n", c.Rarity);
	printf("Price: %5.2f \n", c.Price);
}
void afisareVectorCards(Cards* cards, int nrCards) {
	if (!cards || nrCards == 0)
	{
		printf("Vectorul este gol!");
	}
	else
	{
		printf("\n");
		for (int i = 0; i < nrCards; i++)
			afisareCard(cards[i]);
	}
}
void adaugaCarteInVector(Cards** deck, int* nrCards, Cards carteNoua) {
	Cards* aux = (Cards*)malloc(((*nrCards) + 1) * sizeof(Cards));
	for (int i = 0; i < *nrCards; i++) {
		aux[i] = (*deck)[i];
	}
	aux[*nrCards] = carteNoua;
	if (*deck) free(*deck);
	*deck = aux;
	(*nrCards)++;
}

Cards citireCarteFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	if (!file) {
		printf("nu exista fisierul");
		return;
	}

	char linie[256];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";

	Cards carte;

	carte.Id = atoi(strtok(linie, delimitator));
	carte.Damage = atoi(strtok(NULL, delimitator));
	carte.Block = atoi(strtok(NULL, delimitator));

	char* aux = strtok(NULL, delimitator);
	carte.Name = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(carte.Name, aux);

	aux = strtok(NULL, delimitator);
	carte.Type = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(carte.Type, aux);

	aux = strtok(NULL, delimitator);
	carte.Character = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(carte.Character, aux);

	carte.Rarity = strtok(NULL, delimitator)[0];

	carte.Price = atof(strtok(NULL, delimitator));
	return carte;
}

Cards* citireVectorCartiFisier(const char* numeFisier, int* nrCardsCitite) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("nu exista fisierul");
		return NULL;
	}

	Cards* deck = NULL;
	*nrCardsCitite = 0;

	while (!feof(file)) {
		Cards c = citireCarteFisier(file);
		adaugaCarteInVector(&deck, nrCardsCitite, c);
	}

	fclose(file);
	return deck;
}

void dezalocareVectorCarti(Cards** deck, int* nrCards) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i < *nrCards; i++) {
		free((*deck)[i].Name);
		free((*deck)[i].Type);
		free((*deck)[i].Character);
	}
	*nrCards = 0;
}

int main() {
	/*FILE* file = fopen("cards.txt", "r");
	Cards card = citireCarteFisier(file);
	afisareCard(card);*/

	int nrCarti;
	Cards* Deck = citireVectorCartiFisier("cards.txt", &nrCarti);
	afisareVectorCards(Deck, nrCarti);
	return 0;
}