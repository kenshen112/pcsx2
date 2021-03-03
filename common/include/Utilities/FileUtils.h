#include "FileUtilsInterface.h"

#ifdef __linux__
#include "LinuxFileUtils.h"
#elif _WIN32
#include "WindowsFileUtils.h"
#endif

class FileUtils
{
private:
    FileUtilsInterface *fileStream;
    bool isInit = false;

public:

FileUtils();
FileUtils(fs::path);

int Size();
void Close();
bool IsOpened();
fs::path GetName();
bool Open(fs::path p);
bool Seek(off_t offset);
bool Save(void* buffer, int size);
s32 Read(void* buf, size_t count);

~FileUtils();
};