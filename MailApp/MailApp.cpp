// MailApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Functions.h"
#include <Windows.h>
#pragma warning(disable : 4996)


int main()
{
	createMailFileDirectoryIfNeeded();
    return 0;
}

void createMailFileDirectoryIfNeeded() {
	BOOL success = CreateDirectory(_T("C:\\MailApp"), NULL);
	if (success == 0) {
		printf("Failed fo create directory.\n");
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
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
	FILE *pointer = NULL;
	pointer = fopen("C:\\MailApp\\mailbox.dat", "wb");
	if (pointer == NULL) {
		printf("Failed to open mail file.\n");
	}
	int data[] = {0, sizeof(int) * 3, MAX_PATH};
	fwrite(data, sizeof(data), 1, pointer);
	printf("Sucessfully wrote the initial data to mail file.\n");
	fclose(pointer);
}



