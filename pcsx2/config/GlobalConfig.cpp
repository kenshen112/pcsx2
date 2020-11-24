#include "PrecompiledHeader.h"

#include "GlobalConfig.h"

std::unique_ptr<GlobalConfig> g_Conf;

void GlobalConfig::Load()
{

    //ConLog_LoadSaveSettings(conf);
	//SysTraceLog_LoadSaveSettings();

	// TODO - config - clean this up, removed the unique_ptr stuff here
	//cfg["GlobalConfig"] = g_Conf->gui->Save();

	if (!folderUtils.DoesExist(g_Conf->gui->CurrentIso))
	{
		g_Conf->gui->CurrentIso.clear();
	}

#if defined(_WIN32)
	if (!folderUtils.DoesExist(g_Conf->gui->Folders.RunDisc))
	{
		g_Conf->gui->Folders.RunDisc.clear();
	}
#else
	if (!folderUtils.DoesExist(g_Conf->gui->Folders.RunDisc))
	{
		g_Conf->gui->Folders.RunDisc.clear();
	}
#endif
	//sApp.DispatchUiSettingsEvent( loader );

	g_Conf->emulator->load();
    g_Conf->gui->Load();

}

void GlobalConfig::Save()
{
    // If multiple SaveSettings messages are requested, we want to ignore most of them.
	// Saving settings once when the GUI is idle should be fine. :)

	static std::atomic<bool> isPosted(false);

	if (!wxThread::IsMain())
	{
		if (!isPosted.exchange(true))
			//wxGetApp().PostIdleMethod(AppSaveSettings);

		return;
	}

	Console.WriteLn("Saving config files...");

    if (!folderUtils.DoesExist(g_Conf->gui->CurrentIso))
	{
		g_Conf->gui->CurrentIso.clear();
	}

#if defined(_WIN32)
	if (!folderUtils.DoesExist(g_Conf->gui->Folders.RunDisc))
	{
		g_Conf->gui->Folders.RunDisc.clear();
	}
#else
	if (!folderUtils.DoesExist(g_Conf->gui->Folders.RunDisc))
	{
		g_Conf->gui->Folders.RunDisc.clear();
	}
#endif

	//sApp.GetRecentIsoManager().Add(g_Conf->gui->CurrentIso);

	g_Conf->emulator->save();
	g_Conf->gui->Save();

	isPosted = false;
}

GlobalConfig::GlobalConfig()
{
	gui = std::make_unique<GuiConfig>();
	emulator = std::make_unique<Pcsx2Config>();
}