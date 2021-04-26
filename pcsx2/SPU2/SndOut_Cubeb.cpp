#include "cubeb/cubeb.h"
#include "PrecompiledHeader.h"
#include "Global.h"
#include "SndOut.h"

long DataCallback(cubeb_stream * stm, void * user, const void * input_buffer, void * output_buffer, long nframes)
{
    StereoOutFloat* in  = new StereoOutFloat[SndOutPacketSize];
    StereoOutFloat* out = (StereoOutFloat*)output_buffer;

    for (u16 i = 0; i < nframes; i += SndOutPacketSize)
    {
        //SndBuffer::ReadSamples(in);
        for (int c = 0; c < 2; ++c) 
        {
            out[2 * i + c] = in[i];
        }

    }
    delete in;
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
    u64 ts;

    bool started;

    cubeb* api;
    cubeb_stream* stream;
    cubeb_stream_params outParams;
    //////////////////////////////////////////////////////////////////////////////////////////

    public:

    static SndCubeb cuOut;

    s32 Init() override
    {
        std::cout << "CUBEB AUDIO" << std::endl;

        if (cubeb_init(&api, "PCSX2", NULL) != CUBEB_OK) // The inital startup of the underlying cubeb struct
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

        outParams.rate = rate;
        outParams.channels = 2;
        outParams.format = CUBEB_SAMPLE_S16NE;
        outParams.layout = CUBEB_LAYOUT_STEREO;

        if (cubeb_get_min_latency(api, &outParams, &latency_frames) != CUBEB_OK)
        {
            DevCon.Error("LATENCY NOT DETECTED!");
            return CUBEB_ERROR;

        }

        cubeb_stream_set_volume(stream, volume);
        if(cubeb_stream_init(api, &stream, "PCSX2 Audio", nullptr, nullptr, nullptr, &outParams, latency_frames, DataCallback, StateCallback, this) != CUBEB_OK)
        {
            DevCon.Error("FAILED STREAM");
            return CUBEB_ERROR;
        }
        cubeb_stream_start(stream);
        return CUBEB_OK;
    }

    s32 Test() const override { return 0; }

    int GetEmptySampleCount() override { return 0; }

    void Configure(uptr parent) override {}

    void WriteSettings() const override
    {
    }

    void ReadSettings() override
    {
    }

    void SetApiSettings(wxString api) override
    {
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
        //delete api;
    }
};

SndCubeb SndCubeb::cuOut;

SndOutModule* CubebOut = &SndCubeb::cuOut;