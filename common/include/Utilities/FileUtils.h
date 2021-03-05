#include "FileUtilsInterface.h"

#ifdef __linux__
#include "LinuxFileUtils.h"
#elif _WIN32
#include "WindowsFileUtils.h"
#endif

class FileUtils
{
private:
    std::unique_ptr<FileUtilsInterface> fileStream;
    bool isInit = false;

public:

FileUtils();
FileUtils(fs::path);

int Size();
bool Close();
bool IsOpened();
fs::path GetName();
bool Open(fs::path p);
bool Seek(off_t offset);
s32 Read(void* buf, size_t count);
bool Write(void* buffer, int size);

~FileUtils();
};