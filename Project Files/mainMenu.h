


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Items.h"
#include "Employee.h"
#include "Customers.h"
#include "Logs.h"
#include "Sales.h"


#ifndef _MAINMENU_H_
#define _MAINMENU_H_

void acessLevel1menu(int acessLevel)
{
	itemNode* itemTree = loadItems();
	system("cls");
	while (1)
	{
		int choice;
		printf("Wellcome to Acess Level 1 menu, please enter your choice: \n");
		printf("1. Store Management \n");
		printf("2. Search an item\n");
		printf("3. Add new item to the store\n");
		printf("4. Search customers\n");
		printf("5. Display all items in store\n");
		printf("6. Exit\n");
		printf("Enter your choice: ");

		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			storeManagement();
			break;
		case 2:
			searchItemMenu(itemTree, acessLevel);
			break;
		case 3:
			addNewItem(&itemTree);
			break;
		case 4:
			searchCustomerMenu();
			break;
		case 5:
			printTreeItems(itemTree);
			break;
			
		case 6:
			return;
			
		default:
			printf("Invalid input, please try again\n");
			break;
		}
	}
	freeTree(itemTree);
}

void acessLevel2menu(int acessLevel)
{
	system("cls");
	itemNode* itemTree = loadItems();
	while (1)
	{
		int choice;
		printf("Wellcome to Acess Level 2 menu, please enter your choice: \n");
		printf("1. Store Management \n");
		printf("2. Search an item\n");
		printf("3. Add new item to the store\n");
		printf("4. Search customers\n");
		printf("5. Display all items in store\n");
		printf("6. Delete an item\n");
		printf("7. Edit an item\n");
		printf("8. Exit\n");
		printf("Enter your choice: ");
		
		scanf("%d", &choice);
		
		switch (choice)
		{
		case 1:
			storeManagement();
			break;
			
		case 2:
			searchItemMenu(itemTree, acessLevel);
			break;
			
		case 3:
			addNewItem(&itemTree);
			break;

		case 4:
			searchCustomerMenu();
			break;
		case 5:
			printTreeItems(itemTree);
			break;
			
		case 6:
			deleteItem(&itemTree);
			break;
			
		case 7:
			editItem(&itemTree);
			break;
			
		case 8:
			return;
			
		default:
			printf("Invalid input, please try again\n");
			break;
		}
	}
	freeTree(itemTree);

}

void acessLevel3menu(int acessLevel)
{
	itemNode* itemTree = loadItems();
	system("cls");

	while (1)
	{
		int choice;
		printf("Wellcome to Admin (LVL 3) menu, please enter your choice: \n");
		printf("1. Store Management \n");
		printf("2. Search an item\n");
		printf("3. Add new item to the store\n");
		printf("4. Search customers\n");
		printf("5. Display all items in store\n");
		printf("6. Delete an item\n");
		printf("7. Edit an item\n");
		printf("8. Employee Menu\n");
		printf("9. Exit\n");
		
		printf("Enter your choice: ");

		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			storeManagement();
			break;

		case 2:
			searchItemMenu(itemTree, acessLevel);
			break;

		case 3:
			addNewItem(&itemTree);
			break;
		case 4:
			searchCustomerMenu();
			break;
		case 5:
			printTreeItems(itemTree);
			break;
			
		case 6:
			deleteItem(&itemTree);
			break;

		case 7:
			editItem(&itemTree);
			break;
			
		case 8:
			employeeMenu();
			break;

		case 9:
			return;
		default:
			printf("Invalid input, please try again\n");
			break;
		}
	}
	freeTree(itemTree);
}


void mainMenu()
{
	
	loadLog();
	employee* employees = NULL;
	int employee_size = loadEmployees(&employees);
	linked_list* customerList = loadCustomers();
	salesList* Sales = loadSales();
	while (1)
	{
		system("cls");
		int choice;
		printf("Welcome to Ori and Bar Electorincs Store Management System\n");
		printf("Please log in first:\n");
		int acessLevel = login(employees, employee_size);
		switch (acessLevel)
		{
		case 1:
			acessLevel1menu(acessLevel);
			break;
		case 2:
			acessLevel2menu(acessLevel);
			break;
		case 3:
			acessLevel3menu(acessLevel);
			break;
		}
	}
}


#endif

