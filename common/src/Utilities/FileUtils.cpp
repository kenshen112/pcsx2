#include "FileUtils.h"


FileUtils::FileUtils()
{
    fileStream = nullptr;
}

FileUtils::FileUtils(fs::path p)
{
    #ifdef __linux__
    fileStream = std::make_unique<LinuxFileUtils>();
    #elif _WIN32
    fileStream = std::make_unique<WindowsFileUtils>();
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

fs::path FileUtils::GetName()
{
    return fileStream->GetName();
}

bool FileUtils::Open(fs::path p)
{
    #ifdef __linux__
    fileStream = std::make_unique<LinuxFileUtils>();
    #elif _WIN32
    fileStream = std::make_unique<WindowsFileUtils>();
    #endif

    if (fileStream != nullptr)
    {
        if (fileStream->Open(p) != true)
        {
            fileStream = nullptr;
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

int FileUtils::Size()
{
    return fileStream->Size();
}

bool FileUtils::Write(void* buffer, int size)
{
    return fileStream->Write(buffer, size); 
}

bool FileUtils::Seek(off_t offset)
{
    return fileStream->Seek(offset);
}

bool FileUtils::Close()
{
    return fileStream->Close();
}

s32 FileUtils::Read(void* buff, size_t size)
{
   return fileStream->Read(buff, size);
}

bool FileUtils::IsOpened()
{
    return fileStream->IsOpened();
}

FileUtils::~FileUtils()
{
}