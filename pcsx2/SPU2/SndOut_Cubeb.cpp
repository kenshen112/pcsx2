#include "PrecompiledHeader.h"
//#include "Mixer.h"
#include "Global.h"
#include "SndOut.h"
#include "cubeb/cubeb.h"
class SndCubeb : public SndOutModule
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// Instance vars
	int rv;
	u32 rate;
	u32 latency_frames;
	u64 ts;

	bool started;
	cubeb* api;
	cubeb_stream* stream;
	//////////////////////////////////////////////////////////////////////////////////////////


	s32 Init() override
	{
		cubeb_init(&api, "PCSX2", NULL);                       // The inital startup of the underlying cubeb struct
		rv = cubeb_get_preferred_sample_rate(api, &rate); // Sample rate

		if (rv != CUBEB_OK)
		{
			fprintf(stderr, "Could not get preferred sample-rate");
			return rv;
		}

		//cubeb_stream_init(stream, "PCSX2 Audio", nullptr, nullptr);
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
		return L"Cubeb";
	}

	void Close() override
	{
	}
} static cuOut;

SndOutModule* CubebOut = &cuOut;