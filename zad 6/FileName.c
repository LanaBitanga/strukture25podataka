#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define DATE_EQ 0
#define DATE_LATER 1
#define DATE_EARLIER -1

typedef struct DateTime {
    int year;
    int month;
    int day;
} DateTime;

typedef struct Item* itemPosition;
typedef struct Item {
    char name[50];
    int quantity;
    float price;
    itemPosition next;
} Item;

typedef struct Receipt* receiptPosition;
typedef struct Receipt {
    DateTime date;
    itemPosition items;
    receiptPosition next;
} Receipt;

DateTime setDate(int year, int month, int day) {
    DateTime date;
    date.year = year;
    date.month = month;
    date.day = day;
    return date;
}

int compareDates(DateTime d1, DateTime d2);
int printDateTime(DateTime d);
receiptPosition createReceipt(DateTime date);
itemPosition createItem(const char* name, int quantity, float price);
receiptPosition addReceipt(receiptPosition head, receiptPosition newReceipt);
itemPosition addItem(itemPosition head, itemPosition newItem);
receiptPosition loadReceipt(const char* fileName);
receiptPosition loadAllReceipts(const char* fileName);
void calculateTotal(receiptPosition receipts, const char* itemName, DateTime startDate, DateTime endDate);
void printItemNames(itemPosition items, char printedItems[][50], int* printedCount);
void freeItems(itemPosition items);
void freeReceipts(receiptPosition receipts);


int main() {
    receiptPosition receipts = NULL;

    receipts = loadAllReceipts("racuni.txt");
    if (receipts == NULL) {
        return 1;
    }

    printf("All Dates:\n");
    receiptPosition currentReceipt = receipts;
    while (currentReceipt != NULL) {
        printDateTime(currentReceipt->date);
        currentReceipt = currentReceipt->next;
    }

    printf("\nAll Items:\n");
    char printedItems[MAX_SIZE][50];
    int printedCount = 0;
    currentReceipt = receipts;
    while (currentReceipt != NULL) {
        printItemNames(currentReceipt->items, printedItems, &printedCount);
        currentReceipt = currentReceipt->next;
    }

    char itemName[50];
    int startYear, startMonth, startDay;
    int endYear, endMonth, endDay;

    printf("Enter item name: ");
    scanf("%49s", itemName);
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%d-%d-%d", &startYear, &startMonth, &startDay);
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%d-%d-%d", &endYear, &endMonth, &endDay);

    DateTime startDate = setDate(startYear, startMonth, startDay);
    DateTime endDate = setDate(endYear, endMonth, endDay);

    calculateTotal(receipts, itemName, startDate, endDate);

    freeReceipts(receipts);

    return 0;
}


int compareDates(DateTime d1, DateTime d2) {
    if (d1.year > d2.year) return DATE_LATER;
    if (d1.year < d2.year) return DATE_EARLIER;
    if (d1.month > d2.month) return DATE_LATER;
    if (d1.month < d2.month) return DATE_EARLIER;
    if (d1.day > d2.day) return DATE_LATER;
    if (d1.day < d2.day) return DATE_EARLIER;
    return DATE_EQ;
}

int printDateTime(DateTime d) {
    return printf("%04d-%02d-%02d\n", d.year, d.month, d.day);
}

receiptPosition createReceipt(DateTime date) {
    receiptPosition newReceipt = (receiptPosition)malloc(sizeof(Receipt));
    newReceipt->date = date;
    newReceipt->items = NULL;
    newReceipt->next = NULL;
    return newReceipt;
}

itemPosition createItem(const char* name, int quantity, float price) {
    itemPosition newItem = (itemPosition)malloc(sizeof(Item));
    strcpy(newItem->name, name);
    newItem->quantity = quantity;
    newItem->price = price;
    newItem->next = NULL;
    return newItem;
}

receiptPosition addReceipt(receiptPosition head, receiptPosition newReceipt) {
    if (head == NULL || compareDates(newReceipt->date, head->date) == DATE_EARLIER) {
        newReceipt->next = head;
        return newReceipt;
    }
    receiptPosition current = head;
    while (current->next != NULL && compareDates(newReceipt->date, current->next->date) != DATE_EARLIER) {
        current = current->next;
    }
    newReceipt->next = current->next;
    current->next = newReceipt;
    return head;
}

itemPosition addItem(itemPosition head, itemPosition newItem) {
    if (head == NULL || strcmp(newItem->name, head->name) < 0) {
        newItem->next = head;
        return newItem;
    }
    itemPosition current = head;
    while (current->next != NULL && strcmp(newItem->name, current->next->name) >= 0) {
        current = current->next;
    }
    newItem->next = current->next;
    current->next = newItem;
    return head;
}

receiptPosition loadReceipt(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", fileName);
        return NULL;
    }

    int year, month, day;
    fscanf(file, "%d-%d-%d", &year, &month, &day);
    DateTime date = setDate(year, month, day);
    receiptPosition newReceipt = createReceipt(date);

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[50];
        int quantity;
        float price;
        sscanf(line, "%49[^,], %d, %f", name, &quantity, &price);
        itemPosition newItem = createItem(name, quantity, price);
        newReceipt->items = addItem(newReceipt->items, newItem);
    }

    fclose(file);
    return newReceipt;
}

receiptPosition loadAllReceipts(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", fileName);
        return NULL;
    }

    receiptPosition receipts = NULL;
    char line[100];
    DateTime previousDate = setDate(0, 0, 0);
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        receiptPosition newReceipt = loadReceipt(line);
        if (compareDates(previousDate, setDate(0, 0, 0)) != DATE_EQ && compareDates(newReceipt->date, previousDate) == DATE_EARLIER) {
            printf("Error: Receipts are not in chronological order.\n");
            exit(1);
        }
        previousDate = newReceipt->date;
        receipts = addReceipt(receipts, newReceipt);
    }

    fclose(file);
    return receipts;
}

void calculateTotal(receiptPosition receipts, const char* itemName, DateTime startDate, DateTime endDate) {
    float totalSpent = 0;
    int totalQuantity = 0;

    while (receipts != NULL) {
        if (compareDates(receipts->date, startDate) != DATE_EARLIER && compareDates(receipts->date, endDate) != DATE_LATER) {
            itemPosition item = receipts->items;
            while (item != NULL) {
                if (strcmp(item->name, itemName) == 0) {
                    totalSpent += item->quantity * item->price;
                    totalQuantity += item->quantity;
                }
                item = item->next;
            }
        }
        receipts = receipts->next;
    }

    printf("Total spent on %s: %.2f\n", itemName, totalSpent);
    printf("Total quantity purchased: %d\n", totalQuantity);
}

void printItemNames(itemPosition items, char printedItems[][50], int* printedCount) {
    while (items != NULL) {
        int alreadyPrinted = 0;
        for (int i = 0; i < *printedCount; i++) {
            if (strcmp(printedItems[i], items->name) == 0) {
                alreadyPrinted = 1;
                break;
            }
        }
        if (!alreadyPrinted) {
            strcpy(printedItems[*printedCount], items->name);
            (*printedCount)++;
            printf("%s\n", items->name);
        }
        items = items->next;
    }
}

void freeItems(itemPosition items) {
    while (items != NULL) {
        itemPosition temp = items;
        items = items->next;
        free(temp);
    }
}

void freeReceipts(receiptPosition receipts) {
    while (receipts != NULL) {
        receiptPosition temp = receipts;
        receipts = receipts->next;
        freeItems(temp->items);
        free(temp);
    }
}