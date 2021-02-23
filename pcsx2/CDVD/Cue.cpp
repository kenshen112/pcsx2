#include "Cue.h"


bool Cue::open(fs::path trackPath)
{
    FILE* cueFile = fopen(trackPath.c_str(), "r");
    cueTrackInfo = cue_parse_file(cueFile);
}