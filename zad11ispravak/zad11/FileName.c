#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define HASH_SIZE 11


struct citytree;
typedef struct citytree* citytreeposition;
typedef struct citytree {
    int population;
    char name[20];
    citytreeposition left;
    citytreeposition right;
} citytree;


struct countrylist;
typedef struct countrylist* countrylistposition;
typedef struct countrylist {
    char name[20];
    citytreeposition cities;
    countrylistposition next;
} countrylist;


int hashFunction(const char* name);
citytreeposition insertcity(citytreeposition root, const char* name, int population);
countrylistposition createcountrynode(const char* name, const char* cityfile);
countrylistposition insertcountrysorted(countrylistposition head, countrylistposition newnode);

void searchcities(citytreeposition root, int minpopulation);
void printcities(citytreeposition root);
void printhashtable(countrylistposition hashTable[]);
countrylistposition findcountry(countrylistposition hashTable[], const char* name);

void freecitytree(citytreeposition root);
void freecountrylist(countrylistposition head);


int main() {

    FILE* fp = fopen("drzave.txt", "r");
    if (!fp) {
        printf("Greska u otvaranju datoteke drzave.txt\n");
        return FILE_OPEN_ERROR;
    }

    countrylistposition hashTable[HASH_SIZE] = { NULL };

    char countryname[20], cityfile[20];

    while (fscanf(fp, "%s %s", countryname, cityfile) == 2) {
        int index = hashFunction(countryname);

        countrylistposition newcountry = createcountrynode(countryname, cityfile);
        hashTable[index] = insertcountrysorted(hashTable[index], newcountry);
    }
    fclose(fp);

    printhashtable(hashTable);

    char searchcountry[20];
    int minpopulation;

    printf("\nUnesite drzavu: ");
    scanf("%s", searchcountry);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minpopulation);

    countrylistposition found = findcountry(hashTable, searchcountry);
    if (!found) {
        printf("Drzava nije pronadena!\n");
    }
    else {
        printf("Gradovi u drzavi %s s vise od %d stanovnika:\n",
            searchcountry, minpopulation);
        searchcities(found->cities, minpopulation);
    }

    for (int i = 0; i < HASH_SIZE; i++)
        freecountrylist(hashTable[i]);

    return 0;
}


int hashFunction(const char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += name[i];
    return sum % HASH_SIZE;
}


citytreeposition insertcity(citytreeposition root, const char* name, int population) {
    if (!root) {
        citytreeposition newcity = (citytreeposition)malloc(sizeof(citytree));
        strcpy(newcity->name, name);
        newcity->population = population;
        newcity->left = newcity->right = NULL;
        return newcity;
    }

    if (population < root->population ||
        (population == root->population && strcmp(name, root->name) < 0))
        root->left = insertcity(root->left, name, population);
    else
        root->right = insertcity(root->right, name, population);

    return root;
}


countrylistposition createcountrynode(const char* name, const char* cityfile) {
    countrylistposition newcountry = (countrylistposition)malloc(sizeof(countrylist));
    strcpy(newcountry->name, name);
    newcountry->cities = NULL;
    newcountry->next = NULL;

    FILE* fp = fopen(cityfile, "r");
    if (!fp) {
        printf("Greska u otvaranju %s\n", cityfile);
        return NULL;
    }

    char cityname[20];
    int population;

    while (fscanf(fp, "%s %d", cityname, &population) == 2)
        newcountry->cities = insertcity(newcountry->cities, cityname, population);

    fclose(fp);
    return newcountry;
}


countrylistposition insertcountrysorted(countrylistposition head, countrylistposition newnode) {
    if (!head || strcmp(newnode->name, head->name) < 0) {
        newnode->next = head;
        return newnode;
    }

    countrylistposition current = head;
    while (current->next && strcmp(newnode->name, current->next->name) > 0)
        current = current->next;

    newnode->next = current->next;
    current->next = newnode;
    return head;
}

void printcities(citytreeposition root) {
    if (!root) return;
    printcities(root->left);
    printf("\t%s (%d)\n", root->name, root->population);
    printcities(root->right);
}

void searchcities(citytreeposition root, int minpopulation) {
    if (!root) return;
    searchcities(root->left, minpopulation);
    if (root->population > minpopulation)
        printf("\t%s (%d)\n", root->name, root->population);
    searchcities(root->right, minpopulation);
}

void printhashtable(countrylistposition hashTable[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("\nHash %d:\n", i);
        countrylistposition current = hashTable[i];
        while (current) {
            printf("%s:\n", current->name);
            printcities(current->cities);
            current = current->next;
        }
    }
}
countrylistposition findcountry(countrylistposition hashTable[], const char* name) {
    int index = hashFunction(name);
    countrylistposition current = hashTable[index];

    while (current) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
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
        freecitytree(temp->cities);
        free(temp);
    }
}
