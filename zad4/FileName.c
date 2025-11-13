#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

typedef struct _polinom* position;
typedef struct _polinom {

	int koef;
	int eks;

	position next;

}polinom;

position create(int koeficijent, int exponent);
void insertSorted(position head, int koef, int exp);
void readFromDat(position head, const char* dat);
void print(position head);
int sum(position, position, position);
int product(position, position, position);

int main() {

	polinom head1 = { 0, 0, NULL };
	polinom head2 = { 0, 0, NULL };
	polinom heads = { 0, 0, NULL };
	polinom headp = { 0, 0, NULL };


	readFromDat(&head1, "pol1.txt");
	readFromDat(&head2, "pol2.txt");


	print(&head1);

	printf("\n");

	//print(&head2);

	printf("\n");

	sum(&head1, &head2, &heads);
	print(&heads);
	printf("\n");

	product(&head1, &head2, &headp);
	print(&headp);




	return 0;
}

position create(int koef, int eks) {

	position newPolynom = NULL;
	newPolynom = malloc(sizeof(polinom));
	if (newPolynom == NULL) {
		printf("Greska u alokaciji memorije (create)\n");
		return NULL;
	}

	newPolynom->koef = koef;
	newPolynom->eks = eks;
	newPolynom->next = NULL;

	return newPolynom;
}

void insertSorted(position head, int koef, int exp) {
	position temp = head;

	while (temp->next != NULL && temp->next->eks < exp) {
		temp = temp->next;
	}

	if (temp->next != NULL && temp->next->eks == exp) {
		temp->next->koef += koef;

		if (temp->next->koef == 0) {
			position toDelete = temp->next;
			temp->next = temp->next->next;
			free(toDelete);
		}
	}
	else {
		position newPolynom = create(koef, exp);
		newPolynom->next = temp->next;
		temp->next = newPolynom;
	}
}


void readFromDat(position head, const char* dat) {

	int koef = 0, exp = 0;
	char buffer[MAX];

	FILE* fp;
	fp = fopen(dat, "r");
	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke (readFromDat)\n");
		return;
	}

	if (fgets(buffer, MAX, fp) != NULL) {
		char* ptr = buffer;


		while (sscanf(ptr, "%d %d", &koef, &exp) == 2) {
			insertSorted(head, koef, exp);

			for (int i = 0; i < 2; i++) {
				while (*ptr != ' ' && *ptr != '\0')
					ptr++;
				while (*ptr == ' ')
					ptr++;
			}
		}
	}

	fclose(fp);
}

void print(position head) {

	position temp = head->next;

	while (temp != NULL) {
		printf("%d^%d", temp->koef, temp->eks);
		temp = temp->next;
		if (temp != NULL)printf(" + ");
	}

}

int sum(position p1, position p2, position s) {
	position temp1 = p1->next;
	position temp2 = p2->next;
	position tempProv = NULL;

	while (temp1 != NULL && temp2 != NULL) {
		if (temp1->eks == temp2->eks) {
			insertSorted(s, temp1->koef + temp2->koef, temp1->eks);
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
		else if (temp1->eks > temp2->eks) {
			insertSorted(s, temp1->koef, temp1->eks);
			temp1 = temp1->next;
		}
		else {
			insertSorted(s, temp2->koef, temp2->eks);
			temp2 = temp2->next;
		}

	}


	if (temp1 != NULL)
		tempProv = temp1;
	else tempProv = temp2;
	while (tempProv != NULL) {
		insertSorted(s, tempProv->koef, tempProv->eks);
		tempProv = tempProv->next;
	}

	return 0;
}

int product(position p1, position p2, position p) {
	position a = p1->next;
	position b = NULL;
	while (a != NULL) {
		b = p2->next;
		while (b != NULL) {
			int newk = a->koef * b->koef;
			int newe = a->eks + b->eks;
			insertSorted(p, newk, newe);
			b = b->next;
		}
		a = a->next;
	}
	return 0;
}