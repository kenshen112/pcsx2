#include "LinuxFileUtils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

bool LinuxFileUtils::Open(fs::path p)
{
    currentPath = p;
    
    descriptor = open(currentPath.c_str(), O_CREAT|O_WRONLY);
 
    if (descriptor < 0)
    {
        Console.Error("File Open ERROR: %s", strerror(errno));
        return false;
    }
    else
    {
        int fileSize = lseek(descriptor, 0, SEEK_END);
        if (lockf(descriptor, F_LOCK, fileSize) < 0)
        {
            Console.Error("Lock ERROR: %s", strerror(errno));
            return false;
        }
        return true;
    }
    return descriptor;
}

bool LinuxFileUtils::Close()
{
   if (close(descriptor) < 0)
   {
       Console.Error("Close Error: %s", strerror(errno));
       return false;
   }
   return true;
}

fs::path LinuxFileUtils::GetName()
{
    return currentPath; 
}

bool LinuxFileUtils::Write(void* buffer, int &size)
{
    if (buffer != nullptr)
    {
        // Linux Write uses a generic
        if (write(descriptor, buffer, size) < 0)
        {
            Console.Error("Write Error: %s", strerror(errno));
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        Console.Error("Buffer Nullptr: ", strerror(errno));
        return false;
    }
}

bool LinuxFileUtils::IsOpened()
{
    return descriptor;
}

bool LinuxFileUtils::Seek(off_t offset)
{
    if (lseek(descriptor, offset, SEEK_SET) < 0)
    {
        Console.Error("Seek Error: ", strerror(errno));
        return false;
    }
    return true;
}

s32 LinuxFileUtils::Read(void* buf, size_t count)
{
    if (buf != nullptr)
    {
        data = read(descriptor, buf, count);
        if (data < 0)
        {
            Console.Error("Read Error: %s", strerror(errno));
            return -1; 
        }
            
        return data;
   }
   return -1;
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