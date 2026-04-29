#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
struct Nod {
	Masina info;
	Nod* left;
	Nod* right;
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


void adaugaMasinaInArbore(Nod** root, Masina masinaNoua) {
	if (*root) {
		if ((*root)->info.id > masinaNoua.id)
			adaugaMasinaInArbore(&(*root)->left, masinaNoua);
		if ((*root)->info.id < masinaNoua.id)
			adaugaMasinaInArbore(&(*root)->right, masinaNoua);
	}
	else {
		Nod* aux = (Nod*)malloc(sizeof(Nod));
		aux->info = masinaNoua;
		aux->left = NULL;
		aux->right = NULL;
		*root = aux;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* root = NULL;
	if (file) {
		while (!feof(file)) {
			Masina maux = citireMasinaDinFisier(file);
			adaugaMasinaInArbore(&root, maux);
		}
	}
	return root;
}

void afisarePreOrdine(Nod* root) {
	//preordine - RSD
	if (root != NULL) {
		afisareMasina(root->info);
		afisarePreOrdine(root->left);
		afisarePreOrdine(root->right);
	}
}

void afisareInOrdine(Nod* root) {
	//inordine - SRD
	if (root != NULL) {
		afisareInOrdine(root->left);
		afisareMasina(root->info);
		afisareInOrdine(root->right);
	}
}

void afisarePostOrdine(Nod* root) {
	//Postordine - SDR
	if (root != NULL) {
		afisarePostOrdine(root->left);
		afisarePostOrdine(root->right);
		afisareMasina(root->info);
	}
}

void dezalocareArboreDeMasini(Nod** root) {
	if (*root) {
		dezalocareArboreDeMasini((*root)->left);
		dezalocareArboreDeMasini((*root)->left);
		free((*root)->info.model);
		free((*root)->info.numeSofer);
		free(*root);
		*root = NULL;
	}
}

Masina getMasinaByID(Nod* root, int id) {
	if (!root) {
		Masina m;
		m.id = -1;
		return m;
	}
	else if (id < root->info.id) {
		getMasinaByID(root->left, id);
	}
	else if (id < root->info.id) {
		getMasinaByID(root->right, id);
	}
	else
	{
		Masina m = root->info;
		m.model = malloc(strlen(root->info.model) + 1);
		strcpy(m.model, root->info.model);
		m.numeSofer = malloc(strlen(root->info.numeSofer) + 1);
		strcpy(m.numeSofer, root->info.numeSofer);
		return m;
	}
}

int determinaNumarNoduri(Nod* root) {
	if (root) {
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	}
	else return 0;
}

int calculeazaInaltimeArbore(Nod* root) {
	if (root) {
		return 1 + max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right));
	}
	return 0;
}

float calculeazaPretTotal(Nod* root) {
	if (root) {
		return root->info.pret + calculeazaPretTotal(root->left) + calculeazaPretTotal(root->right);
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* root, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (root) {
		if (strcmp(root->info.numeSofer, numeSofer) == 0) {
			return root->info.pret + calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) + calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		}
		else calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) + calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
	}
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini.txt");
	afisareInOrdine(radacina);
	printf("\n");
	afisareMasina(getMasinaByID(radacina, 3));
	printf("\nNumar noduri: %d", determinaNumarNoduri(radacina));
	printf("\nInaltime arbor: %d", calculeazaInaltimeArbore(radacina));
	printf("\nPretul total al masinilor din arbor e: %5.2f", calculeazaPretTotal(radacina));
	printf("\nPretul total al masinilor soferului %s e: %5.2f", "Ionescu", calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu"));

	return 0;
}