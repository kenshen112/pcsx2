#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "PathUtils.h"
#include "libcue.h"

struct trackInfo
{
        fs::path currentTrack;
        u32 filePosition;
};

class Cue
{
private:
int trackCount;
std::vector<trackInfo> tracks;

Cd *cueTrackInfo;

public:
bool open(fs::path trackPath);


};