#pragma once
#include "FileUtilsInterface.h"
#include <sys/file.h>
#include <stdio.h>

class LinuxFileUtils : public FileUtilsInterface
{
private:
int descriptor = -1;

public:
int Size() override;
void Close() override;
bool IsOpened() override;
fs::path GetName() override;
bool Open(fs::path p) override;
bool Seek(off_t offset) override;
bool Save(void* buffer, int size) override;
s32 Read(void* buf, size_t count) override;
~LinuxFileUtils();
};