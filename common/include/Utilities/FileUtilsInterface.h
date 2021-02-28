#include <iostream>
#include <PathUtils.h>

class FileUtilsInterface
{
protected:
	fs::path currentFilePath;
public:
	virtual bool Open(fs::path p) = 0;
	virtual void Close() = 0;
	virtual bool ApplyLock() = 0;
	virtual void ReleaseLock() = 0;
};