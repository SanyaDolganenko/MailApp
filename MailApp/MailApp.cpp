// MailApp.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "Functions.h"

const int MAX_FILE_LENGTH = 1024;
const char* MAIL_FILE_PATH = "C:\\MailApp\\mailbox.dat";

int main()
{
	createMailFileDirectoryIfNeeded();
	char message[] = "Hello world";
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
		BOOL writeSuccess = 
		WriteFile(pointer, data, sizeof(data), NULL, NULL);
		if (writeSuccess) {
			printf("Sucessfully wrote the initial data to mail file.\n");
		}
		else {
			printf("Failed writing the initial data to mail file.\n");
		}
		CloseHandle(pointer);
	}
}

HANDLE openMailFile(int read) {
	HANDLE handle = CreateFile(_T("C:\\MailApp\\mailbox.dat"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (handle == NULL) {
		printf("Failed to open mail file.\n");
	}
	return handle;
}


void addNewMessage(char* message) {
	
	HANDLE  readingFile = openMailFile(1);
	int mailData[3];
	int lastMailPosition = 0;
	int messageLength = strlen(message) * sizeof(char);
	if (readingFile != NULL) {
		SetFilePointer(readingFile, 0, 0, FILE_BEGIN);
		ReadFile(readingFile, mailData, sizeof(mailData),
			NULL,
			NULL);
		lastMailPosition = mailData[1];
		mailData[0] = mailData[0] + 1;
		mailData[1] = mailData[1] + messageLength + sizeof(int);
		
		if (mailData[2] == MAX_FILE_LENGTH) {
			printf("Sucessfull mail data read.\n");
		}
	}
	
		printf("Writing updated mail data.\n");
		SetFilePointer(readingFile, 0, 0, FILE_BEGIN);
		
		WriteFile(readingFile, mailData, sizeof(mailData),NULL, NULL);
		SetFilePointer(readingFile,lastMailPosition, 0, FILE_BEGIN);
		int messageSize[1] = { messageLength };
		WriteFile(readingFile, messageSize, sizeof(messageSize), NULL, NULL);
		WriteFile(readingFile, message, messageLength, NULL, NULL);
		CloseHandle(readingFile);
}



