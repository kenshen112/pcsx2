#include "PrecompiledHeader.h"
#include "WindowsFileUtils.h"
#include <Windows.h>
#include <fileapi.h>

bool WindowsFileUtils::Open(fs::path p)
{
	fileHandle = CreateFile(LPCWSTR(p.c_str()), FILE_ALL_ACCESS, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, fileHandle);

	if (fileHandle)
	{
		// Do File Handle work in here
		return true;
	}
}