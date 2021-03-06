// MailApp.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "Functions.h"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

const int MAX_FILE_LENGTH = 1024;
const int MAIL_FILE_DATA_SIZE = 3 * sizeof(int);
const char* MAIL_FILE_PATH = "C:\\MailApp\\mailbox.dat";
CString filePath;

using namespace std;
int main()
{
	TCHAR loginString[1000];
	cout << "Please enter your mail login. If there is no such mailbox - new one will be created.\n";
	fgetws(loginString, sizeof(loginString), stdin);
	loginString[wcslen(loginString) - 1] = _T('\0');
	filePath.Format(_T("C:\\MailApp\\%s.dat"), loginString);
	
	createMailFileDirectoryIfNeeded();
	/*
	char one[] = "ONE";
	addNewMessage(one);
	char two[] = "TWO";
	addNewMessage(two);
	char three[] = "THREE";
	addNewMessage(three);
	*/

	offerUserActions();
    return 0;
}

TCHAR* offerLoginAction() {
	TCHAR loginString[1000];
	cout << "Please enter your mail login. If there is no such mailbox - new one will be created.\n";
	fgetws(loginString, sizeof(loginString), stdin);
	
	return loginString;
}

void offerUserActions() {
	cout << "\nWhat would you like to do?\n";
	cout << "Write message - 1\n";
	cout << "Show message - 2\n";
	cout << "Delete message - 3\n";
	cout << "Exit - 4\n";
	int input = getSingleIntegerFromStdin();
	int toDelete;
	switch (input)
	{
	case 1:
		offerInputMessage();
		break;
	case 2:
		cout << "You have " << getMessagesCount() << " messages. " << "What message would you like to see? (Use 0 for all):\n";
		printMessage(getSingleIntegerFromStdin());
		break;
	case 3:
		cout << "Here are your messages:\n\n";
		printMessage(NULL);
		cout << "Enter the message number to delete:\n";
		toDelete = getSingleIntegerFromStdin();
		deleteMessage(toDelete);
		break;
	case 4:
		return;
		break;
	default:
		break;
	}
	offerUserActions();
}

void offerInputMessage() {
	cout << "\nPlease enter the message:\n";
	char message[256];
	fgets(message,256, stdin);
	addNewMessage(message);
}

void createMailFileDirectoryIfNeeded() {
	
	BOOL success = CreateDirectory(_T("C:\\MailApp"), NULL);
	if (success == 0) {
		printf("Failed fo create directory.\n");
		if (GetLastError() == ERROR_ALREADY_EXISTS)  {
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
	HANDLE handle = CreateFile(filePath,
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
	HANDLE handle = CreateFile(filePath,
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

//TODO check if the message fits.
void addNewMessage(char* message) {
	HANDLE  mailboxFile = openMailFile(1);
	int mailData[3];
	int lastMailPosition = 0;
	int messageLength = strlen(message) * sizeof(char);
	if (mailboxFile != NULL) {
		getMailData(mailboxFile, mailData);
		printf("Writing updated mail data.\n");
		lastMailPosition = mailData[1];
		mailData[0] = mailData[0] + 1;
		mailData[1] = mailData[1] + messageLength + sizeof(int);
		SetFilePointer(mailboxFile, 0, 0, FILE_BEGIN);

		WriteFile(mailboxFile, mailData, sizeof(mailData), NULL, NULL);
		SetFilePointer(mailboxFile, lastMailPosition, 0, FILE_BEGIN);
		int messageSize[1] = { messageLength };
		WriteFile(mailboxFile, messageSize, sizeof(messageSize), NULL, NULL);
		WriteFile(mailboxFile, message, messageLength, NULL, NULL);
		CloseHandle(mailboxFile);
	}
}

/*
If the message number is less than one - all messages will be printed. 
CAN BE NULL.
*/
void printMessage(int messageNumber) {
	int messagesCount = getMessagesCount();
	HANDLE  mailboxFile = openMailFile(1);
	SetFilePointer(mailboxFile, MAIL_FILE_DATA_SIZE, 0, FILE_BEGIN);
	if (messageNumber > messagesCount) {
		cout << "You have only " << messagesCount << " messages.\n";
	}
	for (int i = 1; i <= messagesCount; i++) {
		int messageLength[1];
		ReadFile(mailboxFile, messageLength, sizeof(messageLength), NULL, NULL);
		int length = messageLength[0] * sizeof(char);
		char* message = (char*) malloc(length + sizeof(char));
		ReadFile(mailboxFile, message, length, NULL, NULL);
		message[length] = '\0';
		if (messageNumber > 0 ) {
			if (i == messageNumber) {
				cout << "Message # " << i << ": " << message << "\n";
				break;
			}
		}
		else {
			cout << "Message # " << i << ": " << message << "\n";
			//printf(ANSI_COLOR_BLUE "Message # %d: %s" ANSI_COLOR_RESET, i, message);
		}
		free(message);
	}
	CloseHandle(mailboxFile);
}

void deleteMessage(int messageNumber) {
	HANDLE  mailboxFile = openMailFile(1);
	int mailData[3];
	getMailData(mailboxFile, mailData);
	int messagesCount = mailData[0];
	int deleteStartPos = MAIL_FILE_DATA_SIZE;
	int deleteEndPos = 0;
	SetFilePointer(mailboxFile, MAIL_FILE_DATA_SIZE, 0, FILE_BEGIN);
	if (messageNumber > 0 && messageNumber <= messagesCount) {
		for (int i = 1; i <= messageNumber; i++) {
			int messageLength[1];
			ReadFile(mailboxFile, messageLength, sizeof(messageLength), NULL, NULL);
			int length = messageLength[0] * sizeof(char);
			if (i == messageNumber) {
				deleteEndPos = deleteStartPos  + sizeof(int) + length;
			}
			else {
				deleteStartPos += sizeof(int) + length;
			}
			SetFilePointer(mailboxFile, deleteStartPos, NULL, FILE_BEGIN);
		}
		SetFilePointer(mailboxFile, deleteEndPos, NULL, FILE_BEGIN);
		int remainLength = mailData[1] - deleteEndPos;
		byte *data = (byte*) malloc(remainLength);
		ReadFile(mailboxFile, data, remainLength, NULL, NULL);
		SetFilePointer(mailboxFile, deleteStartPos, NULL, FILE_BEGIN);
		WriteFile(mailboxFile, data, remainLength, NULL, NULL);
		int newLength = mailData[1] - (deleteEndPos - deleteStartPos);
		mailData[1] = newLength;
		mailData[0] = mailData[0] - 1;
		SetFilePointer(mailboxFile, 0, NULL, FILE_BEGIN);
		WriteFile(mailboxFile, mailData, sizeof(mailData), NULL, NULL);
		SetFilePointer(mailboxFile, newLength, NULL, FILE_BEGIN);
		SetEndOfFile(mailboxFile);
		SetFilePointer(mailboxFile, 0, NULL, FILE_BEGIN);
	}
	else {
		cout << "There is no message at " << messageNumber;
	}
}

void getMailData(HANDLE mailboxFile, int * output) {
	SetFilePointer(mailboxFile, 0, 0, FILE_BEGIN);
	ReadFile(mailboxFile, output, MAIL_FILE_DATA_SIZE,
		NULL,
		NULL);

	if (output[2] == MAX_FILE_LENGTH) {
		//printf("Sucessfull mail data read.\n");
	}
	else {
		printf("Failed reading mail data.\n");
	}

}

int getMessagesCount() {
	HANDLE  mailboxFile = openMailFile(1);
	int mailData[3];
	getMailData(mailboxFile, mailData);
	CloseHandle(mailboxFile);
	return mailData[0];
}

int getSingleIntegerFromStdin() {
	fseek(stdin, 0, SEEK_END);
	char input[sizeof(int)];
	fgets(input, sizeof(int), stdin);
	fseek(stdin, 0, SEEK_END);
	return atoi(&input[0]);
}



