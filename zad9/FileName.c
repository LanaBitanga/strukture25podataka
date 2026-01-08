#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2
#define MIN 10
#define MAX 90

struct tree;
typedef struct tree* treepointer;

typedef struct _tree {
	int data;
	treepointer left;
	treepointer right;

}tree;

tree* insert(tree* root, int value);
tree* create(int data);
void inorder(tree* root, FILE* fp);
int replace(tree* root);
void freetree(tree* root);
tree* copytree(tree* root);
void generateRandomNumbers(int* numbers, int size, int min, int max);

int main() {
	tree* root = NULL;
	tree* rootcopy = NULL;

	int numbers[10];

	srand((unsigned)time(NULL));
	generateRandomNumbers(numbers, 10, MIN, MAX);

	for (int i = 0; i < 10; i++) {
		root = insert(root, numbers[i]);
	}

	rootcopy = copytree(root);

	int choice = 0;

	FILE* fp = NULL;

	do {

		printf("\nOdaberi opciju:\n");
		printf("\n 1. Inorder ispis stabla.\n");
		printf("\n 2. Replace stablo.\n");
		printf("\n 3. Upis u datoteku.\n");
		printf("\n 4. Izlaz.\n");

		scanf("%d", &choice);

		switch (choice) {

		case 1:
			printf("\nInorder ispis stabla:\n");
			inorder(rootcopy, NULL);
			break;

		case 2:
			printf("\nReplace stablo: \n");
			replace(root);
			printf("Replace zavrsen. Novo stablo: \n");
			inorder(root, NULL);
			break;

		case 3:
			fp = fopen("stablo.txt", "w");

			if (fp == NULL) {
				printf("Greska pri otvaranju datoteke\n");
				return FILE_OPEN_ERROR;
			}

			printf("Upisivanje stabla u datoteku.\n");
			fprintf(fp, "Originalno stablo (inorder):\n");
			inorder(rootcopy, fp);
			fprintf(fp, "\nReplace stablo (inorder):\n");
			inorder(root, fp);
			fclose(fp);
			break;

		case 4:
			printf("\nIzlaz iz programa.\n");
			freetree(root);
			freetree(rootcopy);

			break;

		default:
			printf("Nepoznata opcija, pokusaj opet :).\n");

		}

	} while (choice != 4);

	return 0;
}

tree* create(int data) {

	tree* newnode = (tree*)malloc(sizeof(tree));

	if (!newnode) {
		printf("Malloc greska.\n");
		return MALLOC_ERROR;
	}

	newnode->data = data;
	newnode->right = NULL;
	newnode->left = NULL;

	return newnode;
}

tree* insert(tree* root, int value) {

	if (root == NULL)
		return create(value);

	if (root->data > value) {
		root->right = insert(root->right, value);
	}

	else if (root->data < value) {
		root->left = insert(root->left, value);
	}

	return root;
}

void inorder(tree* root, FILE* fp) {

	if (root != NULL) {

		inorder(root->left, fp);
		if (fp) {
			fprintf(fp, "%d ", root->data);
		}
		else {
			printf("%d ", root->data);
		}
		inorder(root->right, fp);

	}
}

int replace(tree* root) {

	if (root == NULL) {
		return 0;
	}
	int leftValue = replace(root->left);
	int rightValue = replace(root->right);

	int originalValue = root->data;
	root->data = leftValue + rightValue;

	return originalValue + root->data;

}

tree* copytree(tree* root) {

	if (root == NULL)
	{
		return NULL;
	}
	tree* newnode = create(root->data);

	newnode->left = copytree(root->left);
	newnode->right = copytree(root->right);

	return newnode;
}

void freetree(tree* root) {

	if (root == NULL) {
		return;
	}

	freetree(root->left);
	freetree(root->right);

	free(root);
}

void generateRandomNumbers(int* numbers, int size, int min, int max) {

	for (int i = 0; i < size; i++) {

		numbers[i] = rand() % (max - min + 1) + min;
	}

}