#include "LinuxFileUtils.h"

bool LinuxFileUtils::Open(fs::path p)
{
    descriptor = open(p.c_str(), O_CREAT|O_WRONLY);

     
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

bool LinuxFileUtils::Save(void* buffer)
{
    // Linux Write uses a generic
    int isWrite = write(descriptor, buffer, sizeof(buffer));
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