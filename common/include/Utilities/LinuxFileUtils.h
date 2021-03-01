#pragma once
#include "FileUtilsInterface.h"
#include <sys/file.h>
#include <stdio.h>

class LinuxFileUtils : public FileUtilsInterface
{
private:
int descriptor = -1;

public:
bool Open(fs::path p);
void Close();
bool Save(void* buffer);

};