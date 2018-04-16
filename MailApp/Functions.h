#pragma once
#include "stdafx.h"

void createMailFileDirectoryIfNeeded(char * login);

void uploadDefaultFileData();

void createMailFile(char * login);

HANDLE openMailFile(int read);

void addNewMessage(char* message);

void offerUserActions();

void offerInputMessage();

void printMessage(int messageNumber);

void deleteMessage(int messageNumber);

void getMailData(HANDLE file, int *output);

int getMessagesCount();

int getSingleIntegerFromStdin();

char* offerLoginAction();