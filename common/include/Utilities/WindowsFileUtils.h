#pragma once
#include "FileUtilsInterface.h"

#include <windows.h>
#include <stdio.h>

class WindowsFileUtils : FileUtilsInterface
{
private:
	HANDLE fileHandle;
public:
	bool Open(fs::path p);
};