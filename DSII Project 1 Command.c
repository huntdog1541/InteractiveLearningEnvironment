/*
 * Commands.c
 *
 *  Created on: Sep 2, 2013
 *      Author: David Hunt
 */
#include "Commands.h"
#include "CmmdNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cmdTree * initialTree(cmdTree * t)
{
	//cmdTree t;
	strcpy(t->ft, "hunt/");
	t->root = createNode();
	strcpy(t->root->fName, "root");
	t->currentDirectory = NULL;
	t->lastNode = NULL;
	t->root->fileType = 'D';
	return t;
}

/*
 * ls() - lists the files in a directory
 */
void ls(cmdTree * t)
{
	Node * nt = NULL;

	if(t->currentDirectory != NULL)
	{
		if(strcmp(t->currentDirectory->fName, "root") == 0)
		{
			printf("Tree is empty\n");
		}
		if(t->currentDirectory->rightNode != NULL)
		{
			nt = t->currentDirectory->rightNode;
			while(nt != NULL)
			{
				printf(" %s ", nt->fName);
				nt = nt->rightNode;
			}

			printf("\n");
		}
		else
		{
			printf("No Files\n");
		}
	}
}

/*
 * mkdir(char *) - creates a directory
 */
void mkdir(char * dirt, cmdTree * t)
{
	//Node * temp = NULL;

	if(t->currentDirectory == NULL)
	{
			t->root->bottomNode = createNode();

			strcpy(t->root->bottomNode->fName, dirt);
			t->root->bottomNode->fileType = 'D';
			t->root->bottomNode->parentNode = t->root;
			t->currentDirectory = t->root->bottomNode;
			t->lastNode = t->currentDirectory;
	}
	else
	{

		t->lastNode->rightNode = createNode();
		strcpy(t->lastNode->rightNode->fName, dirt);
		t->lastNode->rightNode->fileType = 'D';
		t->lastNode->rightNode->parentNode = t->currentDirectory;
		t->lastNode = t->lastNode->rightNode;
		t->currentDirectory = t->lastNode;
	}


}

/*
 * cd(char *) - change directory
 */
void cd(char * dirt, cmdTree * t)
{
	Node * temp = NULL;
	Node * temp2 = NULL;
	temp = t->currentNode;
	while((temp->rightNode != NULL) && (temp2 == NULL))
	{
		if((strcmp(dirt, temp->fName)) == 0)
		{
			temp2 = temp;
		}
		else
		{
			while((temp->fileType == 'F') && (temp->rightNode != NULL))
			{
				temp = temp->rightNode;
			}
		}
	}
	t->currentDirectory = temp2;
}

/*
 * cd2(char *) - change directory to the parent directory
 */
void cd2(cmdTree * t)
{
	if(t->currentNode->parentNode != NULL)
	{
		t->currentDirectory = t->currentNode->parentNode;
	}
	else
	{
		fprintf(stderr, "There is no parent\n");
	}
}

/*
 * pwd() - change to root directory
 */
void pwd(cmdTree * tree)
{
	Node * temp = tree->currentDirectory;
	printf("%s", tree->ft);
	temp = printLocation(temp);
	printf("%s", tree->currentDirectory->fName);
	printf("\n\n");
}

/*
 * addf(char *) - adding a file to the directory
 */
void addf(char * fName1, cmdTree * t)
{
	if(t->lastNode != NULL)
	{
		t->lastNode->rightNode = createNode();
		strcpy(t->lastNode->rightNode->fName, fName1);
		t->lastNode->rightNode->fileType = 'F';
		t->lastNode->rightNode->parentNode = t->currentDirectory;
		t->lastNode->rightNode->previousNode = t->lastNode;
		t->lastNode = t->lastNode->rightNode;
	}
	else
	{
		t->lastNode = createNode();
		strcpy(t->lastNode->fName, fName1);
		t->lastNode->fileType = 'F';
		t->lastNode->previousNode = t->root;
		t->lastNode->parentNode = t->currentDirectory;
	}
}

/*
 * mv(char *, char *) - renames the contents of a file or folder
 */
void mv(char * fName1, char * fName2, cmdTree * t)
{
	whereis(fName1, t);
	if(t->currentNode != NULL)
	{
		strcpy(t->currentNode->fName, fName2);
	}

}

/*
 * cp(char *, char *) -
 */
void cp(char * fName1, char * fName2, cmdTree * t)
{

}

/*
 * rm(char *) - remove file or directory
 */
void rm(char * fName1, cmdTree * t)
{
	Node * temp;
	whereis(fName1, t);
	temp = t->currentNode;
	if(t->currentNode->rightNode != NULL)
	{
		temp->previousNode->rightNode = temp->rightNode;
		t->currentNode = temp->previousNode;
		temp = freeNode(temp);
	}
	else
	{
		temp->previousNode->rightNode = NULL;
		t->currentNode = temp->previousNode;
		temp = freeNode(temp);
	}

}

/*
 * whereis() - locates a specific file or folder and sets the Node to currentNode
 */
void whereis(char * fName1, cmdTree * t)
{
	Node* temp = NULL;
	temp = finditName(fName1, t->root);
	t->currentNode = temp;
}

void commandLine(cmdTree * t)
{
	int controlLoop = 0;
	char sentence[100];
	FILE * fin;

	fin = fopen("Input2.txt", "r");
	if(fin == NULL)
	{
		perror("Error opening file");
		//return (-1);
	}

	while(controlLoop == 0)
	{
		printprompt();
		if(fgets(sentence, 100, fin) != NULL)
		{
			controlLoop = parseString(sentence, t);
		}

	}
	//return 0;
}

void printprompt()
{
	printf("$ ");
}

int parseString(char * st, cmdTree * t)
{
	char * wd;
	char   Words[100][35];
	int i = 0;
	//int j = 0;
    int cont = 0;
    //printf("\nSplitting the sentence\n");

	wd = strtok(st, " ,-\n");
	while((wd != NULL) && (cont == 0))
	{
	printf("%s ", wd);
	strcpy(Words[i], wd);
	//Words[i] = wd;
	i++;
	wd = strtok(NULL, " ,-\n");
	}

	//printf("\n%i\n", i);
	cont = firstChar(Words, t, i);
	//cont = printWords(Words, 2);

	return cont;
}

int firstChar(char (*words)[35], cmdTree * t, int n)
{
	 int cont = 0;
	 int wrdCount = 0;
	 //int chaCount = 0;

	 while(wrdCount <= n)
	 {
		 if(strcmp(words[wrdCount], "ls") == 0) // ls
		 {
			 printf(" ls\n");
			 ls(t);
			 printTreeInfo(t);
		 }
		 else if(words[wrdCount][0] == 'm') //mkdir <dirname>
		 {
			 if(strcmp(words[wrdCount], "mv") == 0)
			 {
				 printf(" mv\n");
			 }
			 if(strcmp(words[wrdCount],"mkdir") == 0)
			 {
				 printf(" mkdir\n");
				 if(words[wrdCount +1] != NULL)
					 mkdir(words[wrdCount+1], t);
				 printTreeInfo(t);
				 //cont = 1;
			 }
		 }
		 else if(strcmp(words[wrdCount], "addf") == 0) //addf <filename>
		 {
			 printf(" addf\n");
			 //addf("File1", t);
			 //printTreeInfo(t);
		 }
		 else if(words[wrdCount][0] == 'c') //cd <dirname>, cd .., cp <filename1> <filename>
		 {
			 if(strcmp(words[wrdCount], "cd") == 0)
			 {
				 if(strcmp(words[wrdCount+1], "..") == 0)
				 {
					 printf(" cd .. \n");
				 }
				 else
				 {
					 printf(" cd\n");
				 }
			 }
			 if(strcmp(words[wrdCount],"cp") == 0)
			 {
				 printf(" cp\n");
			 }
		 }
		 else if(strcmp(words[wrdCount], "pwd") == 0) //pwd
		 {
			 printf(" pwd\n");
			 pwd(t);
		 }
		 else if(strcmp(words[wrdCount], "rm") == 0) //rm <filename>
		 {
			 printf(" rm <filename> \n");
		 }
		 else if(strcmp(words[wrdCount], "bye") == 0) //bye
		 {
			 printf(" bye \n");
			 cont = 1;
		 }
		 else if(strcmp(words[wrdCount], "whereis") == 0) //whereis
		 {
			 printf(" whereis \n");
		 }
		 wrdCount++;
	 }

	    return cont;

}

int printWords(char (*wrds)[35], int endloop)
{
	int i = 0;
	while(i != endloop)
	{
		printf("\n%s %i", wrds[i], i);
		i++;
	}
	return 1;
}

void printTreeInfo(cmdTree * t)
{
	printf("\nTree Info:\n");
	if(t->ft != NULL)
			printf("Tree Name: %s\n", t->ft);
	if(t->root != NULL)
			printf("Root Directory: %s\n", t->root->fName);
	if(t->currentDirectory != NULL)
	{
		printf("Current Directory: %s\n", t->currentDirectory->fName);
		if(t->currentDirectory->parentNode != NULL)
				printf("Current Director Parent Node: %s\n", t->currentDirectory->parentNode->fName);
		else
			printf("Current Directory Parent Node: NULL\n");
	}
	else
	{
		printf("Current Directory: NULL\n");
		printf("Current Directory Parent Node: NULL\n");
	}
	if(t->lastNode != NULL)
		printf("Last Node: %s\n", t->lastNode->fName);
	else
	{
		printf("Last Node: NULL\n\n\n");
	}

}

Node * finditName(char * str,Node * p)
{
	Node * temp = p;

	if(p == NULL)
	{
		return NULL;
	}

	if(strcmp(temp->fName, str) != 0)
	{
		if(p->bottomNode != NULL)
		{
			temp = finditName(str, p->bottomNode);
		}
		else if(p->rightNode != NULL)
		{
			temp = finditName(str, p->rightNode);
		}
	}
	else
	{
		return temp;
	}

	return temp;
}

Node * printLocation(Node * n)
{
	Node * temp1 = NULL;
	if((n != NULL) && (strcmp(n->fName, "root") != 0))
	{
		temp1 = printLocation(n->parentNode);
		printf("%s/", temp1->fName);
	}
	return n;
}
