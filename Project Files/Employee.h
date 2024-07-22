

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Logs.h"

#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_
gets();
typedef struct employee {
	char username[32];
	char password[32];
	int accessLevel;
	char fullName[32];
}employee;

int loadEmployees(employee** employees) {
	FILE* fp = fopen("employees.bin", "rb");
	if (fp == NULL) {
		printf("Create new file employees\n");
		// File doesn't exist, create it
		fp = fopen("employees.bin", "wb");
		if (fp == NULL) {
			printf("Error creating file\n");
			return 0;
		}
	}
	else {
		printf("Loading existing employees file\n");
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	*employees = (employee*)malloc(size);
	if (*employees == NULL) {
		printf("Error allocating memory\n");
		return 0;
	}
	int read = fread(*employees, 1, size, fp);
	if (read != size) {
		printf("Error reading file\n");
		return 0;
	}
	fclose(fp);
	return size / sizeof(employee);
}

void saveEmployees(employee* employees, int employees_size) {
	FILE* fp = fopen("employees.bin", "wb");
	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}
	fwrite(employees, sizeof(employee), employees_size, fp);
	fclose(fp);
}

employee* searchEmployeeByUsername(employee* employees, int employees_size, char* username) {
	for (int i = 0; i < employees_size; i++) {
		if (strcmp(employees[i].username, username) == 0) {
			return &employees[i];
		}
	}
	
	return NULL;
}

void addEmployee(employee** employees, int* employees_size) {
	if (*employees_size == 0) {
		*employees = (employee*)malloc(sizeof(employee));
		if (*employees == NULL) {
			printf("Error allocating memory\n");
			return;
		}
	}
	else {
		*employees = (employee*)realloc(*employees, (*employees_size + 1) * sizeof(employee));
		if (*employees == NULL) {
			printf("Error allocating memory\n");
			return;
		}
	}
	char username[32];
	while (true) {
		printf("Enter username: ");
		scanf("%s", username);
		if (searchEmployeeByUsername(*employees, *employees_size, username) == NULL) {
			break;
		}
		printf("Username already exists\n");
	}
	strcpy((*employees)[*employees_size].username, username);
	printf("Enter the new employee password: ");
	scanf("%s", (*employees)[*employees_size].password);
	printf("Enter the new employee access level: ");
	//if the access level isn't int between 1-3 ask again
	while (true) {
		scanf("%d", &(*employees)[*employees_size].accessLevel);
		if ((*employees)[*employees_size].accessLevel >= 1 && (*employees)[*employees_size].accessLevel <= 3) {
			break;
		}
		printf("Enter the new employee access level: ");
	}
	printf("Enter the new employee full name: ");
	gets((*employees)[*employees_size].fullName);
	gets((*employees)[*employees_size].fullName);
	

	*employees_size += 1;	
	saveEmployees(*employees, *employees_size);
	
	char log[150];
	sprintf(log, "New employee added: %s", (*employees)[*employees_size - 1].username);
	logMessage(log);
}


void deleteEmployee(employee** employees, int* employees_size) {
	char username[32];
	printf("Enter the username of the employee to delete: ");
	scanf("%s", username);
	employee* employee_temp = searchEmployeeByUsername(*employees, *employees_size, username);

	if (employee_temp == NULL) {
		printf("Employee not found\n");
		return;
	}
	else {
		if (strcmp(employee_temp->username, "admin") == 0) {
			printf("You can't delete admin\n");
			return;
		}
		int index = employee_temp - *employees;
		for (int i = index; i < *employees_size - 1; i++) {
			(*employees)[i] = (*employees)[i + 1];
		}
		*employees_size -= 1;
		*employees = (employee*)realloc(*employees, *employees_size * sizeof(employee));
		if (*employees == NULL) {
			printf("Error allocating memory\n");
			return;
		}
		saveEmployees(*employees, *employees_size);
		char log[150];
		sprintf(log, "Employee deleted: %s", username);
		logMessage(log);
	}
}

void printEmployeeByUsername(employee* employees, int employees_size) {
	char username[32];
	printf("Enter the username of the employee you want to print: ");
	scanf("%s", username);
	employee* isExist = searchEmployeeByUsername(employees, employees_size, username);
	if (isExist == NULL) {
		printf("Employee doesn't exist\n");
		return;
	}
	else
	{
		printf("Username: %s\n", isExist->username);
		printf("Password: %s\n", isExist->password);
		printf("Access level: %d\n", isExist->accessLevel);
		printf("Full name: %s\n", isExist->fullName);
		printf("--------------------------------------------------\n");
	}
}

void editEmployee(employee** employees, int* employees_size) {
	char username[32];
	printf("Enter the username of the employee you want to edit: ");
	scanf("%s", username);

	employee* isExist = searchEmployeeByUsername(*employees, *employees_size, username);
	if (isExist == NULL) {
		printf("Employee doesn't exist\n");
		return;
	}
	else
	{	
		int choice;
		do {
			printf("1. Edit username\n");
			printf("2. Edit password\n");
			printf("3. Edit access level\n");
			printf("4. Edit full name\n");
			printf("5. Exit\n");
			printf("Enter your choice: ");
			scanf("%d", &choice);
			switch (choice)
			{
			case 1:
				while (true) {
					printf("Enter new username: ");
					scanf("%s", username);
					if (searchEmployeeByUsername(*employees, *employees_size, username) == NULL) {
						break;
					}
					printf("Username already exists\n");
				}
				strcpy(isExist->username, username);
				break;
			case 2:
				printf("Enter new password: ");
				scanf("%s", isExist->password);
				break;
			case 3:
				printf("Enter new access level: ");
				while (true) {
					scanf("%d", &isExist->accessLevel);
					if (isExist->accessLevel >= 1 && isExist->accessLevel <= 3) {
						break;
					}
					printf("Enter new access level: ");
				}
				break;
			case 4:
				printf("Enter new full name: ");
				gets(isExist->fullName);
				break;
			case 5:
				break;
			default:
				printf("Invalid choice\n");
				break;
			}
		} while (choice != 5);

		printf("Username: %s\n", isExist->username);
		printf("Password: %s\n", isExist->password);
		printf("Access level: %d\n", isExist->accessLevel);
		printf("Full name: %s\n", isExist->fullName);
		printf("--------------------------------------------------\n");
		
		saveEmployees(*employees, *employees_size);
		
		char log[150];
		sprintf(log, "Employee edited: %s", username);
		logMessage(log);
	}
	free(*employees);
}

void printAllEmployees(employee* employees, int employees_size) {
	for (int i = 0; i < employees_size; i++) {
		printf("Username: %s\n", employees[i].username);
		printf("Password: %s\n", employees[i].password);
		printf("Access level: %d\n", employees[i].accessLevel);
		printf("Full name: %s\n", employees[i].fullName);
		printf("--------------------------------------------------\n");
	}

}



void employeeMenu()
{
	employee* employees = NULL;
	int employees_size = loadEmployees(&employees);
	int choice;
	system("cls");
	while (1)
	{
		printf("1. Add employee\n");
		printf("2. Delete employee\n");
		printf("3. Edit employee\n");
		printf("4. Search employee by his username\n");
		printf("5. Display all employees\n");
		printf("6. Save and exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			addEmployee(&employees, &employees_size);
			break;
		case 2:
			deleteEmployee(&employees, &employees_size);
			break;
		case 3:
			editEmployee(&employees, &employees_size);
			break;
		case 4:
			printEmployeeByUsername(employees, employees_size);
			break;
		case 5:
			printAllEmployees(employees, employees_size);
			break;
		case 6:
			return;
		default:
			printf("Invalid choice\n");
			break;
		}
	}
	free(employees);
}


int login(employee* employees, int employees_size)
{

	if (employees_size == 0) {
		employee* newEmployee = (employee*)malloc(sizeof(employee));
		strcpy(newEmployee->username, "admin");
		strcpy(newEmployee->password, "admin");
		newEmployee->accessLevel = 3;
		strcpy(newEmployee->fullName, "Manager_System");
		employees_size++;
		employees = (employee*)realloc(employees, employees_size * sizeof(employee));
		employees[employees_size - 1] = *newEmployee;
		saveEmployees(employees, employees_size);
	}

	int counter = 0;

	char username[32];
	char password[32];

	printf("Enter username: ");
	scanf("%s", username);
	printf("Enter password: ");
	scanf("%s", password);
	
	while (true)
	{
		employee* isExist = searchEmployeeByUsername(employees, employees_size, username);
		if (isExist != NULL && strcmp(isExist->password, password) == 0)
		{
			return isExist->accessLevel;
		}
		
		else
		{
			printf("Wrong username or password\n");
			printf("Enter username: ");
			scanf("%s", username);
			printf("Enter password: ");
			scanf("%s", password);
			counter++;
			if (counter == 3)
			{
				printf("\n\nTry again later\n\n");
				exit(0);
			}
		}
	}
}





#endif

