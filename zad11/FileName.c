#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11

typedef struct _citytree* CityTreePosition;
typedef struct _citytree {
    char name[20];
    int population;
    CityTreePosition left;
    CityTreePosition right;
} CityTree;

typedef struct _country* CountryPosition;
typedef struct _country {
    char name[20];
    CityTreePosition cities;
    CountryPosition next;
} Country;

/* hash funkcija */
int hashFunction(const char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += (int)name[i];
    return sum % TABLE_SIZE;
}

/* stablo gradova */
CityTreePosition insertCity(CityTreePosition root, const char* name, int population) {
    if (root == NULL) {
        CityTreePosition newCity = (CityTreePosition)malloc(sizeof(CityTree));
        strcpy(newCity->name, name);
        newCity->population = population;
        newCity->left = newCity->right = NULL;
        return newCity;
    }

    if (population < root->population ||
        (population == root->population && strcmp(name, root->name) < 0))
        root->left = insertCity(root->left, name, population);
    else
        root->right = insertCity(root->right, name, population);

    return root;
}

/* ucitavanje gradova */
void loadCities(CountryPosition country, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char city[20];
    int population;

    while (fscanf(fp, "%s %d", city, &population) == 2)
        country->cities = insertCity(country->cities, city, population);

    fclose(fp);
}

/* umetanje drzave u sortiranu listu */
void insertCountry(CountryPosition* head, const char* name, const char* cityfile) {
    CountryPosition newCountry = (CountryPosition)malloc(sizeof(Country));
    strcpy(newCountry->name, name);
    newCountry->cities = NULL;
    newCountry->next = NULL;

    loadCities(newCountry, cityfile);

    if (*head == NULL || strcmp(name, (*head)->name) < 0) {
        newCountry->next = *head;
        *head = newCountry;
        return;
    }

    CountryPosition current = *head;
    while (current->next != NULL && strcmp(name, current->next->name) > 0)
        current = current->next;

    newCountry->next = current->next;
    current->next = newCountry;
}

/* ispis stabla gradova */
void printCities(CityTreePosition root) {
    if (!root) return;

    printCities(root->left);
    printf("    %s (%d)\n", root->name, root->population);
    printCities(root->right);
}

/* ispis hash tablice */
void printHashTable(CountryPosition table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("\n[%d]\n", i);
        CountryPosition current = table[i];
        while (current) {
            printf(" Drzava: %s\n", current->name);
            printCities(current->cities);
            current = current->next;
        }
    }
}

int main() {
    CountryPosition hashTable[TABLE_SIZE] = { NULL };

    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) {
        printf("Greska pri otvaranju drzave.txt\n");
        return -1;
    }

    char country[20], cityfile[20];

    while (fscanf(fp, "%s %s", country, cityfile) == 2) {
        int index = hashFunction(country);
        insertCountry(&hashTable[index], country, cityfile);
    }

    fclose(fp);

    printHashTable(hashTable);

    return 0;
}
