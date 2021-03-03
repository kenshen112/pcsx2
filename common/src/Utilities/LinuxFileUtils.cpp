#include "LinuxFileUtils.h"

bool LinuxFileUtils::Open(fs::path p)
{
    currentPath = p;
    
    descriptor = open(currentPath.c_str(), O_CREAT|O_WRONLY);
 
    if (descriptor < 0)
    {
        Console.Error("File Open ERROR: " + errno);
        return false;
    }
    else
    {
        uint fileSize = lseek(descriptor, 0, SEEK_END);
        if (lockf(descriptor, LOCK_EX|LOCK_NB, fileSize) < 0)
        {
            Console.Error("Lock ERROR: " + errno);
            return false;
        }
        return true;
    }
}

void LinuxFileUtils::Close()
{
   int state = close(descriptor);
}

fs::path LinuxFileUtils::GetName()
{
    return currentPath; 
}

bool LinuxFileUtils::Save(void* buffer, int size)
{
    // Linux Write uses a generic
    int isWrite = write(descriptor, buffer, size);
    if (isWrite < 0)
    {
        Console.Error("Write Error: " + errno);
        return false;
    }
    else
    {
        return true;
    }
}

bool LinuxFileUtils::IsOpened()
{
    return descriptor;
}

bool LinuxFileUtils::Seek(off_t offset)
{
    lseek(descriptor, offset, SEEK_CUR);
}

s32 LinuxFileUtils::Read(void* buf, size_t count)
{
    return read(descriptor, buf, count);
}

int LinuxFileUtils::Size()
{
    int size = lseek(descriptor, 0, SEEK_END);

    if (size >= 0)
    {
        return size;
    }
    else
    {
        Console.Error("Size incorrect");
        return -1;
    }
}

LinuxFileUtils::~LinuxFileUtils()
{
    
}