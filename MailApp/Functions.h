#pragma once
#include "stdafx.h"

void createMailFileDirectoryIfNeeded();

void uploadDefaultFileData();

void createMailFile();

HANDLE openMailFile();

void addNewMessage(char* message);

void offerUserActions();

void offerInputMessage();

void printMessage(int messageNumber);

void deleteMessage(int messageNumber);

void getMailData(HANDLE file, int *output);

int getMessagesCount();

int getSingleIntegerFromStdin();

TCHAR* offerLoginAction();

int getControlSum(HANDLE file);

void updateMailData();