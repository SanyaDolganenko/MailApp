// MailApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Functions.h"
#include <Windows.h>
#include <string.h>
#pragma warning(disable : 4996)

const int MAX_FILE_LENGTH = 1024;
const char* MAIL_FILE_PATH = "C:\\MailApp\\mailbox.dat";

int main()
{
	createMailFileDirectoryIfNeeded();
	char message[] = "";
	addNewMessage(message);
    return 0;
}

void createMailFileDirectoryIfNeeded() {
	BOOL success = CreateDirectory(_T("C:\\MailApp"), NULL);
	if (success == 0) {
		printf("Failed fo create directory.\n");
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			SetLastError(0);
			printf("Directory already exists.\n");
			createMailFile();
		}
	}
	else {
		printf("Sucessfully created the mail file DIRECTORY.\n");
		createMailFile();
	}
}

void createMailFile() {
	SetLastError(0);
	HANDLE handle = CreateFile(_T("C:\\MailApp\\mailbox.dat"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (handle == NULL || GetLastError() == ERROR_FILE_EXISTS) {
		printf("Failed creating new mail file.\n");
		if (GetLastError() == ERROR_FILE_EXISTS) {
			SetLastError(0);
			printf("The mail file already exists.\n");
		}
		CloseHandle(handle);
	}
	else {
		printf("Sucessfully created the mail file.\n");
		CloseHandle(handle);
		//Init the default data.
		uploadDefaultFileData();
	}
}

void uploadDefaultFileData() {
	HANDLE pointer = openMailFile(0);
	if (pointer != NULL) {
		int data[] = { 0, sizeof(int) * 3, MAX_FILE_LENGTH };
		//fwrite(data, sizeof(data), 1, pointer);
		printf("Sucessfully wrote the initial data to mail file.\n");
		//fclose(pointer);
	}
}

HANDLE openMailFile(int read) {
	HANDLE pointer = NULL;
	//pointer = fopen(MAIL_FILE_PATH, read ?"rb" : "wb");
	
	if (pointer == NULL) {
		printf("Failed to open mail file.\n");
	}
	return pointer;
}


void addNewMessage(char* message) {
	/*
	FILE *readingFile = openMailFile(1);
	int mailData[3];
	if (readingFile != NULL) {
		fread(mailData, sizeof(mailData), 1, readingFile);
		mailData[0] = mailData[0] + 1;
		mailData[1] = mailData[1] + strlen(message) * sizeof(char);
		fclose(readingFile);
		if (mailData[2] == MAX_FILE_LENGTH) {
			printf("Sucessfull mail data read.\n");
		}
	}
	FILE * writingFile = openMailFile(0);
	if (writingFile != NULL) {
		printf("Writing updated mail data.\n");
		SetFilePointer(writingFile, 0, 0, FILE_BEGIN);
		fwrite(mailData, sizeof(mailData), 1, readingFile);
	}
	*/
}



