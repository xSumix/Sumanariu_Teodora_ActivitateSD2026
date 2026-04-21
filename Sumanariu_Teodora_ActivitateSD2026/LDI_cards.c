#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

typedef struct card card;
typedef struct nod nod;
typedef struct ldi ldi;

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

struct nod {
	card item;
	nod* next;
	nod* back;
};

struct ldi {
	nod* first;
	nod* last;
};

card citireCarteDinFisier(FILE* file) {
	char linie[256];
	fgets(linie, 255, file);
	char delim[3] = ",\n";
	card carte;
	carte.ID = atoi(strtok(linie, delim));
	carte.Damage = atoi(strtok(NULL, delim));
	carte.Block = atoi(strtok(NULL, delim));
	char* aux = strtok(NULL, delim);
	carte.Name = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(carte.Name, aux);
	aux = strtok(NULL, delim);
	carte.Type = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(carte.Type, aux);
	aux = strtok(NULL, delim);
	carte.Character = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(carte.Character, aux);
	carte.Rarity = (strtok(NULL, delim))[0];
	carte.Price = atof(strtok(NULL, delim));
	return carte;
}

void afisareCarte(card card) {
	printf("%-4d | %-20s | %-8s | %-10s | %c | %5.2f",
		card.ID, card.Name, card.Type, card.Character, card.Rarity, card.Price);

	if (card.Damage > 0) printf(" | DMG: %3d", card.Damage);
	if (card.Block > 0) printf(" | BLK: %3d", card.Block);

	printf("\n");
}

void afisareListaSE(ldi list) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	nod* aux = list.first;
	while (aux)
	{
		afisareCarte(aux->item);
		aux = aux->next;
	}
}

void afisareListaES(ldi list) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	nod* aux = list.last;
	while (aux)
	{
		afisareCarte(aux->item);
		aux = aux->back;
	}
}

void adaugaCarteInLista(ldi* list, card newCard) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	nod* aux = (nod*)malloc(sizeof(nod));
	aux->item = newCard;
	aux->back = list->last;
	aux->next = NULL;

	if (list->last)
		list->last->next = aux;
	else
		list->first = aux;
	list->last = aux;
}

void adaugaLaInceputInLista(ldi* list, card newCard) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	nod* aux = (nod*)malloc(sizeof(nod));
	aux->item = newCard;
	aux->back = NULL;
	aux->next = list->first;
	if (list->first)
		list->first->back = aux;
	else
		list->last = aux;
	list->first = aux;
}

ldi citireLDCartiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	if (!file) {
		printf("Eroare: fisierul '%s' nu a putut fi deschis!\n", numeFisier);
		ldi empty = { NULL, NULL };
		return empty;
	}

	ldi list;
	list.first = NULL;
	list.last = NULL;
	while (!feof(file)) {
		card c = citireCarteDinFisier(file);
		adaugaCarteInLista(&list, c);
	}
	fclose(file);
	return list;
}

void dezalocareLDMasini(ldi* list) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	nod* head = list->first;
	while (head) {
		free(head->item.Name);
		free(head->item.Type);
		free(head->item.Character);
		nod* aux = head;
		head = head->next;
		free(aux);
	}
	list->first = NULL;
	list->last = NULL;
}

float calculeazaPretMediu(ldi list) {
	//calculeaza pretul mediu al masinilor din lista.
	nod* aux = list.first;
	float pretMediu = 0;
	int nrCards = 0;
	while (aux) {
		pretMediu += aux->item.Price;
		nrCards++;
		aux = aux->next;
	}
	pretMediu /= nrCards;
	return pretMediu;
}

void stergeCarteDupaID(ldi* list, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	nod* aux = list->first;
	while (aux) {
		if (aux->item.ID == id) {
			if (aux->back) {
				aux->back->next = aux->next;
			}
			else {
				list->first = aux->next;
			}
			if (aux->next) {
				aux->next->back = aux->back;
			}
			else {
				list->last = aux->back;
			}
			free(aux->item.Name);
			free(aux->item.Type);
			free(aux->item.Character);
			free(aux);
			return;
		}
		aux = aux->next;
	}
}

char* getNumeCaracterCartiScumpe(ldi list) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	nod* aux = list.first;
	char* numeChar = NULL;
	float max = 0;
	while (aux) {
		if (aux->item.Price > max) {
			max = aux->item.Price;
			free(numeChar);
			numeChar = (char*)malloc((strlen(aux->item.Character) + 1) * sizeof(char));
			strcpy(numeChar, aux->item.Character);
		}
		aux = aux->next;
	}
	return numeChar;
}

int comparaNume(card a, card b) {
	return strcmp(a.Name, b.Name);
}

void sortLDListNumeAlfabetic(ldi* list) {
	if (!list || !list->first || !list->first->next) return;

	nod* newlist = NULL;

	nod* actual = list->first;
	while (actual) {
		nod* urm = actual->next;

		actual->next = NULL;
		actual->back = NULL;

		if (!newlist || comparaNume(actual->item, newlist->item) < 0) {
			actual->next = newlist;
			if (newlist) newlist->back = actual;
			newlist = actual;
		}
		else {
			nod* aux = newlist;
			while (aux->next && comparaNume(actual->item, aux->next->item) > 0) aux = aux->next;
			actual->next = aux->next;
			actual->back = aux;
			if (aux->next) aux->next->back = actual;
			aux->next = actual;

		}

		actual = urm;
	}

	list->first = newlist;
	nod* aux = newlist;
	while (aux->next) aux = aux->next;
	list->last = aux;

}

int main() {

	//FILE* file = fopen("cards.txt", "r");
	//card c = citireCarteDinFisier(file);
	//afisareCarte(c);
	//fclose(file);

	ldi deck = citireLDCartiDinFisier("cards.txt");
	/*afisareListaES(deck);
	afisareListaSE(deck);*/
	//float pret = calculeazaPretMediu(deck);
	//printf("%5.2f", pret);
	/*stergeCarteDupaID(&deck, 90);
	afisareListaSE(deck);*/
	//printf("caracterul cu cea mai scumap carte e: %s", getNumeCaracterCartiScumpe(deck));
	sortLDListNumeAlfabetic(&deck);
	afisareListaSE(deck);

	return 0;
}