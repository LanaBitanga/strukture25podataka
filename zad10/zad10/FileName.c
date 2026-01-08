﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

struct citytree;
typedef struct _citytree* citytreeposition;
typedef struct _citytree {

	int population;
	char name[20];
	citytreeposition left;
	citytreeposition right;

}citytree;


typedef struct _countrylist* countrylistposition;
typedef struct _countrylist {

	char name[20];
	countrylistposition next;
	citytreeposition cities;                     // lista drzava u sebi ima pointer na stablo gradova

}countrylist;


typedef struct _citylist* citylistposition;
typedef struct _citylist {

	int population;
	char name[20];
	citylistposition next;

}citylist;


typedef struct _countrytree* countrytreeposition;
typedef struct _countrytree {

	char name[20];
	countrytreeposition left;
	countrytreeposition right;
	countrylistposition cityhead; // stablo drzava u sebi ima pointer na listu gradova

}countrytree;


countrytreeposition insertcountrytree(countrytreeposition root, const char* name);
citytreeposition insertcity(citytreeposition root, const char* name, int population);
countrylistposition createcountrynode(const char* name, const char* cityfile);
void printcountries(countrytreeposition root);
void searchcities(citytreeposition root, int minpopulation);
void freecitytree(citytreeposition root);
void freecountrylist(countrylistposition head);
void freecountrytree(countrytreeposition root);

int main() {

	FILE* fp = fopen("drzave.txt", "r");

	if (fp == NULL) {
		printf("Greska u otvaranju datoteke drzave.txt.\n");
		return FILE_OPEN_ERROR;
	}

	countrytreeposition countrytree = NULL;
	char countryname[20], cityfile[20];

	while (fscanf(fp, "%s %s", countryname, cityfile) == 2) {

		countrytree = insertcountrytree(countrytree, countryname);
		countrylistposition newcountry = NULL;													 //novi cvor u listi država
		newcountry = createcountrynode(countryname, cityfile);;
		newcountry->next = countrytree->cityhead;
		countrytree->cityhead = (citylistposition)newcountry;
		//novi cvor liste drzava je novi cvor gradova te drzave
	}
	fclose(fp);

	printcountries(countrytree);

	int minpopulation;
	printf("Unesite neki broj stanovnika: ");
	scanf(" %d", &minpopulation);

	char searchcountry[20];
	printf("Unesite naziv drzave za pretragu: ");
	scanf("%c", searchcountry);

	countrytreeposition current = countrytree;

	while (current != NULL) {

		int rezultat = strcmp(searchcountry, current->name);
		if (rezultat == 0) {
			printf("Gradovi u unesenoj drzavi sa vecim brojem stanovnika od %d:\n ", minpopulation);
			searchcities(current->cityhead->cities, minpopulation);
			break;
		}

		else if (rezultat < 0) {
			current = current->left;
		}

		else {
			current = current->right;
		}
	}

	freecountrytree(countrytree);

	return 0;
}


countrytreeposition insertcountrytree(countrytreeposition root, const char* name) {                         //ubacivanje novog cvora drzava u stablo drzava

	if (root == NULL) {

		countrytreeposition newnode = (countrytreeposition)malloc(sizeof(countrytree));

		if (newnode == NULL) {
			printf("Malloc greska.\n");
			return NULL;
		}

		strcpy(newnode->name, name);
		newnode->left = NULL;
		newnode->right = NULL;
		newnode->cityhead = NULL;
		return newnode;

	}
	if (strcmp(name, root->name) < 0) {
		root->left = insertcountrytree(root->left, name);
	}

	else {
		root->right = insertcountrytree(root->right, name);
	}

	return root;
}


citytreeposition insertcity(citytreeposition root, const char* name, int population) {

	if (root == NULL)
		return NULL;

	else {

		citytreeposition newcity = (citytreeposition)malloc(sizeof(citytree));
		if (newcity == NULL) {
			printf("Malloc greska.\n");
			return NULL;
		}

		strcpy(newcity->name, name);
		newcity->population = population;
		newcity->left = NULL;
		newcity->right = NULL;
		return newcity;
	}

	if (population < root->population || (population == root->population && strcmp(name, root->name) < 0)) {

		root->left = insertcity(root->left, name, population);
	}

	else {
		root->right = insertcity(root->right, name, population);
	}
	return root;

}

countrylistposition createcountrynode(const char* name, const char* cityfile) {										//ubacivanje novog cvora drzava u LISTU drzava

	countrylistposition newcountry = (countrylistposition)malloc(sizeof(countrylist));

	if (newcountry == NULL) {
		printf("Malloc greska.\n");
		return NULL;
	}

	strcpy(newcountry->name, name);
	newcountry->next = NULL;
	newcountry->cities = NULL;

	FILE* filecity = fopen(cityfile, "r");

	if (filecity == NULL) {
		printf("Greska u otvaranju datoteke.\n");
		return FILE_OPEN_ERROR;
	}

	int population;
	char cityname[20];

	while (fscanf(filecity, "%s %d", cityname, &population) == 2) {

		newcountry->cities = insertcity(newcountry->cities, cityname, population);

	}

	fclose(filecity);

	return newcountry;

}

void printcountries(countrytreeposition root) {										//void - return; 

	if (root == NULL)
		return;

	else {
		printcountries(root->left);
		printf("Drzava: %s\n", root->name);
		countrylistposition current = root->cityhead;                     //current pokazuje na prvi čvor vezane liste gradova

		while (current != NULL) {
			printf(" Gradovi:\n");
			searchcities(current->cities, 0);
			current = current->next;
		}

		printcountries(root->right);
	}

}

void searchcities(citytreeposition root, int minpopulation) {

	if (root == NULL)
		return;

	else {

		searchcities(root->left, minpopulation);
		if (root->population > minpopulation) {
			printf("Grad %s, broj stanovnika: %d\n", root->name, root->population);
		}
		searchcities(root->left, minpopulation);

	}
}

void freecitytree(citytreeposition root) {
	if (root) {
		freecitytree(root->left);
		freecitytree(root->right);
		free(root);
	}
}

void freecountrylist(countrylistposition head) {
	while (head) {
		countrylistposition temp = head;
		head = head->next;
		freecountrylist(temp->cities);
		free(temp);
	}
}

void freecountrytree(countrytreeposition root) {
	if (root) {
		freecountrytree(root->left);
		freecountrytree(root->right);
		freecountrytree((countrylistposition)root->cityhead);
		free(root);
	}
}