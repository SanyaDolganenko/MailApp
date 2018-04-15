#pragma once
void createMailFileDirectoryIfNeeded();

void uploadDefaultFileData();

void createMailFile();

HANDLE openMailFile(int read);

void addNewMessage(char* message);