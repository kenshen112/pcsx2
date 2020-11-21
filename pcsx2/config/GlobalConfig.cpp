#include "PrecompiledHeader.h"

#include "GlobalConfig.h"

std::unique_ptr<GlobalConfig> g_Conf;

void GlobalConfig::Load()
{
	g_Conf->emulator->load();
    g_Conf->gui->Load();

}

void GlobalConfig::Save()
{
	g_Conf->emulator->save();
	g_Conf->gui->Save();
}

GlobalConfig::GlobalConfig()
{
	gui = std::make_unique<GuiConfig>();
	emulator = std::make_unique<Pcsx2Config>();
}