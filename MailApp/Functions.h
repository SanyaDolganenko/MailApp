#pragma once
#include "stdafx.h"

void createMailFileDirectoryIfNeeded();

void uploadDefaultFileData();

void createMailFile();

HANDLE openMailFile(int read);

void addNewMessage(char* message);

void offerUserActions();

void offerInputMessage();