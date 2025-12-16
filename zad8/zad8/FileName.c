
#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>

struct _tree;
typedef struct _tree* tp;

typedef struct _tree {

	int data;
	tp right;
	tp left;
}tree;


tree* Insert(tree* root, int value);
tree* Create(int data);
void Preorder(tree* root);
void Inorder(tree* root);
void Postorder(tree* root);
void Level(tree* root);
int Height(tree* root);
void Printcurrentlevel(tree* root, int level);
int Search(tree* root, int value);
tree* Delete(tree* root, int value);
tree* Min(tree* root);
void FreeTree(tree* root);



int main() {

	tree* root = NULL;

	int numbers[] = { 5,3,2,4,7,6,8 };

	int n = sizeof(numbers) / sizeof(numbers[0]);

	for (int i = 0; i < n; i++) {
		root = Insert(root, numbers[i]);
	}
	int choice, value;

	do {

		printf("Odaberi opciju: \n");

		printf("\n1. Dodaj element\n");
		printf("2. Ispis (preorder)\n");
		printf("3. Ispis (inorder)\n");
		printf("4. Ispis (postorder)\n");
		printf("5. Ispis (level order)\n");
		printf("6. Pronadi element\n");
		printf("7. Izbrisi element\n");
		printf("8. Izlaz\n");

		scanf("%d", &choice);

		switch (choice) {


		case 1:
			printf("Unesi vrijednost: ");
			scanf("%d", &value);
			root = Insert(root, value);
			break;

		case 2:
			Preorder(root);
			printf("\n");
			break;

		case 3:
			Inorder(root);
			printf("\n");
			break;

		case 4:
			Postorder(root);
			printf("\n");
			break;

		case 5:
			Level(root);
			printf("\n");
			break;

		case 6:
			printf("Unesi vrijednost za pretragu: ");
			scanf("%d", &value);

			if (Search(root, value) == 1)
				printf("Element pronaden.\n");
			else {
				printf("Element ne postoji.\n");
			}
			break;

		case 7:
			printf("Unesi vrijednost za brisanje: ");
			scanf("%d", &value);
			root = Delete(root, value);
			break;

		case 8:
			printf("Izlaz. Oslobodena memorija.\n");
			FreeTree(root);
			root = NULL;
			break;

		default:
			printf("Krivi unos.\n ");
		}

	} while (choice != 8);

	return 0;
}


tree* Create(int data) {

	tree* new = (tree*)malloc(sizeof(tree));
	new->data = data;
	new->left = new->right = NULL;

	return new;

}


tree* Insert(tree* root, int value) {

	if (root == NULL)
		return Create(value);

	if (value < root->data)
		root->left = Insert(root->left, value);
	else if (value > root->data)
		root->right = Insert(root->right, value);

	return root;
}

void Preorder(tree* root) {

	if (root != NULL) {

		printf("%d", root->data);
		Preorder(root->left);
		Preorder(root->right);
	}
}
void Inorder(tree* root) {

	if (root != NULL) {

		Preorder(root->left);
		printf("%d", root->data);
		Preorder(root->right);
	}
}

void Postorder(tree* root) {

	if (root != NULL) {

		Preorder(root->left);
		Preorder(root->right);
		printf("%d", root->data);
	}
}

void Level(tree* root) {

	int h = Height(root);
	for (int i = 0; i <= h; i++)
		Printcurrentlevel(root, i);
}

int Height(tree* root) {

	if (root == NULL)
		return 0;

	int LH = Height(root->left);
	int RH = Height(root->right);
	int max;

	if (RH > LH)
		max = RH;
	else
		max = LH;

	return max + 1;
}

void Printcurrentlevel(tree* root, int level) {

	if (root == NULL)
		return;

	if (level == 1)
		printf("%d", root->data);

	else if (level > 1) {

		Printcurrentlevel(root->left, level - 1);
		Printcurrentlevel(root->right, level - 1);

	}
}

int Search(tree* root, int value) {

	if (root == NULL)
		return 0;
	if (root->data == value)
		return 1;
	if (value < root->data) return Search(root->left, value);
	else
		return Search(root->right, value);
}

tree* Min(tree* root) {

	while (root->left != NULL)
		root = root->left;
	return root;

}

tree* Delete(tree* root, int data) {

	if (root == NULL)
		return root;

	else if (data < root->data)
		root->left = Delete(root->left, data);

	else if (data > root->data)
		root->right = Delete(root->right, data);

	else {

		if (root->right == NULL && root->left == NULL) {
			free(root);
			root = NULL;
		}

		else if (root->left == NULL) {
			tree* temp = root;
			root = root->right;
			free(temp);
		}

		else if (root->right == NULL) {
			tree* temp = root;
			root = root->left;
			free(temp);
		}

		else {

			tree* temp = Min(root->right);
			root->data = temp->data;
			root->right = Delete(root->right, temp->data);
		}

	}

	return root;
}

void FreeTree(tree* root) {

	if (root == NULL)
		return;

	FreeTree(root->left);
	FreeTree(root->right);

	free(root);
}
