#define _CRT_SECURE_NO_WARNINGS
#define NAME_MAX 15
#define SCORE_MAX 50
#define FILE_NOT_OPENED -1
#define ALLOC_ERROR NULL

#include <stdio.h>
#include <stdlib.h>

//ZADATAK 1:
//Napisati program koji prvo procita koliko redaka ima datoteka, tj. koliko ima studenata
//zapisanih u datoteci. Nakon toga potrebno je dinamicki alocirati prostor za niz struktura
//studenata (ime, prezime, bodovi) i ucitati iz datoteke sve zapise. Na ekran ispisati ime,
//prezime, apsolutni i relativni broj bodova.
//Napomena: Svaki redak datoteke sadrzi ime i prezime studenta, te broj bodova na kolokviju.
//relatvan_br_bodova = br_bodova/max_br_bodova*100

typedef struct _Student {

	char name[NAME_MAX];
	char surname[NAME_MAX];
	int score;

}student;

int studentCounter();

student* studentAlloc(int nmbr);

int studentInput(int nmbr);

int studentPrint(int nmbr, student* studList);

float resultRel(int score);

int main() {

	int studCount = studentCounter();

	printf("Broj studenata: %d\n", studCount);

	studentInput(studCount);

	return 0;
}

int studentCounter() {

	int counter = 0;
	char buffName[NAME_MAX] = { "\0" };
	char buffSurn[NAME_MAX] = { "\0" };
	int buffScore = 0;

	FILE* filePointer = fopen("popis.txt", "r");

	if (filePointer == NULL) {
		printf("File cannot be opened.");
		return FILE_NOT_OPENED;
	}

	while (fscanf(filePointer, "%s %s %d", buffName, buffSurn, &buffScore) == 3) {
		counter++;
	}

	fclose(filePointer);

	return counter;
}

student* studentAlloc(int nmbr) {

	student* studArray = (student*)malloc(nmbr * sizeof(student));

	if (studArray == NULL) {
		printf("Memory cannot be allocated.");
		return ALLOC_ERROR;
	}

	return studArray;
}

int studentInput(int nmbr) {

	student* studList = studentAlloc(nmbr);
	FILE* filePointer;

	filePointer = fopen("popis.txt", "r");

	if (filePointer == NULL) {
		printf("File cannot be opened.");
		return FILE_NOT_OPENED;
	}

	int i = 0;
	for (i = 0; i < nmbr; i++) {
		fscanf(filePointer, "%s %s %d", studList[i].name, studList[i].surname, &studList[i].score);
	}

	studentPrint(nmbr, studList);

	fclose(filePointer);
	free(studList);

	return 0;
}

int studentPrint(int nmbr, student* studList) {

	printf("\nStudenti:\n\nIme: \tPrezime: \tApsolutni bodovi: \tRelativni bodovi:\n\n");

	int i = 0;
	for (i = 0; i < nmbr; i++) {
		printf("%s \t%s \t\t%d \t\t\t%f\n", studList[i].name, studList[i].surname, studList[i].score, resultRel(studList[i].score));
	}

	return 0;
}

float resultRel(int score) {

	float rel = (float)score / SCORE_MAX * 100;

	return rel;
}