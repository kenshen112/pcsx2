#include "FileUtils.h"


FileUtils::FileUtils(fs::path p)
{
    #ifdef __linux__
    fileStream = new LinuxFileUtils();
    #elif _WIN32
    fileStream = new WindowsFileUtils();
    #endif

    if (fileStream != nullptr)
    {
        if (!fileStream->Open(p))
        {
            fileStream = nullptr;
            return;
        }
    }
}

bool FileUtils::Save(void* buffer)
{
    return fileStream->Save(buffer); 
}
