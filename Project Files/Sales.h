

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Items.h"
#include "Employee.h"
#include "Customers.h"
#include "Logs.h"
#include <time.h>

#ifndef _Sales_H_
#define _Sales_H_

typedef struct Sales {
	int SN;// serial number-> running number 1 2 3 ...
	int customerID;
	int itemID;
	time_t purchaseDate;// purchaseDate
}sales;

typedef struct SaleNode {
	sales data;
	struct SaleNode* next;
} saleNode;


typedef struct SalesList {
	saleNode* head;
	int size;
} salesList;

//call the functions
void saveSales(salesList* list); salesList* loadSales();



// Function to create a new sale node
saleNode* createSaleNode(sales* data) {
	saleNode* newNode = (saleNode*)malloc(sizeof(saleNode));
	newNode->data = *data;
	newNode->next = NULL;
	return newNode;
}


//this function is only for checking the refund function and to add an old sale that made more than 14 days ago
void insertOldSale(salesList** list, int SN, int customerID, int itemID, int year, int month, int day, int hour, int min, int sec) {
	struct tm purchaseDate = { 0 };
	purchaseDate.tm_year = year - 1900;
	purchaseDate.tm_mon = month - 1;
	purchaseDate.tm_mday = day;
	purchaseDate.tm_hour = hour;
	purchaseDate.tm_min = min;
	purchaseDate.tm_sec = sec;
	time_t purchaseDate_t = mktime(&purchaseDate);

	sales newSale = { SN, customerID, itemID, purchaseDate_t };
	
	saleNode* newNode = createSaleNode(&newSale);
	newNode->next = (*list)->head;
	(*list)->head = newNode;
	(*list)->size++;
	saveSales(*list);
}

salesList* createSalesList() {
	salesList* list = (salesList*)malloc(sizeof(salesList));
	list->head = NULL;
	list->size = 0;
	return list;
}

void insertSale(salesList** list, sales* data) {
	saleNode* newNode = createSaleNode(data);
	newNode->next = (*list)->head;
	(*list)->head = newNode;
	(*list)->size++;
}

saleNode* searchSale(salesList* list, int SN) {
	saleNode* current = list->head;
	while (current != NULL) {
		if (current->data.SN == SN) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void deleteSale(salesList** list, int SN) {
	saleNode* current = (*list)->head;
	saleNode* previous = NULL;
	while (current != NULL) {
		if (current->data.SN == SN) {
			if (previous == NULL) {
				(*list)->head = current->next;
			}
			else {
				previous->next = current->next;
			}
			free(current);
			(*list)->size--;
			return;
		}
		previous = current;
		current = current->next;
	}
	saveSales(list);

	char log[100];
	sprintf(log, "Sale %d deleted", SN);
	logMessage(log);
	
}

void refundSale(salesList* list, itemNode* itemTree, saleNode* currentSale) {
	//check if the the item purchased within 14 days
	time_t timeNow = time(NULL);
	if (difftime(timeNow, currentSale->data.purchaseDate) > (14*24*60*60)) {
		printf("The item was purchased more than 14 days ago, it cannot be refunded.\n");
		return;
	}
	//check if the item is in stock
	itemNode* item = searchItem(&itemTree, currentSale->data.itemID);
	item->data->amount++;
	//if the item amount was 0 and now 1 then make the item available again
	if (item->data->amount == 1) {
		item->data->isAvailable = true;
	}
	
	char log[100];
	sprintf(log, "Item %s was refunded by the customer with the ID - %d", item->data->productName, currentSale->data.customerID);
	logMessage(log);

	deleteSale(&list, currentSale->data.SN);
	
	FILE* fp = fopen("items.bin", "wb");
	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}
	saveTree(itemTree, fp);
	fclose(fp);
	
	saveSales(list);
}

void printSalesList(salesList* list) {
	saleNode* current = list->head;
	itemNode* itemTree = loadItems();
	linked_list* customersList = loadCustomers();
	
	while (current != NULL) {
		itemNode* item = searchItem(&itemTree, current->data.itemID);
		customer* customer = searchCustomerById(customersList, current->data.customerID);
		printf("Serial number: %d\n", current->data.SN);
		printf("Customer ID: %d\n", current->data.customerID);
		printf("Customer name: %s", customer->Fullname);
		printf("Item ID: %d\n", current->data.itemID);
		printf("Product name: %s\n", item->data->productName);
		printf("Purchase date: %s\n", ctime(&current->data.purchaseDate));
		current = current->next;
	}

}


void displayCustomerProducts(salesList* list, int customerID) {
	saleNode* current = list->head;
	itemNode* itemTree = loadItems();
	linked_list* customersList = loadCustomers();
	customer* customer = searchCustomerById(customersList, customerID);

	if (current == NULL) {
		printf("No Items found for this customer\n");
		return;
	}
	while (current != NULL) {
		if (current->data.customerID == customerID) {
			printf("Serial number of the purchase: %d\n", current->data.SN);
			printf("Customer ID: %d\n", current->data.customerID);
			printf("Customer Name: %s", customer->Fullname);
			printf("Item ID: %d\n", current->data.itemID);
			printf("Item Name: %s\n", searchItem(&itemTree, current->data.itemID)->data->productName);
			printf("Purchase Date: %s\n", ctime(&(current->data.purchaseDate)));
			printf("--------------------------------------------------\n");
		}
		current = current->next;
	}
	freeTree(itemTree);
	freeList(customersList);
}


int checkCustomerItems(salesList* list, int customerID) {
	saleNode* current = list->head;
	int count = 0;
	while (current != NULL) {
		if (current->data.customerID == customerID) {
			count++;
		}
		current = current->next;
	}
	return count;
}

void sellItem(salesList** list, int customerID, int itemID) {
	sales sale;
	if ((*list)->head != NULL) {
		sale.SN = (*list)->head->data.SN + 1;
	}
	else {
		sale.SN = 1;
	}
	sale.customerID = customerID;
	sale.itemID = itemID;
	time(&sale.purchaseDate);
	insertSale(list, &sale);

	saveSales(*list);

	char log[100];
	sprintf(log, "Customer %d bought item id %d", sale.customerID, sale.itemID);
	logMessage(log);
}

void freeSalesList(salesList* list) {
	saleNode* current = list->head;
	while (current != NULL) {
		saleNode* next = current->next;
		free(current);
		current = next;
	}
	free(list);
}

void reverseSalesList(salesList** list) {
	saleNode* current = (*list)->head;
	saleNode* previous = NULL;
	saleNode* next = NULL;
	while (current != NULL) {
		next = current->next;
		current->next = previous;
		previous = current;
		current = next;
	}
	(*list)->head = previous;
}

salesList* loadSales() {
	salesList* list = createSalesList();
	FILE* fp = fopen("Sales.bin", "rb");
	if (fp == NULL) {
		printf("Create new file employees\n");
		// File doesn't exist, create it
		fp = fopen("Sales.bin", "wb");
		if (fp == NULL) {
			printf("Error creating file\n");
			return 0;
		}
	}
	sales data;
	while (fread(&data, sizeof(sales), 1, fp) == 1) {
		saleNode* newNode = createSaleNode(&data);
		newNode->next = list->head;
		list->head = newNode;
		list->size++;
	}
	fclose(fp);
	reverseSalesList(&list);
	return list;
}

void saveSales(salesList* list) {
	FILE* fp = fopen("Sales.bin", "wb");
	if (fp == NULL) {
		printf("Error saving sales\n");
		return;
	}
	saleNode* current = list->head;
	while (current != NULL) {
		fwrite(&current->data, sizeof(sales), 1, fp);
		current = current->next;
	}
	fclose(fp);
}

void sellItemPrompt(salesList* list, itemNode* itemTree, linked_list* customerList) {
	int customerID, itemID, serialNumber;
	printf("Enter customer ID: ");
	scanf("%d", &customerID);
	if (checkCustomerItems(list, customerID) == 3) {
		printf("Customer has reached the maximum number of items\n");
		return;
	}
	

	
	if (searchCustomerById(customerList, customerID) == NULL) {
		printf("Customer not found, creating new customer\n");
		if (list->head != NULL) {
			customerID = list->head->data.customerID + 1;
		}

		else {
			customerID = 1;
		}
		printf("The new customer ID is %d\n", customerID);
		addNewCustomer(&customerList);
	}
	
	printf("Enter item ID: ");
	scanf("%d", &itemID);
	itemNode* checkItem = NULL;
	checkItem = searchItem(&itemTree, itemID);
	if (checkItem == NULL) {
		printf("The item doesn't exist maybe in the future\n");
		return;
	}
	if (checkItem->data->isAvailable == 0 || checkItem->data->amount <= 0) {
		printf("The item isn't available try again in the future\n");
		return;
	}
	sellItem(&list, customerID, itemID);
	searchItem(&itemTree, itemID)->data->amount--;
	
	//check if the item amount is 0 so the item isn't available
	if (searchItem(&itemTree, itemID)->data->amount == 0) {
		searchItem(&itemTree, itemID)->data->isAvailable = 0;
	}


	FILE* fp = fopen("items.bin", "wb");
	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}
	saveTree(itemTree, fp);
	fclose(fp);
}

void storeManagement() {
	int choice = -1;
	int customerID, itemID, serialNumber;
	while (choice != 0) {
		salesList* list = loadSales();
		linked_list* customerList = loadCustomers();
		itemNode* itemTree = loadItems();
		printf("--- Store Management ---\n");
		printf("1. Display all products associated with the same customer\n");
		printf("2. Sell an item\n");
		printf("3. Refund an item\n");
		printf("4. Display all customers\n");
		printf("0. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice) {
		case 1: {
			system("cls");
			printf("Enter customer ID: ");
			scanf("%d", &customerID);
			if (searchCustomerById(customerList, customerID) == NULL) {
				printf("Customer not found\n");
			}
			else {
				displayCustomerProducts(list, customerID);
			}
			break;
		}
		case 2: {
			system("cls");
			sellItemPrompt(list, itemTree, customerList);
			break;
		}
		case 3: {
			system("cls");
			printf("Enter serial number of the item to be returned: ");
			scanf("%d", &serialNumber);
			saleNode* currentSale = searchSale(list, serialNumber);
			if (!currentSale) {
				printf("Serial number not found\n");
				break;
			}
			
			refundSale(list, itemTree, currentSale);
			break;
		}
		case 4: {
			system("cls");
			printSalesList(list);
			break;
		}
		case 0: {
			system("cls");
			freeTree(itemTree);
			freeSalesList(list);
			freeList(customerList);
			printf("Exiting store management...\n");
			break;
		}
		default: {
			printf("Invalid choice. Please try again.\n");
			break;
		}
		}
	}
}



#endif
