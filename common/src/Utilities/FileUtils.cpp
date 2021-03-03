#include "FileUtils.h"


FileUtils::FileUtils()
{
    fileStream = nullptr;
}

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

fs::path FileUtils::GetName()
{
    return fileStream->GetName();
}

bool FileUtils::Open(fs::path p)
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
            return false;
        }
    }
    else
    {
        return true;
    }
}

int FileUtils::Size()
{
    return fileStream->Size();
}

bool FileUtils::Save(void* buffer, int size)
{
    return fileStream->Save(buffer, size); 
}

bool FileUtils::Seek(off_t offset)
{
    fileStream->Seek(offset);
}

void FileUtils::Close()
{
    fileStream->Close();
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
    //delete fileStream; // This calls destructor
}