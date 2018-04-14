#pragma once
void createMailFileDirectoryIfNeeded();

void uploadDefaultFileData();

void createMailFile();

FILE* openMailFile(int read);

void addNewMessage(char* message);