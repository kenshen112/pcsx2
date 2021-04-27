#include "PrecompiledHeader.h"
#include "Global.h"
#include "SndOut.h"
#if defined(__linux__) || defined(__APPLE__)
#include "Linux/Dialogs.h"
#elif defined(_WIN32)
#include "Windows/Dialogs.h"
#endif
#include "cubeb/cubeb.h"

long DataCallback(cubeb_stream * stm, void * user, const void * input_buffer, void * output_buffer, long nframes)
{
    StereoOut16* out = (StereoOut16*)output_buffer;
    //Console.Warning("Packet Size: %02d", nframes);
    SndBuffer::ReadSamples(out, nframes);

    return nframes;
}
void StateCallback(cubeb_stream * stm, void * user, cubeb_state state)
{
    printf("state=%d\n", state);
}

class SndCubeb : public SndOutModule
{
    //////////////////////////////////////////////////////////////////////////////////////////
    // Instance vars
    private:

    int rv;
    int volume = 50;
    u32 channels;
    u32 rate;
    u32 latency_frames;

    std::string m_api;
    bool started;

    cubeb* api;
    cubeb_stream* stream;
    cubeb_stream_params outParams;
    //////////////////////////////////////////////////////////////////////////////////////////

    public:

    static SndCubeb cuOut;

    s32 Init() override
    {
        ReadSettings(); // To select Cubeb API

        if (cubeb_init(&api, "PCSX2", m_api.c_str()) != CUBEB_OK) // The inital startup of the underlying cubeb struct
        {
            DevCon.Error("INIT FAILED!");
            return CUBEB_ERROR;
        }
        if (cubeb_get_preferred_sample_rate(api, &rate) != CUBEB_OK)
        {
            DevCon.Error("Could not get preferred sample-rate");
            return CUBEB_ERROR;
        }

        started = true; 

        outParams.rate = 48000; // ToDo, change for psx
        outParams.channels = 2;// = channels; for when we add dolby digital
        outParams.format = CUBEB_SAMPLE_S16LE;
        outParams.layout = CUBEB_LAYOUT_STEREO;

        if (cubeb_get_min_latency(api, &outParams, &latency_frames) != CUBEB_OK)
        {
            DevCon.Error("LATENCY NOT DETECTED!");
            return CUBEB_ERROR;
        }

        if(cubeb_stream_init(api, &stream, "PCSX2", nullptr, nullptr, nullptr, &outParams, latency_frames, DataCallback, StateCallback, nullptr) != CUBEB_OK)
        {
            DevCon.Error("FAILED STREAM");
            return CUBEB_ERROR;
        }

        cubeb_stream_set_volume(stream, volume);
        cubeb_stream_start(stream);
        return CUBEB_OK;
    }

    s32 Test() const override { return 0; }

    int GetEmptySampleCount() override { return 0; }

    void Configure(uptr parent) override {}

    void WriteSettings() const override
    {
        // TODO USE TOWXSTRING
        CfgWriteStr(L"CUBEB", L"HostApi", wxString(m_api.c_str(), wxConvUTF8));
    }

    void ReadSettings() override
    {
        wxString api(L"EMPTYEMPTYEMPTY");
        CfgReadStr(L"CUBEB", L"HostApi", api, L"pulse");
        CfgWriteInt(L"MIXING", L"FinalVolume", volume);
        SetApiSettings(api);
    }

    void SetApiSettings(wxString api) override
    {
        // ToDo change to, FromWxString
        m_api = std::string(api.utf8_str());
    }

    const wchar_t* GetIdent() const override
    {
        return L"Cubeb";
    }

    const wchar_t* GetLongName() const override
    {
        return L"Cubeb (Cross-platform)";
    }

    void Close() override
    {
        cubeb_stream_stop(stream);
        cubeb_stream_destroy(stream);
        cubeb_destroy(api);
    }
};

SndCubeb SndCubeb::cuOut;

SndOutModule* CubebOut = &SndCubeb::cuOut;