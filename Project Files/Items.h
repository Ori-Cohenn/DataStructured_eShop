


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Logs.h"


#ifndef _ITEMS_H_
#define _ITEMS_H_


typedef struct item {
	int ID;
	char productName[32];
	char category[32];
	char brand[32];
	double price;
	bool isAvailable;
	int amount;
	time_t date;
}item;

typedef struct node {
	item* data;
	struct node* left;
	struct node* right;
}itemNode;

itemNode* createNode(item* data) {
	itemNode* newNode = (itemNode*)malloc(sizeof(itemNode));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

itemNode* searchItem(itemNode** root, int ID)
{
	if (*root == NULL || (*root)->data->ID == ID)
		return *root;

	if ((*root)->data->ID < ID)
		return searchItem(&(*root)->right, ID);

	return searchItem(&(*root)->left, ID);
}


itemNode* insertNode(itemNode** root, item* data) {
	if (*root == NULL) {
		*root = createNode(data);
	}
	else if (data->ID < (*root)->data->ID) {
		(*root)->left = insertNode(&(*root)->left, data);
	}
	else if (data->ID > (*root)->data->ID) {
		(*root)->right = insertNode(&(*root)->right, data);
	}
	return *root;
}


itemNode* findMin(itemNode* root) {
	if (root == NULL) {
		return NULL;
	}
	if (root->left == NULL) {
		return root;
	}
	return findMin(root->left);
}

itemNode* deleteNode(itemNode** root, int ID) {
	if (*root == NULL) {
		return NULL;
	}
	if ((*root)->data->ID > ID) {
		return deleteNode(&(*root)->left, ID);
	}
	else if ((*root)->data->ID < ID) {
		return deleteNode(&(*root)->right, ID);
	}
	else {
		if ((*root)->left == NULL && (*root)->right == NULL) {
			free(*root);
			*root = NULL;
			return *root;
		}
		else if ((*root)->left == NULL) {
			itemNode* temp = *root;
			*root = (*root)->right;
			free(temp);
			return *root;
		}
		else if ((*root)->right == NULL) {
			itemNode* temp = *root;
			*root = (*root)->left;
			free(temp);
			return *root;
		}
		else {
			itemNode* temp = findMin((*root)->right);
			(*root)->data = temp->data;
			return deleteNode(&(*root)->right, temp->data->ID);
		}
	}
}

itemNode* loadItems() {
	FILE* fp = fopen("items.bin", "rb");
	if (fp == NULL) {
		printf("Error opening file\n");
		return NULL;
	}
	itemNode* root = NULL;
	item* data = (item*)malloc(sizeof(item));
	while (fread(data, sizeof(item), 1, fp) == 1) {
		insertNode(&root, data);
		data = (item*)malloc(sizeof(item));
	}
	
	fclose(fp);
	free(data);

	return root;
}

void saveTree(itemNode* root, FILE* fp) {
	if (root == NULL) {
		return;
	}
	saveTree(root->left, fp);
	fwrite(root->data, sizeof(item), 1, fp);
	saveTree(root->right, fp);
}


void freeTree(itemNode* root) {
	if (root == NULL) {
		return;
	}
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

itemNode* searchByCategory(itemNode* root, char* category, itemNode** result) {
	if (root == NULL) {
		return NULL;
	}
	if (strcmp(root->data->category, category) == 0) {
		insertNode(result, root->data);
	}
	searchByCategory(root->left, category, result);
	searchByCategory(root->right, category, result);
}


itemNode* searchByBrand(itemNode* root, char* brand, itemNode** result) {
	if (root == NULL) {
		return NULL;
	}
	if (strcmp(root->data->brand, brand) == 0) {
		insertNode(result, root->data);
	}
	searchByBrand(root->left, brand, result);
	searchByBrand(root->right, brand, result);
}
itemNode* searchByCategoryAndBrand(itemNode* root, char* category, char* brand, itemNode** result) {
	if (root == NULL) {
		return NULL;
	}
	if (strcmp(root->data->category, category) == 0 && strcmp(root->data->brand, brand) == 0) {
		insertNode(result, root->data);
	}
	searchByCategoryAndBrand(root->left, category, brand, result);
	searchByCategoryAndBrand(root->right, category, brand, result);
}
itemNode* searchByPrice(itemNode* root, float price, int above, itemNode** result) {
	if (root == NULL) {
		return NULL;
	}
	if (above == 0) {
		if (root->data->price <= price) {
			insertNode(result, root->data);
		}
	}
	else {
		if (root->data->price > price) {
			insertNode(result, root->data);
		}
	}
	searchByPrice(root->left, price, above, result);
	searchByPrice(root->right, price, above, result);
}
itemNode* searchByAvailability(itemNode* root, int isAvailable, itemNode** result) {
	if (root == NULL) {
		return NULL;
	}
	if (root->data->isAvailable == isAvailable) {
		insertNode(result, root->data);
	}
	searchByAvailability(root->left, isAvailable, result);
	searchByAvailability(root->right, isAvailable, result);
}
itemNode* findItemsByDate(itemNode* root, time_t entered_date, int before_or_after, itemNode** result) {
	if (root == NULL) {
		return;
	}
	if ((before_or_after == 0 && difftime(root->data->date, entered_date) <= 0) ||
		(before_or_after == 1 && difftime(root->data->date, entered_date) > 0)) {
		insertNode(result, root->data);
	}
	findItemsByDate(root->left, entered_date, before_or_after, result);
	findItemsByDate(root->right, entered_date, before_or_after, result);
}


void addNewItem(itemNode** itemTree) {
	item* newItem = (item*)malloc(sizeof(item));
	system("cls");
	while (1) {
		printf("Enter the ID of the item: ");
		scanf("%d", &newItem->ID);
		if (searchItem(itemTree, newItem->ID) == NULL) {
			break;
		}
		printf("The ID is already exist, please enter another ID\n");
	}

	//there is a problem when you want to add an item  there is a problem if you not use gets() twice!!!
	printf("Enter the product name: ");
	gets(newItem->productName);
	gets(newItem->productName);

	printf("Enter the category: ");
	gets(newItem->category);

	printf("Enter the brand: ");
	gets(newItem->brand);

	while (1) {
		printf("Enter the price: ");
		scanf("%lf", &newItem->price);
		if (newItem->price > 0) {
			break;
		}
		printf("The price should be greater than zero\n");
	}

	while (1) {
		printf("Enter the quantity: ");
		scanf("%d", &newItem->amount);
		if (newItem->amount > 0) {
			break;
		}
		printf("The amount should be greater than zero\n");
	};
	//if the amount is 0, the item is out of stock
	if (newItem->amount == 0) {
		newItem->isAvailable = 0;
	}
	else {
		newItem->isAvailable = 1;
	}

	newItem->date = time(NULL);

	insertNode(itemTree, newItem);

	FILE* fp = fopen("items.bin", "wb");
	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}
	saveTree(*itemTree, fp);
	fclose(fp);


	char log[100];
	sprintf(log, "New item added: %s, amount: %d", newItem->productName, newItem->amount);
	logMessage(log);

}

void deleteItem(itemNode** itemTree) {
	int ID;
	printf("Enter the ID of the item: ");
	scanf("%d", &ID);

	while (searchItem(itemTree, ID) == NULL) {
		printf("The item is not found, please enter another ID\n");
		printf("Enter the ID of the item: ");
		scanf("%d", &ID);
	}

	char log[100];
	sprintf(log, "Item deleted: %s, ID: %d", searchItem(itemTree, ID)->data->productName, searchItem(itemTree, ID)->data->ID);
	logMessage(log);


	deleteNode(itemTree, ID);

	FILE* fp = fopen("items.bin", "wb");
	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}
	saveTree(*itemTree, fp);
	fclose(fp);



}

void printTreeItems(itemNode* root) {
	if (root == NULL) {
		return;
	}
	printf("ID: %d\n", root->data->ID);
	printf("Product name: %s\n", root->data->productName);
	printf("Category: %s\n", root->data->category);
	printf("Brand: %s\n", root->data->brand);
	printf("Price: %.2lf\n", root->data->price);
	printf("Amount: %d\n", root->data->amount);
	printf("Date: %s\n", ctime(&root->data->date));
	if (root->data->isAvailable == 1) {
		printf("Available\n");
	}
	else {
		printf("Not available\n");
	}
	printf("-------------------------------------------------\n");
	if (root->left != NULL) {
		printTreeItems(root->left);
	}
	if (root->right != NULL) {
		printTreeItems(root->right);
	}
}

void editItem(itemNode** itemTree) {
	FILE* fp;
	int ID;
	printf("Enter the ID of the item: ");
	scanf("%d", &ID);

	while (searchItem(itemTree, ID) == NULL) {
		printf("The item is not found, please enter another ID\n");
		printf("Enter the ID of the item: ");
		scanf("%d", &ID);
	}
	
	while (1) {
		printf("What do you want to edit?\n");
		printf("1. Product name\n");
		printf("2. Category\n");
		printf("3. Brand\n");
		printf("4. Price\n");
		printf("5. Amount\n");
		printf("6. Exit\n");
		int choice = 0;
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("Enter the new product name: ");
			gets(searchItem(itemTree, ID)->data->productName);
			gets(searchItem(itemTree, ID)->data->productName);
			break;
		case 2:
			printf("Enter the new category: ");
			gets(searchItem(itemTree, ID)->data->category);
			gets(searchItem(itemTree, ID)->data->category);
			break;
		case 3:
			printf("Enter the new brand: ");
			gets(searchItem(itemTree, ID)->data->brand);
			gets(searchItem(itemTree, ID)->data->brand);
			break;
		case 4:
			while (1) {
				printf("Enter the new price: ");
				scanf("%lf", &searchItem(itemTree, ID)->data->price);
				if (searchItem(itemTree, ID)->data->price > 0) {
					break;
				}
				printf("The price should be greater than zero\n");
			}
			break;
		case 5:
			printf("Enter the new amount: ");
			scanf("%d", &searchItem(itemTree, ID)->data->amount);
			//if the amount is 0 make item isAvailable = 0
			if (searchItem(itemTree, ID)->data->amount == 0) {
				searchItem(itemTree, ID)->data->isAvailable = 0;
			}
			//if the amount is greater than 0 make item isAvailable = 1
			else if (searchItem(itemTree, ID)->data->amount > 0) {
				searchItem(itemTree, ID)->data->isAvailable = 1;
			}
			break;
		case 6:
			fp = fopen("items.bin", "wb");
			if (fp == NULL) {
				printf("Error opening fp\n");
				return;
			}
			saveTree(*itemTree, fp);
			fclose(fp);
			char log[100];
			sprintf(log, "Item edited: %s, ID: %d", searchItem(itemTree, ID)->data->productName, searchItem(itemTree, ID)->data->ID);
			logMessage(log);
			return;
		default:
			printf("Invalid choice\n");
			break;
		}
	}
}

void printItem(itemNode* root, int ID) {
	if (root == NULL) {
		return;
	}
	if (root->data->ID == ID) {
		printf("ID: %d\n", root->data->ID);
		printf("Product name: %s\n", root->data->productName);
		printf("Category: %s\n", root->data->category);
		printf("Brand: %s\n", root->data->brand);
		printf("Price: %.2lf\n", root->data->price);
		printf("Amount: %d\n", root->data->amount);
		printf("Date: %s\n", ctime(&root->data->date));
		if (root->data->isAvailable == 1) {
			printf("Available\n");
		}
		else {
			printf("Not available\n");
		}
		printf("-------------------------------------------------\n");
	}
	if (root->left != NULL) {
		printItem(root->left, ID);
	}
	if (root->right != NULL) {
		printItem(root->right, ID);
	}
}


itemNode* findbyDatePrompt(itemNode* itemTree, int acessLevel) {
	int day, month, year;
	int before_or_after;

	printf("Enter a date (in format DD-MM-YYYY): ");
	scanf("%d-%d-%d", &day, &month, &year);

	struct tm date = { 0 };
	date.tm_mday = day;
	date.tm_mon = month - 1;
	date.tm_year = year - 1900;
	date.tm_isdst = -1;
	time_t entered_date = mktime(&date);
	
	itemNode* dateTree = NULL;
	printf("Enter 0 to search items on or before the date, or 1 to search items after the date: ");
	scanf("%d", &before_or_after);

	findItemsByDate(itemTree, entered_date, before_or_after, &dateTree);
	printTreeItems(dateTree);

	if (dateTree == NULL) {
		printf("No items found\n");
		return NULL;
	}


	if (acessLevel > 1)
	{
		char choice;
		int ID;
		printf("Do you want to edit one of the items? (y/n): ");
		scanf(" %c", &choice);

		if (choice == 'y') {
			while (1) {
				printf("Enter the ID: ");
				scanf("%d", &ID);
				if (searchItem(&dateTree, ID) == NULL) {
					printf("ID not found\n");
				}
				else {
					editItem(&itemTree);
					break;
				}
			}
		}

	}

	return dateTree;
}
itemNode* searchByCategoryPrompt(item* itemTree, int acessLevel)
{

	printf("Enter the Category: ");
	char category[50];
	gets(category);
	gets(category);
	itemNode* categoryTree = NULL;
	searchByCategory(itemTree, category, &categoryTree);
	printTreeItems(categoryTree);

	if (categoryTree == NULL) {
		printf("No items found\n");
		return NULL;
	}

	if (acessLevel > 1)
	{
		char choice;
		int ID;
		printf("Do you want to edit one of the items? (y/n): ");
		scanf(" %c", &choice);
		if (choice == 'y') {
			while (1) {
				printf("Enter the ID: ");
				scanf("%d", &ID);
				if (searchItem(&categoryTree, ID) == NULL) {
					printf("ID not found\n");
				}
				else {
					editItem(&itemTree);
					break;
				}
			}
		}
	}

	return categoryTree;

}
itemNode* searchByBrandPrompt(item* itemTree, int acessLevel)
{
	printf("Enter the Brand: ");
	char brand[50];
	gets(brand);
	gets(brand);
		
	itemNode* brandTree = NULL;
	searchByBrand(itemTree, brand, &brandTree);
	printTreeItems(brandTree);
	if (brandTree == NULL) {
		printf("No items found\n");
		return NULL;
	}

	if (acessLevel > 1)
	{
		char choice;
		int ID;
		printf("Do you want to edit one of the items? (y/n): ");
		scanf(" %c", &choice);
		if (choice == 'y') {
			while (1) {
				printf("Enter the ID: ");
				scanf("%d", &ID);
				if (searchItem(&brandTree, ID) == NULL) {
					printf("ID not found\n");
				}
				else {
					editItem(&itemTree);
					break;
				}
			}
		}
	}

	return brandTree;


}
itemNode* searchByCategoryAndBrandPrompt(item* itemTree, int acessLevel)
{
	
	printf("Enter the Category: ");
	char category2[50];
	gets(category2);
	gets(category2);
	
	printf("Enter the Brand: ");
	char brand2[50];
	gets(brand2);
	
	itemNode* categoryBrandTree = NULL;
	searchByCategoryAndBrand(itemTree, category2, brand2, &categoryBrandTree);
	printTreeItems(categoryBrandTree);
	if (categoryBrandTree == NULL) {
		printf("No items found\n");
		return NULL;
	}

	if (acessLevel > 1)
	{
		char choice;
		int ID;
		printf("Do you want to edit one of the items? (y/n): ");
		scanf("%c", &choice);

		if (choice == 'y') {
			while (1) {
				printf("Enter the ID: ");
				scanf("%d", &ID);
				if (searchItem(&categoryBrandTree, ID) == NULL) {
					printf("ID not found\n");
				}
				else {
					editItem(&itemTree);
					break;
				}
			}
		}
	}

	return categoryBrandTree;

}
itemNode* searchByPricePrompt(item* itemTree, int acessLevel)
{
	
	printf("Enter the price you want to sort by: ");
	double price;
	scanf("%lf", &price);
	printf("Do you want to search for items under and equal to the price or above the price?\n");
	printf("0. Under and equal to the price\n");
	printf("1. Above the price\n");
	int choice;
	scanf("%d", &choice);
	
	while (choice != 0 && choice != 1) {
		printf("Invalid choice, the choices should be only 0 or 1\n");
		printf("Try Again\n");
		scanf("%d", &choice);
	}
	itemNode* priceTree = NULL;
	searchByPrice(itemTree, price, choice, &priceTree);
	printTreeItems(priceTree);
	if (priceTree == NULL) {
		printf("No items found\n");
		return NULL;
	}
	

	if (acessLevel > 1)
	{
		char choice2;
		int ID;
		printf("Do you want to edit one of the items? (y/n): ");
		scanf("%c", &choice2);
		scanf("%c", &choice2);


		if (choice2 == 'y') {
			while (1) {
				printf("Enter the ID: ");
				scanf("%d", &ID);
				if (searchItem(&priceTree, ID) == NULL) {
					printf("ID not found\n");
				}
				else {
					editItem(&itemTree);
					break;
				}
			}
		}
	}
	
	return priceTree;
}
itemNode* searchByAvailabilityPrompt(item* itemTree, int acessLevel)
{
	
	printf("Do you want to search for available items or not available items?\n");
	printf("1. Available items\n");
	printf("0. Not available items\n");
	int choice1;
	scanf("%d", &choice1);

	itemNode* availabilityTree = NULL;
	searchByAvailability(itemTree, choice1, &availabilityTree);
	printTreeItems(availabilityTree);
	
	while (choice1 != 0 && choice1 != 1) {
		printf("Invalid choice, the choices should be only 0 or 1\n");
		printf("Try Again\n");
		scanf("%d", &choice1);
	}
	if (availabilityTree == NULL) {
		printf("No items found\n");
		return NULL;
	}
	if (availabilityTree == NULL) {
		printf("No items found\n");
		return NULL;
	}

	if (acessLevel > 1)
	{
		char choice2;
		int ID;
		printf("Do you want to edit one of the items? (y/n): ");
		scanf("%c", &choice2);
		scanf("%c", &choice2);

		if (choice2 == 'y') {
			while (1) {
				printf("Enter the ID: ");
				scanf("%d", &ID);
				if (searchItem(&availabilityTree, ID) == NULL) {
					printf("ID not found\n");
				}
				else {
					editItem(&itemTree);
					break;
				}
			}
		}

	}

	return availabilityTree;
}


void searchItemMenu(itemNode* itemTree, int acessLevel) {
	int ChooseID;
	char myChoice;

	while (1) {
		printf("What do you want to search for?\n");
		printf("1. ID\n");
		printf("2. Category\n");
		printf("3. Brand\n");
		printf("4. Category and brand\n");
		printf("5. Price range\n");
		printf("6. Available items\n");
		printf("7. Date when the items been added\n");
		printf("8. Exit\n");
		int choice;
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("Enter the ID: ");
			int ID;
			scanf("%d", &ID);
			printItem(itemTree, ID);
			break;

		case 2:
			system("cls");
			itemNode* categoryTree = searchByCategoryPrompt(itemTree, acessLevel);
			freeTree(categoryTree);
			break;

		case 3:
			system("cls");
			itemNode* brandTree = searchByBrandPrompt(itemTree, acessLevel);
			freeTree(brandTree);
			break;
			
		case 4:
			system("cls");
			itemNode* categoryBrandTree = searchByCategoryAndBrandPrompt(itemTree, acessLevel);
			freeTree(categoryBrandTree);
			break;

		case 5:
			system("cls");
			itemNode* priceTree = searchByPricePrompt(itemTree, acessLevel);
			freeTree(priceTree);
			break;

		case 6:
			system("cls");
			itemNode* availabilityTree = searchByAvailabilityPrompt(itemTree, acessLevel);
			freeTree(availabilityTree);
			break;
		case 7:
			system("cls");
			itemNode* dateTree = findbyDatePrompt(itemTree, acessLevel);
			freeTree(dateTree);
			
			break;
			
		case 8:
			return;
		default:
			printf("Invalid choice\n");
			break;
		}
	}
}


#endif

