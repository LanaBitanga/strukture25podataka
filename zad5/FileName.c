/*Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste.*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
struct stog;
typedef struct stog* Position;
typedef struct stog
{
	int el;
	Position next;
}Stog;
void Read(char*, char*);
void Push(Position, int);
int Pop(Position);
void Postfix(Position, char*, int);

int main()
{
	Stog head;
	head.next = NULL;
	int number;
	char name[256];
	char* buffer = NULL;
	buffer = (char*)malloc(1000 * sizeof(char));
	memset(buffer, '\0', 1000); // radi i bez ovoga
	printf("\nEnter the name of the document from which you want to read the data(e.g. postfix.txt):\t");
	scanf("%s", name);
	Read(&name, buffer);
	number = strlen(buffer);
	Postfix(&head, buffer, number);
	system("pause");
	return 0;
}
void Read(char* FileName, char* buffer)
{
	FILE* dat;
	int i = 0, n;
	dat = fopen(FileName, "r");
	if (!dat)
	{
		printf("File failed to open!!");
		return 1;
	}
	fgets(buffer, 1000, dat);
	fclose(dat);
}
void Push(Position p, int a)
{
	Position q = NULL;
	while (p->next != NULL)
		p = p->next;
	q = (Position)malloc(sizeof(Stog));
	q->el = a;
	q->next = p->next;
	p->next = q;
}
int Pop(Position p)
{
	Position temp;
	int element;
	temp = p;

	while (temp->next != NULL)
	{
		p = temp;
		temp = temp->next;
	}
	element = temp->el;
	p->next = temp->next;
	free(temp);
	return element;
}
void Postfix(Position head, char* buffer, int n)
{
	int number, result = 0, count = 0;
	int c_value, i_value;
	char operation;
	while (result <= n)
	{
		c_value = 0;
		i_value = 0;
		i_value = sscanf(buffer, "%d%n", &number, &count);
		if (i_value == 1)
		{
			printf(" %d", number);
			Push(head, number);
		}
		else
		{
			c_value = sscanf(buffer, " %c%n", &operation, &count);
			if (c_value == 1)
			{
				printf(" %c", operation);
				int el1 = Pop(head);
				int el2 = Pop(head);
				switch (operation)
				{
				case'+':Push(head, el2 + el1); break;
				case'-':Push(head, el2 - el1); break;
				case'*':Push(head, el2 * el1); break;
				case'/':Push(head, el2 / el1); break;
				}
			}
		}
		buffer += count;
		result += count;
	}
	printf(" = %d\n\n", Pop(head));
}