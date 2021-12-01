#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct buyInfo
{
	char* chem_name;
	char* chem_code;
	//char chem_name[41];
	//char chem_code[21];
	long quantity;
	long cost;
} buyInfo;

typedef struct invInfo
{
	//char* chem_name;
	//char* chem_code;
	//char* chem_name = (char*)malloc(41);
	//char* chem_code = (char*)malloc(21);
	char chem_name[41];
	char chem_code[21];
	long quantity;
} invInfo;

typedef struct chemNode
{
	invInfo data;
	struct chemNode* left;
	struct chemNode* right;
} chemNode;

typedef struct chemTree
{
	double balance;
	chemNode* root;
} chemTree;

void initInventory(char* invFileName, double bal, chemTree* invTree);
FILE* initErr(char* errFileName, FILE* currErr);
void makeSale(char* saleFileName, chemTree* currInv, FILE* currErr);
void printMenu(chemTree* invTree);
void insertChemNode(chemTree* invTree);
void insertChemNode1(chemNode* invNode, chemNode* Node);
void printTree(chemNode* invNode);
void deleteTree1(chemNode* invNode);
void deleteTree(chemTree* invTree);
chemNode* makeNode(char* chem_code, char* chem_name, int quantity);

void fprintTree(chemNode* invNode);
int saveInventory(char* invFileName, char* errFileName, chemTree* currInv, FILE* currErr);

void makePurchase(struct buyInfo* buy, chemTree* currInv);
chemNode* findNode(char* chem_code, chemNode* Node);

int main()
{
	int choice;
	double balance;
	
	chemTree* invTree = (chemTree*)malloc(sizeof(chemTree));

	FILE* currErr = NULL;
	invTree->root = NULL;
	invTree->balance = 0;

	while (1)
	{
		char* invFileName = (char*)malloc(200 * sizeof(char));
		char* errFileName = (char*)malloc(200 * sizeof(char));
		char* errFileName_2 = (char*)malloc(200);

		char chemName[41];
		char chemCode[21];
		//char* chemName = (char*)malloc(41);
		//char* chemCode = (char*)malloc(21);
		long quantity = 0;
		long cost = 0;

		printMenu(invTree);
		scanf("%d", &choice);
		while ((getchar()) != '\n');
		switch (choice)
		{
		case 0:
			exit(1);
		case 1:
		{//char* invFileName = (char*)malloc(200 * sizeof(char));
			//char* errFileName = (char*)malloc(200 * sizeof(char));
			printf("Please enter inventory file name: ");
			scanf("%200[^\n]s", invFileName);
			while ((getchar()) != '\n');
			printf("Please enter balance value: ");

			while (!(scanf("%lf", &balance)))
			{
				printf("Error in floating point number input. Try again: ");
			}
			while ((getchar()) != '\n');
			initInventory(invFileName, balance, invTree);
			printf("Please enter error file name: ");
			//while ((getchar()) != '\n');
			scanf("%200[^\n]s", errFileName);
			while ((getchar()) != '\n');
			currErr = initErr(errFileName, currErr);
			//free(invFileName);
			//free(errFileName);
			break; }
		case 2:
		{


			printf("\nPlease enter name of file for saving current inventory status:");
			scanf("%200[^\n]s", invFileName);
			while ((getchar()) != '\n');
			printf("\nPlease enter name of file for saving error status:");
			scanf("%200[^\n]s", errFileName_2);


			saveInventory(invFileName, errFileName_2, invTree, currErr);


			break;
		}
		case 3:
			break;
		case 4:
		{	
		buyInfo* purch = (buyInfo*)malloc(sizeof(buyInfo));
		
		printf("Please insert chemical name:");
		scanf("%41[^\n]s", chemName);
		while ((getchar()) != '\n');
		purch->chem_name = chemName;

		printf("Please insert chemical code:");
		scanf("%21[^\n]s", chemCode);
		while ((getchar()) != '\n');
		purch->chem_code = chemCode;
		
		printf("Please insert chemical quantity:");
		scanf("%ld", &quantity);
		purch->quantity = quantity;
		
		printf("Please insert chemical cost:");
		scanf("%ld", &cost);
		purch->quantity = cost;
		
		makePurchase(purch, invTree);
		free(purch);
		break;
		}
		default:
			printf("command not recognized. Try again.\n");

		}
		
		//free(chemName);
		//free(chemCode);
		free(invFileName);
		free(errFileName);
		free(errFileName_2);
		}
		return 0;
	}


void printMenu(chemTree* invTree){
	printf("Welcome to Chem-R-Us LTD database. Current company balance is %lf.\n"
		"Please select an option from the following menu.\n"
		"Please enter choice.\n"
		"0) Quit.\n"
		"1) Initialize company inventory and error report files.\n"
		"2) Save current Inventory and error status to files.\n"
		"3) Record a sale.\n"
		"4) Record a purchase. \n\n", invTree->balance);
}

void initInventory(char* invFileName, double bal, chemTree* invTree)
{
	char* chem_name = (char*)malloc(41);
	char* chem_code = (char*)malloc(21);
	long quantity;
	chemNode* Node;

	invTree->balance = bal;
	deleteTree(invTree);
	FILE* invFile = fopen(invFileName, "r");
	if (invFile == NULL)
		printf("Error in opening inventory file. Inventory tree initialized as empty.\n");
	else
	{
		while (fscanf(invFile, "%40[^ ] %20[^ ] %ld", chem_name, chem_code, &quantity) == 3)
		{
			while ((fgetc(invFile)) != '\n');
			Node = makeNode(chem_code, chem_name, quantity);
			insertChemNode(invTree, Node);
			
		}
		//printTree(invTree->root);
		fclose(invFile);
	}

}

FILE* initErr(char* errFileName, FILE* currErr)
{

	if (currErr != NULL)
	{
		fclose(currErr);
	}
	currErr = fopen(errFileName, "a");
	return currErr;
}

void makeSale(char* saleFileName, chemTree* currInv, FILE* currErr)
{

}

void insertChemNode(chemTree* invTree, chemNode* Node)
{
	if (invTree->root == NULL)
		invTree->root = Node;
	else
	{
		insertChemNode1(invTree->root, Node);
	}
}

void insertChemNode1(chemNode* invNode, chemNode* Node)
{
	if (strcmp(Node->data.chem_code, invNode->data.chem_code) == 0)
		invNode->data.quantity = invNode->data.quantity + Node->data.quantity;
	if (strcmp(Node->data.chem_code, invNode->data.chem_code) > 0)
	{
		if (invNode->right == NULL)
			invNode->right = Node;
		else
			insertChemNode1(invNode->right, Node);
	}
	if (strcmp(Node->data.chem_code, invNode->data.chem_code) < 0)
	{
		if (invNode->left == NULL)
			invNode->left = Node;
		else
			insertChemNode1(invNode->left, Node);
	}
}

void printTree(chemNode* invNode)
{
	if (invNode == NULL)
		printf("The tree is empty");
	else
	{
		if (invNode->left != NULL)
			printTree(invNode->left);
		printf("%s %d\n", invNode->data.chem_code, invNode->data.quantity);
		if (invNode->right != NULL)
			printTree(invNode->right);
	}
}

void deleteTree(chemTree* invTree)
{
	deleteTree1(invTree->root);
	invTree->root = NULL;
}

void deleteTree1(chemNode* invNode)
{
	if (invNode != NULL)
	{
		if (invNode->left != NULL)
			deleteTree1(invNode->left);
		if (invNode->right != NULL)
			deleteTree1(invNode->right);
		free(invNode);
		//invNode = NULL;
	}
}

chemNode* makeNode(char* chem_code, char* chem_name, long quantity)
{
	chemNode* Node = (chemNode*)malloc(sizeof(chemNode));
	Node->data.quantity = quantity;
	strcpy_s(Node->data.chem_code, 20, chem_code);
	strcpy_s(Node->data.chem_name, 40, chem_name);
	Node->left = Node->right = NULL;
	return Node;
}

void fprintTree(FILE* invFile, chemNode* invNode)
{
	if (invNode == NULL) {}

	else
	{
		if (invNode->left != NULL)
			fprintTree(invFile, invNode->left);
		fprintf(invFile, "%s %s %d\n", invNode->data.chem_name, invNode->data.chem_code, invNode->data.quantity);
		if (invNode->right != NULL)
			fprintTree(invFile, invNode->right);
	}
}
int saveInventory(char* invFileName, char* errFileName, chemTree* currInv, FILE* currErr) {
	FILE* invFile = fopen(invFileName, "w");
	FILE* errFile = fopen(errFileName, "w");
	fseek(currErr, 0, SEEK_SET);

	chemNode* rootNode = currInv->root;

	fprintTree(invFile, rootNode); // print invTree into file

	fclose(invFile);

	char c = fgetc(currErr);	   //copy currErr content into errFile
	while (c != EOF) {
		fputc(c, errFile);
		c = fgetc(currErr);
	}
	fclose(errFile);

}

void makePurchase(struct buyInfo* buy, chemTree* currInv)
{

	chemNode* buytoNode = findNode(buy->chem_code,currInv->root);         // Find the node to buy into.
	currInv->balance -= buy->cost;										  // Decrease invTree balance
	if (buytoNode != NULL)
		buytoNode->data.quantity += buy->quantity;						  // If the node exists, increase it's quantity.
	else {																  // else, crease a new node and insert it into the tree
		chemNode* node = makeNode(buy->chem_code, buy->chem_name, buy->quantity);	
		insertChemNode(currInv, node);
	}

	
}

chemNode* findNode(char* chem_code, chemNode* Node)
{
	if (Node == NULL)
		return NULL;
	if (strcmp(Node->data.chem_code, chem_code) == 0)
		return Node;
	if (strcmp(Node->data.chem_code, chem_code) > 0)
	{
		if (Node->left != NULL)
			return findNode(chem_code, Node->left);
		return NULL;
	}
	if (strcmp(Node->data.chem_code, chem_code) < 0)
	{
		if (Node->right != NULL)
			return findNode(chem_code, Node->right);
		return NULL;
	}
}


