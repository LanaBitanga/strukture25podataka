
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

typedef struct _direct* position;
typedef struct _stack* Sposition;

typedef struct _direct {
	char name[MAX];
	position sibling;
	position child;
}Direct;

typedef struct _stack {
	position data;
	Sposition next;
}Stack;

position AllocationDirect();
Sposition AllocationStack();
int Push(position, Sposition);
position Pop(Sposition);
position MakeDirect(position, char*);
position ChangeDirect(position, Sposition, char*);
position PrevChange(Sposition);
int Print(position);
int Path(position, Sposition);
int Command(position, Sposition);
void Del(position);

int main() {

	Direct C;
	C.sibling = NULL;
	C.child = NULL;
	strcpy(C.name, "C:");

	Stack SHead;
	SHead.data = NULL;
	SHead.next = NULL;

	Push(&C, &SHead);
	Command(&C, &SHead);

	return 0;
}

position AllocationDirect() {
	position newD = (position)malloc(sizeof(Direct));
	if (!newD)
		printf("Greska 1");

	newD->child = NULL;
	newD->sibling = NULL;
	return newD;
}

Sposition AllocationStack() {

	Sposition newS = NULL;
	newS = (Sposition)malloc(sizeof(Sposition));

	if (newS == NULL) {
		return 1;
	}

	return newS;

}

int Push(position current, Sposition SHead) {

	Sposition newS;
	Sposition tempS = SHead;

	if (newS = AllocationStack()) {

		while (tempS->next)
			tempS = tempS->next;

		newS->next = tempS->next;
		tempS->next = newS;

		newS->data = current;

		return 0;
	}

	else
		return 1;


}

/*position Pop(Sposition SHead) {
	Sposition tempS = SHead->next;
	Sposition prevS = SHead->next;

	if (!tempS->next)
		return NULL;

	while (tempS->next) {
		prevS = tempS;
		tempS = tempS->next;
	}
	position dir = tempS->data;
	prevS->next = tempS->next;
	free(tempS);
	return dir;
}
*/
position Pop(Sposition SHead) {

	Sposition tempS = SHead->next;
	Sposition prevS = SHead;

	if (!tempS) {
		return NULL;
	}

	while (tempS->next) {
		prevS = tempS;
		tempS = tempS->next;
	}

	position dir = tempS->data;
	prevS->next = tempS->next;
	free(tempS);
	return dir;

}


position MakeDirect(position current, char* name) {

	position newD = AllocationDirect();

	strcpy(newD->name, name);

	if (!current->child)
		current->child = newD;

	else {

		position currentChild = current->child;

		while (currentChild->sibling)
			currentChild = currentChild->sibling;
		currentChild->sibling = newD;

	}

	return current;
}

position ChangeDirect(position current, Sposition SHead, char* name) {

	if (!current->child)
		return current;

	else {
		position parent = current;
		position find = current->child;

		while (find) {

			if (!strcmp(find->name, name)) {
				Push(find, SHead);
				return find;

			}
			find = find->sibling;
		}

		return parent;

	}
}

position PrevChange(Sposition SHead) {
	return Pop(SHead);
}

int Print(position current) {

	if (!current->child || !current) {
		printf("Nema direktorija.\n");
		return 0;
	}

	position p = current->child;

	while (p) {

		printf(" %s\n", p->name);
		p = p->sibling;
	}

	return 0;
}

int Path(position current, Sposition SHead) {

	char string[MAX] = "";

	while (SHead) {

		strcat(string, SHead->data->name);
		strcat(string, "\\");
		SHead = SHead->next;
	}

	strcat(string, ">");
	printf("%s", string);

	return 0;
}

void Del(position current) {
	if (!current)
		return;

	Del(current->sibling);
	Del(current->child);
	free(current);
}

int Command(position current, Sposition SHead) {
	position C = current;
	char input[MAX];
	char command[5];
	char dname[MAX];
	Path(current, SHead->next);

	do {
		fgets(input, MAX, stdin);
		sscanf(input, "%s %s", command, dname);
		if (!strcmp(command, "md")) {
			current = MakeDirect(current, dname);
			Path(current, SHead->next);
		}
		else if (!strcmp(command, "cd")) {
			current = ChangeDirect(current, SHead, dname);
			Path(current, SHead->next);
		}
		/*else if (!strcmp(command, "cd..")) {
			if ((current = PrevChange(SHead)) == NULL)
				current = C;
			Path(current, SHead->next);
		}*/

		else if (!strcmp(command, "cd..")) {
			current = PrevChange(SHead);
			if (!current)
				current = C; // Ako nema roditelja, vrati se na korijen (C:)
			Path(current, SHead->next);
		}

		else if (!strcmp(command, "dir")) {
			Print(current);
			Path(current, SHead->next);
		}
		else if (!strcmp(command, "exit"))
			Del(current);
		else
			printf("Krivo");
	} while (strcmp(command, "exit"));

	return 0;
}
