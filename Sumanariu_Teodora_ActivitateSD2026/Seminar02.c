#include<stdio.h>
#include<malloc.h>
// seminar 2 vectori 
// un vector utilizeaza memorie contigua (toate el vor fi alocate intrun singur bloc) de memorie si elementele sunt similare
struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id, int RAM, char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = 1;
	t.RAM = RAM;
	t.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;

}

void afisare(struct Telefon t) {
	//afisarea tuturor atributelor.
	printf("ID: %d\n", t.id);
	printf("RAM: %d\n", t.RAM);
	printf("producator: %d\n", t.producator);
	printf("pret: %d\n", t.pret);
	printf("seira: %d\n", t.serie);

}

void afisareVector(struct Telefon* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Telefon* vectorNou = NULL;

	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
}

void copiazaAnumiteElemente(struct Telefon* vector, char nrElemente, float prag, struct Telefon** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
}

struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Telefon s;
	s.id = 1;

	return s;
}



int main() {
	struct Telefon t;
	t = initializare(1, 64, "Samsung", 1200, 'S');
	afisare(t);
	return 0;
}