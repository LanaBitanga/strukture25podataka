#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

typedef struct _student {
    char firstName[MAX];
    char lastName[MAX];
    int year;
    struct _student* next;
} Student;

typedef Student* Position;

Position createStudent(const char* first, const char* last, int year);
void insertAtBeginning(Position head, Position what);
void insertAtEnd(Position head, Position what);
Position find(const char* last, Position head);
Position findPrev(const char* last, Position head);
void deleteByLastName(const char* last, Position head);
void insertAfterWho(const char* last, Position head, Position what);
void insertBeforeWho(const char* last, Position head, Position what);
void printList(Position head);
void sortList(Position head);
void saveToFile(const char* filename, Position head);
void readFromFile(const char* filename, Position head);

int main(void) {
    Student head = { "", "", 0, NULL };
    Position p = NULL;
    char first[MAX], last[MAX], filename[MAX];
    int year, n, choice;

    printf("Unesite 1 za čitanje iz datoteke ili 2 za unos nove liste: ");
    if (scanf(" %d", &choice) != 1) return 0;

    if (choice == 1) {
        printf("Ime datoteke (npr. Student.txt): ");
        scanf(" %255s", filename);
        readFromFile(filename, &head);
        printList(&head);
    }
    else if (choice == 2) {
        printf("Koliko studenata unosite? ");
        if (scanf(" %d", &n) != 1) n = 0;
        for (int i = 0; i < n; i++) {
            printf("\nIme: "); scanf(" %255s", first);
            printf("Prezime: "); scanf(" %255s", last);
            printf("Godina rođenja: "); scanf(" %d", &year);
            p = createStudent(first, last, year);
            insertAtEnd(&head, p);
        }
        printList(&head);
    }

    while (1) {
        printf("\n--- MENU ---\n");
        printf("B - dodaj na početak\nA - dodaj na kraj\nF - pronađi po prezimenu\nD - obriši po prezimenu\n");
        printf("I - ubaci iza prezimena\nK - ubaci ispred prezimena\nS - sortiraj po prezimenu\nC - spremi u datoteku\nE - izlaz\n");
        printf("Opcija: ");
        char op; scanf(" %c", &op);

        switch (op) {
        case 'b': case 'B':
            printf("Ime: "); scanf(" %255s", first);
            printf("Prezime: "); scanf(" %255s", last);
            printf("Godina rođenja: "); scanf(" %d", &year);
            p = createStudent(first, last, year);
            insertAtBeginning(&head, p);
            printList(&head);
            break;
        case 'a': case 'A':
            printf("Ime: "); scanf(" %255s", first);
            printf("Prezime: "); scanf(" %255s", last);
            printf("Godina rođenja: "); scanf(" %d", &year);
            p = createStudent(first, last, year);
            insertAtEnd(&head, p);
            printList(&head);
            break;
        case 'f': case 'F':
            printf("Prezime za pretragu: "); scanf(" %255s", last);
            p = find(last, &head);
            if (p) printf("Pronađeno: %s %s %d\n", p->firstName, p->lastName, p->year);
            else printf("Osoba ne postoji.\n");
            break;
        case 'd': case 'D':
            printf("Prezime za brisanje: "); scanf(" %255s", last);
            deleteByLastName(last, &head);
            printList(&head);
            break;
        case 'i': case 'I':
            printf("Novi - Ime: "); scanf(" %255s", first);
            printf("Novi - Prezime: "); scanf(" %255s", last);
            printf("Godina rođenja: "); scanf(" %d", &year);
            p = createStudent(first, last, year);
            printf("Ubaci iza prezimena: "); scanf(" %255s", last);
            insertAfterWho(last, &head, p);
            printList(&head);
            break;
        case 'k': case 'K':
            printf("Novi - Ime: "); scanf(" %255s", first);
            printf("Novi - Prezime: "); scanf(" %255s", last);
            printf("Godina rođenja: "); scanf(" %d", &year);
            p = createStudent(first, last, year);
            printf("Ubaci ispred prezimena: "); scanf(" %255s", last);
            insertBeforeWho(last, &head, p);
            printList(&head);
            break;
        case 's': case 'S':
            sortList(&head);
            printList(&head);
            break;
        case 'c': case 'C':
            printf("Ime datoteke za spremanje: "); scanf(" %255s", filename);
            saveToFile(filename, &head);
            break;
        case 'e': case 'E':
            while (head.next) {
                Position tmp = head.next;
                head.next = tmp->next;
                free(tmp);
            }
            return 0;
        default:
            printf("Neispravan unos.\n");
        }
    }
    return 0;
}

Position createStudent(const char* first, const char* last, int year) {
    Position p = (Position)malloc(sizeof(Student));
    if (!p) {
        perror("Alokacija neuspjela");
        return NULL;
    }
    strncpy(p->firstName, first, MAX - 1); p->firstName[MAX - 1] = '\0';
    strncpy(p->lastName, last, MAX - 1); p->lastName[MAX - 1] = '\0';
    p->year = year;
    p->next = NULL;
    return p;
}

void insertAtBeginning(Position head, Position what) {
    what->next = head->next;
    head->next = what;
}

void insertAtEnd(Position head, Position what) {
    Position p = head;
    while (p->next) p = p->next;
    p->next = what;
    what->next = NULL;
}

Position find(const char* last, Position head) {
    Position p = head->next;
    while (p) {
        if (strcmp(p->lastName, last) == 0) return p;
        p = p->next;
    }
    return NULL;
}

Position findPrev(const char* last, Position head) {
    Position p = head;
    while (p->next) {
        if (strcmp(p->next->lastName, last) == 0) return p;
        p = p->next;
    }
    return NULL;
}

void deleteByLastName(const char* last, Position head) {
    Position prev = findPrev(last, head);
    if (!prev) {
        printf("Osoba s prezimenom '%s' nije pronađena.\n", last);
        return;
    }
    Position toDel = prev->next;
    prev->next = toDel->next;
    free(toDel);
    printf("Osoba obrisana.\n");
}

void insertAfterWho(const char* last, Position head, Position what) {
    Position where = find(last, head);
    if (!where) {
        printf("Ciljna osoba nije pronađena. Ubacujem na kraj.\n");
        insertAtEnd(head, what);
        return;
    }
    what->next = where->next;
    where->next = what;
}

void insertBeforeWho(const char* last, Position head, Position what) {
    Position prev = findPrev(last, head);
    if (!prev) {
        printf("Ciljna osoba nije pronađena. Ubacujem na početak.\n");
        insertAtBeginning(head, what);
        return;
    }
    what->next = prev->next;
    prev->next = what;
}

void printList(Position head) {
    Position p = head->next;
    int i = 1;
    printf("\nLista:\n");
    printf("%3s %-15s %-15s %s\n", "#", "Ime", "Prezime", "Godina");
    while (p) {
        printf("%3d %-15s %-15s %d\n", i++, p->firstName, p->lastName, p->year);
        p = p->next;
    }
}

void sortList(Position head) {
    if (!head->next || !head->next->next) return;
    int swapped;
    do {
        swapped = 0;
        Position p = head->next;
        while (p->next) {
            if (strcmp(p->lastName, p->next->lastName) > 0 ||
                (strcmp(p->lastName, p->next->lastName) == 0 &&
                    strcmp(p->firstName, p->next->firstName) > 0)) {
                char tmpFirst[MAX], tmpLast[MAX]; int tmpYear;
                strcpy(tmpFirst, p->firstName); strcpy(tmpLast, p->lastName); tmpYear = p->year;
                strcpy(p->firstName, p->next->firstName); strcpy(p->lastName, p->next->lastName); p->year = p->next->year;
                strcpy(p->next->firstName, tmpFirst); strcpy(p->next->lastName, tmpLast); p->next->year = tmpYear;
                swapped = 1;
            }
            p = p->next;
        }
    } while (swapped);
}

void saveToFile(const char* filename, Position head) {
    FILE* fp = fopen(filename, "w");
    if (!fp) { perror("Ne mogu otvoriti datoteku"); return; }
    Position p = head->next;
    while (p) {
        fprintf(fp, "%s;%s;%d\n", p->firstName, p->lastName, p->year);
        p = p->next;
    }
    fclose(fp);
    printf("Spremljeno u %s\n", filename);
}

void readFromFile(const char* filename, Position head) {
    FILE* fp = fopen(filename, "r");
    if (!fp) { perror("Ne mogu otvoriti datoteku"); return; }
    char line[512];
    while (fgets(line, sizeof line, fp)) {
        char first[MAX] = { 0 }, last[MAX] = { 0 }; int year = 0;
        if (sscanf(line, "%255[^;];%255[^;];%d", first, last, &year) >= 2) {
            Position p = createStudent(first, last, year);
            insertAtEnd(head, p);
        }
    }
    fclose(fp);
}
