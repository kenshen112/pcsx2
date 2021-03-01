#pragma once

#include <iostream>
#include <PathUtils.h>

// This is supposed to be an OS Specific overide to WxFile
class FileUtilsInterface
{
public:
	virtual bool Open(fs::path p) = 0;
	virtual void Close() = 0;
	virtual bool Save(void* buffer) = 0;
};