#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Card Card;
typedef struct Nod Nod;

struct Card {
	int ID;
	int Damage;
	int Block;
	char* Name;
	char* Type;
	char* Character;
	char Rarity;
	float Price;
};

struct Nod {
	Card info;
	Nod* next;
};

Card citireCarteDinFisier(FILE* file) {
	char linie[256];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";
	Card carte;

	carte.ID = atoi(strtok(linie, delimitator));
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

Card initializare(int id, int damage, int block, char* name, char* type, char* character, char rarity, float price) {
	Card c;
	c.ID = id;
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

void afisareCarte(Card c) {
	printf("Id: %d \n", c.ID);
	if (c.Damage > 0) printf("Damage: %d \n", c.Damage);
	if (c.Block > 0) printf("Block: %d \n", c.Block);
	printf("Name: %s \n", c.Name);
	printf("Type: %s \n", c.Type);
	printf("Character: %s \n", c.Character);
	printf("Rarity: %c \n", c.Rarity);
	printf("Price: %5.2f \n", c.Price);
}

void afisareListaCarti(Nod* head) {
	while (head) {
		afisareCarte(head->info);
		head = head->next;
	}
}

void adaugaCarteInLista(Nod** head, Card carteNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* aux = malloc(sizeof(Nod));
	aux->info = carteNoua;
	aux->next = NULL;

	if (!(*head)) *head = aux;
	else {
		Nod* auxi = *head;
		while (auxi->next) {
			auxi = auxi->next;
		}
		auxi->next = aux;

	}
}

void adaugaLaInceputInLista(Nod** head, Card carteNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* aux = malloc(sizeof(Nod));
	aux->info = carteNoua;
	aux->next = *head;
	*head = aux;
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* head = NULL;
	if (!file) {
		printf("fisierul nu exista");
	}
	else {
		while (!feof(file)) {
			adaugaCarteInLista(&head, citireCarteDinFisier(file));
		}
	}
	fclose(file);
	return head;
}

void dezalocareListaMasini(Nod** head) {
	//sunt dezalocate toate masinile si lista de elemente
	while ((*head)) {
		free((*head)->info.Name);
		free((*head)->info.Type);
		free((*head)->info.Character);
		Nod* aux = (*head)->next;
		free(*head);
		*head = aux;
	}
}

float calculeazaPretMediu(Nod* head) {
	//calculeaza pretul mediu al masinilor din lista.
	int nrCarti = 0;
	float suma = 0;
	while (head) {
		suma += head->info.Price;
		nrCarti++;
		head = head->next;
	}
	if (nrCarti > 0) {
		return suma / nrCarti;
	}
	return 0;
}

void stergeCartiDinRarity(Nod** head, char RarityCautata) {
	if (head == NULL || *head == NULL) return;
	while ((*head) && (*head)->info.Rarity == RarityCautata) {
		Nod* aux = *head;
		*head = (*head)->next;
		free(aux);
	}
	//stergem cartile cautate care se afla la inceputul listei
	Nod* curent = *head;
	Nod* anterior = NULL;
	while (curent) {
		if (curent->info.Rarity == RarityCautata) {
			anterior->next = curent->next;
			free(curent);
			curent = anterior->next;
		}
		else {
			anterior = curent;
			curent = curent->next;
		}
	}
}

float calculeazaPretulCartilorUnuiCaracter(Nod* head, const char* numeCaracter) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	while (head) {
		if (strcmp(head->info.Character, numeCaracter) == 0)
			suma += head->info.Price;
		head = head->next;
	}
	return suma;
}

int main() {
	/*FILE* file = fopen("cards.txt", "r");
	Card c = citireCarteDinFisier(file);
	afisareCarte(c);
	fclose(file);*/

	Nod* Deck;
	Deck = citireListaMasiniDinFisier("cards.txt");
	//afisareListaCarti(Deck);

	Card newCard = initializare(91, 0, 0, "Limit Break", "Skill", "Ironclad", 'R', 11.50);
	//afisareCarte(newCard);
	//adaugaLaInceputInLista(&Deck, newCard);
	//printf("Suma cartilor caracterului %s este %5.2f", "Ironclad", calculeazaPretulCartilorUnuiCaracter(Deck, "Ironclad"));
	adaugaCarteInLista(&Deck, newCard);
	//afisareListaCarti(Deck);
	//printf("Pretul mediu al cartilor caracterului %s este %5.2f", "Ironclad", calculeazaPretulCartilorUnuiCaracter(Deck,"Ironclad"));
	//printf("Suma cartilor caracterului %s este %5.2f", "Ironclad", calculeazaPretulCartilorUnuiCaracter(Deck,"Ironclad"));
	stergeCartiDinRarity(&Deck, 'U');
	afisareListaCarti(Deck);
	return 0;
}