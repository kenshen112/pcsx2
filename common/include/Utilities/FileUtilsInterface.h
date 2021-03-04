#pragma once

#include <iostream>
#include <PathUtils.h>

// This is supposed to be an OS Specific overide to WxFile
class FileUtilsInterface
{
protected:
fs::path currentPath;
public:
	virtual bool Open(fs::path p) = 0;
	virtual bool Write(void* buffer, int &size) = 0;
	virtual bool Seek(off_t offset) = 0;
	virtual bool IsOpened() = 0;
	virtual bool Close() = 0;
	virtual s32 Read(void* buf, size_t count) = 0;
	virtual int Size() = 0;
	virtual fs::path GetName() = 0;

	virtual ~FileUtilsInterface(){};
};