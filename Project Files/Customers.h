

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Logs.h"
#include "Employee.h"

#ifndef _CUSTOMERS_H_
#define _CUSTOMERS_H_



typedef struct Customer {
	int ID;
	char Fullname[40];
	char address[100];
	char phone[20];
	time_t date;
} customer;

typedef struct Node {
	customer data;
	struct Node* next;
} node;

typedef struct LinkedList {
	node* head;
	node* tail;
} linked_list;

node* createListNode(customer* data)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = *data;
	newNode->next = NULL;
	return newNode;
}



void insertCustomer(linked_list** list, customer* customersData) {
	node* newNode = createListNode(customersData);

	if ((*list)->head == NULL) {
		(*list)->head = newNode;
		(*list)->tail = newNode;
	}
	else {
		(*list)->tail->next = newNode;
		(*list)->tail = newNode;
	}
}

void deleteCustomer(linked_list** list, int id) {
	node* curr = (*list)->head;
	node* prev = NULL;
	while (curr != NULL) {
		if (curr->data.ID == id) {
			if (prev == NULL) {
				(*list)->head = curr->next;
			}
			else {
				prev->next = curr->next;
			}
			if (curr->next == NULL) {
				(*list)->tail = prev;
			}
			free(curr);
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}

int getListSize(linked_list* list) {
	int size = 0;
	node* curr = list->head;
	while (curr != NULL) {
		size++;
		curr = curr->next;
	}
	return size;
}


customer* searchCustomerById(linked_list* list, int id) {
	node* curr = list->head;
	while (curr != NULL) {
		if (curr->data.ID == id) {
			return &curr->data;
		}
		curr = curr->next;
	}
	return NULL;
}

linked_list* searchCustomersByFullName(linked_list* list, const char* fullName) {
	linked_list* result = (linked_list*)malloc(sizeof(linked_list));
	result->head = NULL;
	result->tail = NULL;

	node* curr = list->head;
	while (curr != NULL) {
		if (strcmp(curr->data.Fullname, fullName) == 0) {
			insertCustomer(&result, &curr->data);
		}
		curr = curr->next;
	}
	return result;
}

linked_list* searchCustomersByAddress(linked_list* list, const char* address) {
	linked_list* result = (linked_list*)malloc(sizeof(linked_list));
	result->head = NULL;
	result->tail = NULL;

	node* curr = list->head;
	while (curr != NULL) {
		if (strcmp(curr->data.address, address) == 0) {
			insertCustomer(&result, &curr->data);
		}
		curr = curr->next;
	}
	return result;
}

linked_list* searchCustomersByPhone(linked_list* list, const char* phone) {
	linked_list* result = (linked_list*)malloc(sizeof(linked_list));
	result->head = NULL;
	result->tail = NULL;

	node* curr = list->head;
	while (curr != NULL) {
		if (strcmp(curr->data.phone, phone) == 0) {
			insertCustomer(&result, &curr->data);
		}
		curr = curr->next;
	}
	return result;
}

linked_list* searchCustomersByDate(linked_list* list, int day, int month, int year, int after) {
	linked_list* result = (linked_list*)malloc(sizeof(linked_list));
	result->head = NULL;
	result->tail = NULL;

	struct tm searchDate;
	searchDate.tm_sec = 0;
	searchDate.tm_min = 0;
	searchDate.tm_hour = 0;
	searchDate.tm_mday = day;
	searchDate.tm_mon = month - 1; 
	searchDate.tm_year = year - 1900;

	node* curr = list->head;
	while (curr != NULL) {
		struct tm customerDate = *localtime(&curr->data.date);
		if (after) {
			if (mktime(&customerDate) >= mktime(&searchDate)) {
				insertCustomer(&result, &curr->data);
			}
		}
		else {
			if (mktime(&customerDate) <= mktime(&searchDate)) {
				insertCustomer(&result, &curr->data);
			}
		}
		curr = curr->next;
	}
	return result;
}

linked_list* loadCustomers()
{
	linked_list* list = (linked_list*)malloc(sizeof(linked_list));
	list->head = NULL;
	list->tail = NULL;
	FILE* fp = fopen("customers.bin", "rb");
	if (fp == NULL) {
		return list;
	}
	customer* customersData = (customer*)malloc(sizeof(customer));
	while (fread(customersData, sizeof(customer), 1, fp) == 1) {
		insertCustomer(&list, customersData);
	}
	fclose(fp);
	return list;
}

void saveCustomers(linked_list* list)
{
	FILE* fp = fopen("customers.bin", "wb");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}
	node* curr = list->head;
	while (curr != NULL)
	{
		fwrite(&curr->data, sizeof(customer), 1, fp);
		curr = curr->next;
	}
	fclose(fp);
}


void freeList(linked_list* list)
{
	node* curr = list->head;
	while (curr != NULL)
	{
		node* temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(list);
}


void printList() {
	linked_list* list = loadCustomers();
	node* curr = list->head;
	while (curr != NULL) {
		printf("Customer ID: %d\n", curr->data.ID);
		printf("Customer Fullname: %s\n", curr->data.Fullname);
		printf("Customer Address: %s\n", curr->data.address);
		printf("Customer Phone: %s\n", curr->data.phone);
		printf("Customer Date: %s\n", ctime(&curr->data.date));
		printf("--------------------------------------------------\n");
		curr = curr->next;
	}
	freeList(list);
}


void addNewCustomer(linked_list** list) {
	customer newCustomer;
	time_t currentTime;

	time(&currentTime);
	
	if ((*list)->tail != NULL) {
		newCustomer.ID = (*list)->tail->data.ID + 1;
	}

	else {
		newCustomer.ID = 1;
	}
	printf("Enter customer full name: ");
	fgets(newCustomer.Fullname, sizeof(newCustomer.Fullname), stdin);
	fgets(newCustomer.Fullname, sizeof(newCustomer.Fullname), stdin);


	printf("Enter customer address: ");
	fgets(newCustomer.address, sizeof(newCustomer.address), stdin);

	printf("Enter customer phone: ");
	fgets(newCustomer.phone, sizeof(newCustomer.phone), stdin);

	newCustomer.date = currentTime;

	insertCustomer(list, &newCustomer);

	saveCustomers(*list);

	logMessage("Customer added", newCustomer.Fullname);


}

void deleteCustomerFromList(linked_list** list) {
	int id;
	printf("Enter customer ID: ");
	scanf("%d", &id);
	deleteCustomer(list, id);
	saveCustomers(*list);
}


void printAllCustomers(linked_list* list)
{
	node* curr = list->head;
	while (curr != NULL)
	{
		printf("Customer ID: %d\n", curr->data.ID);
		printf("Customer Fullname: %s\n", curr->data.Fullname);
		printf("Customer Address: %s\n", curr->data.address);
		printf("Customer Phone: %s\n", curr->data.phone);
		printf("Customer Date: %s\n", ctime(&curr->data.date));
		printf("--------------------------------------------------\n");
		curr = curr->next;
	}
}


void searchCustomersByIDPrompt(linked_list* list) {
	int id;
	printf("Enter ID: ");
	scanf("%d", &id);
	customer* customer = searchCustomerById(list, id);
	if (customer != NULL) {
		printf("Customer ID: %d\n", customer->ID);
		printf("Customer Fullname: %s\n", customer->Fullname);
		printf("Customer Address: %s\n", customer->address);
		printf("Customer Phone: %s\n", customer->phone);
		printf("Customer Date: %s\n", ctime(&customer->date));
		printf("--------------------------------------------------\n");
	}
	else {
		printf("Customer not found\n");
	}
}

void searchCustomersByNamePrompt(linked_list* list) {
	char name[40];
	printf("Enter name: ");
	fgets(name, sizeof(name), stdin);
	fgets(name, sizeof(name), stdin);
	linked_list* result = searchCustomersByFullName(list, name);
	printf("Number of customers found: %d\n", getListSize(result));
	if (result->head != NULL) {
		printf("Customers:\n");
		printList(result);
	}
	else {
		printf("No customers found.\n");
	}
}

void searchCustomersByAddressPrompt(linked_list* list) {
	char address[100];
	printf("Enter address: ");
	fgets(address, sizeof(address), stdin);
	fgets(address, sizeof(address), stdin);
	
	linked_list* result = searchCustomersByAddress(list, address);
	printf("Number of customers found: %d\n", getListSize(result));
	if (result->head != NULL) {
		printf("Customers:\n");
		printList(result);
	}
	else {
		printf("No customers found.\n");
	}
}

void searchCustomersByPhonePrompt(linked_list* list) {
	char phone[20];
	printf("Enter phone number: ");
	fgets(phone, sizeof(phone), stdin);
	fgets(phone, sizeof(phone), stdin);
	linked_list* result = searchCustomersByPhone(list, phone);
	printf("Number of customers found: %d\n", getListSize(result));
	if (result->head != NULL) {
		printf("Customers:\n");
		printList(result);
	}
	else {
		printf("No customers found.\n");
	}
}


void searchCustomersByDatePrompt(linked_list* list) {
	int day, month, year, after;
	printf("Enter date (dd-mm-yyyy): ");
	scanf("%d-%d-%d", &day, &month, &year);
	printf("Search for dates (1) after or (0) before or on the entered date: ");
	scanf("%d", &after);

	linked_list* result = searchCustomersByDate(list, day, month, year, after);

	printf("Number of customers found: %d\n", getListSize(result));
	if (result->head != NULL) {
		printf("Customers:\n");
		printList(result);
	}
	else {
		printf("No customers found.\n");
	}
}



void searchCustomerMenu() {
	int choice;
	linked_list* list = loadCustomers();
	while (1)
	{
		printf("1- Search by ID\n");
		printf("2- Search by name\n");
		printf("3- Search by address\n");
		printf("4- Search by phone\n");
		printf("5- Search by date\n");
		printf("6- Display all customers fields\n");
		printf("7- Back\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			searchCustomersByIDPrompt(list);
			break;
		case 2:
			searchCustomersByNamePrompt(list);
			break;
		case 3:
			searchCustomersByAddressPrompt(list);
			break;
		case 4:
			searchCustomersByPhonePrompt(list);
			break;
		case 5:
			searchCustomersByDatePrompt(list);
			break;
		case 6:
			printAllCustomers(list);
			break;
		case 7:
			freeList(list);
			return;
		default:
			printf("Invalid choice\n");
			break;
		}
	}
}


#endif 
