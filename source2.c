#Submitters: 208725218, 205887425
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct buyInfo
{
	char chem_name[41];
	char chem_code[21];
	long quantity;
	long cost;
} buyInfo;

typedef struct invInfo
{
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
void fprintTree(chemNode* invNode);
void makePurchase(struct buyInfo* buy, chemTree* currInv);
int saveInventory(char* invFileName, char* errFileName, chemTree* currInv, FILE* currErr);
chemNode* makeNode(char* chem_code, char* chem_name, int quantity);
chemNode* findNode(char* chem_code, chemNode* Node);
chemNode* minValueNode(chemNode* node);
chemNode* deleteNode(chemNode* root, char* chem_code);


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
		printMenu(invTree);
		scanf("%d", &choice);
		while ((getchar()) != '\n');
		switch (choice)
		{
		case 0:
			exit(1);
		case 1:
		{
			printf("Please enter inventory file name: ");
			scanf("%200[^\n]s", invFileName);
			while ((getchar()) != '\n');
			printf("Please enter balance value: ");
			int m;
			m = scanf("%lf", &balance);
			while ((getchar()) != '\n');
			while (!m)
			{
				printf("Error in floating point number input. Try again: ");
				m = scanf("%lf", &balance);
				while ((getchar()) != '\n');
			}

			initInventory(invFileName, balance, invTree);
			printf("Please enter error file name: ");

			if (scanf("%200[^\n]s", errFileName))
			{
				while ((getchar()) != '\n');
				currErr = initErr(errFileName, currErr);
			}
			else
			{
				while ((getchar()) != '\n');
				currErr = initErr("", currErr);
			}

			break; }
		case 2:
		{	if (invTree->root == NULL)
					printf("inventory not initialized yet. Initialize before choosing any other option.\n");
				else
				{
					if (currErr == NULL)
						printf("Error file not initialized yet. Initialize before choosing any other option.\n");
					else
					{
						printf("Please enter name of file for saving current inventory status: ");
						scanf("%200[^\n]s", invFileName);
						while ((getchar()) != '\n');
						printf("Please enter name of file for saving error status: ");
						scanf("%200[^\n]s", errFileName_2);
						saveInventory(invFileName, errFileName_2, invTree, currErr);
					}
				}
		break;
		}
			
		case 3:
		{	if (invTree->root == NULL)
				printf("inventory not initialized yet. Initialize before choosing any other option.\n");
			else
			{
				if (currErr == NULL)
					printf("Error file not initialized yet. Initialize before choosing any other option.\n");
				else
				{
					char* saleFileName = (char*)malloc(200 * sizeof(char));
					printf("Please enter name of sale file: ");
					scanf("%200s", saleFileName);
					while ((getchar()) != '\n');
					makeSale(saleFileName, invTree, currErr);
					free(saleFileName);
				}
			}
			break;
		}
		case 4:
			{	if (invTree->root == NULL)
				printf("inventory not initialized yet. Initialize before choosing any other option.\n");
			else
			{
				if (currErr == NULL)
					printf("Error file not initialized yet. Initialize before choosing any other option.\n");
				else
				{
					buyInfo* purch = (buyInfo*)malloc(sizeof(buyInfo));
					char* chemName = (char*)malloc(41 * sizeof(char));
					char* chemCode = (char*)malloc(21 * sizeof(char));
					long quantity = 0;
					long cost = 0;
					printf("Please insert chemical name: ");
					scanf("%41[^\n]s", chemName);
					while ((getchar()) != '\n');
					strcpy(purch->chem_name, chemName);

					printf("Please insert chemical code: ");
					scanf("%21[^\n]s", chemCode);
					while ((getchar()) != '\n');
					strcpy(purch->chem_code, chemCode);

					printf("Please insert chemical quantity: ");
					scanf("%ld", &quantity);
					purch->quantity = quantity;

					printf("Please insert chemical cost: ");
					scanf("%ld", &cost);
					purch->cost = cost;

					makePurchase(purch, invTree);
					free(purch);
				}
			}
			break;
		}
		default:
			printf("command not recognized. Try again.\n");
		}
		free(invFileName);
		free(errFileName);
	}
	return 0;
}

void printMenu(chemTree* invTree)
{
	printf("Welcome to Chem-R-Us LTD database. Current company balance is %lf.\n"
		"Please select an option from the following menu.\n"
		"Please enter choice.\n"
		"0) Quit.\n"
		"1) Initialize company inventory and error report files.\n"
		"2) Save current Inventory and error status to files.\n"
		"3) Record a sale.\n"
		"4) Record a purchase. \n\n", invTree->balance);
}



FILE* initErr(char* errFileName, FILE* currErr)
{

	if (currErr != NULL)
	{
		fclose(currErr);
	}
	currErr = fopen(errFileName, "w+");
	if (currErr == NULL)
		printf("Error in openning error file. Database is not initialized.\n\n");
	return currErr;
}

void makeSale(char* saleFileName, chemTree* currInv, FILE* currErr)
{
	char* chem_code = (char*)malloc(21 * sizeof(char));                     
	long quantity;
	long price;
	char* nameOfRep = (char*)malloc(21 * sizeof(char));
	chemNode* invNode = (chemNode*)malloc(sizeof(chemNode));
	printf("Please enter name of sales representative (no spaces): ");
	scanf("%20s", nameOfRep);
	while ((getchar()) != '\n');
	FILE* saleFile = fopen(saleFileName, "r");
	if (saleFile == NULL)
		printf("Faild to open the sale file for reading.\n");
	else
	{
		while (fscanf(saleFile, "%20[^ ] %ld %ld", chem_code, &quantity, &price) == 3)
		{
			while ((fgetc(saleFile)) != '\n');
			invNode = findNode(chem_code, currInv->root);
			if (invNode == NULL || invNode->data.quantity < quantity)
			{
				fprintf(currErr, "%s %s %d\n", chem_code, nameOfRep, quantity);
			}
			else
			{
				if (invNode->data.quantity > quantity)
					invNode->data.quantity = invNode->data.quantity - quantity;
				else
					deleteNode(currInv->root, chem_code);
				currInv->balance = currInv->balance + price;
			}
		}
		
		fclose(saleFile);
	}
	free(chem_code);
	free(nameOfRep);
	free(invNode);
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
 /* Recursive in-order print the tree*/
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
		
		fclose(invFile);
	}

}

void makePurchase(struct buyInfo* buy, chemTree* currInv)
{

	chemNode* buytoNode = findNode(buy->chem_code, currInv->root);         // Find the node to buy into.
	currInv->balance -= buy->cost;										   // Decrease invTree balance
	if (buytoNode != NULL)
		buytoNode->data.quantity += buy->quantity;						   // If the node exists, increase it's quantity.
	else {																   // else, crease a new node and insert it into the tree
		chemNode* node = makeNode(buy->chem_code, buy->chem_name, buy->quantity);
		insertChemNode(currInv, node);
	}


}

void fprintTree(FILE* invFile, chemNode* invNode)
/* Recursive in-order print the tree into invFile*/
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
	fseek(currErr, 0, SEEK_SET);
	char c = fgetc(currErr);	   // copy currErr content into errFile
	while (c != EOF) {
		fputc(c, errFile);
		c = fgetc(currErr);
	}
	fclose(errFile);

}

void deleteTree(chemTree* invTree)
{
	deleteTree1(invTree->root);
	invTree->root = NULL;
}

void deleteTree1(chemNode* invNode)
/* Recursively delete each node of the tree, free it's allocated memory*/
{	
	if (invNode != NULL)
	{
		if (invNode->left != NULL)
			deleteTree1(invNode->left);
		if (invNode->right != NULL)
			deleteTree1(invNode->right);
		free(invNode);
		
	}
}

chemNode* makeNode(char* chem_code, char* chem_name, long quantity)
/* Initialize a new node with chem_dode and chem_name fields */
{	
	chemNode* Node = (chemNode*)malloc(sizeof(chemNode));
	Node->data.quantity = quantity;
	strcpy_s(Node->data.chem_code, 20, chem_code);
	strcpy_s(Node->data.chem_name, 40, chem_name);
	Node->left = Node->right = NULL;
	return Node;
}

chemNode* findNode(char* chem_code, chemNode* Node)
/* Binary search a node by it's chem_code */
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

chemNode* deleteNode(chemNode* root, char* chem_code)
{
	// base case
	if (root == NULL)
		return root;

	// If the key to be deleted
	// is smaller than the root's
	// key, then it lies in left subtree
	if (strcmp(chem_code, root->data.chem_code) < 0)
		root->left = deleteNode(root->left, chem_code);

	// If the key to be deleted
	// is greater than the root's
	// key, then it lies in right subtree
	else if (strcmp(chem_code, root->data.chem_code) > 0)
		root->right = deleteNode(root->right, chem_code);
	// if key is same as root's key,
	// then this is the node
	// to be deleted
	else {
		// node with only one child or no child
		if (root->left == NULL) {
			chemNode* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			chemNode* temp = root->left;
			free(root);
			return temp;
		}

		// node with two children:
		// Get the inorder successor
		// (smallest in the right subtree)
		chemNode* temp = minValueNode(root->right);

	
		// Copy the inorder successor's content to this node
		strcpy(root->data.chem_code, temp->data.chem_code);

		// Delete the inorder successor
		root->right = deleteNode(root->right, temp->data.chem_code);
	}
	return root;
}

chemNode* minValueNode(chemNode* node)
{
	chemNode* current = node;

	/* loop down the tree to find the leftmost leaf */
	while (current && current->left != NULL)
		current = current->left;

	return current;
}




