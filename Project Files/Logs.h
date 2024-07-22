

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifndef _LOGS_H_
#define _LOGS_H_

void loadLog() {
	FILE* fp = fopen("logs.txt", "r");
	if (fp == NULL) {
		printf("Create new file logs\n");

		// File doesn't exist, create it
		fp = fopen("logs.txt", "w");
		if (fp == NULL) {
			printf("Error creating file\n");
			return;
		}
	}
	else {
		printf("Loading existing logs file\n");
	}
}

void logMessage(char* log) {
	// this part of the func it is for the date part.
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char time[32];
	strftime(time, sizeof(time), "%c - ", &tm);
	char logAndTime[150];
	sprintf(logAndTime, "%s %s", time, log);
	
	// this part of the func it is for the writing part.
	FILE* fp = fopen("logs.txt", "a");
	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}
	fprintf(fp, "%s\n", logAndTime);
	printf("%s\n", logAndTime);
	fclose(fp);
}



#endif