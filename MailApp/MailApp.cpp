// MailApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Functions.h"
#include <Windows.h>



int main()
{
	createDefaultMailFile();
    return 0;
}

void createDefaultMailFile() {
	BOOL success = CreateDirectory(_T("C:\\MailApp"), NULL);
	if (success == 0) {
		printf("Failed fo create directory.");
	}
	else {

	}
}



