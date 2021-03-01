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

bool Save(void* buffer); 
void Close();
~FileUtils();
};