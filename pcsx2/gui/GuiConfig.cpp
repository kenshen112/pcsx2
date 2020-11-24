#include "PrecompiledHeader.h"
#include "fmt/format.h"
#include "GuiConfig.h"
#include "App.h"
FolderUtils folderUtils;

namespace PathDefs
{
	// Specifies the root folder for the application install.
	// (currently it's the CWD, but in the future I intend to move all binaries to a "bin"
	// sub folder, in which case the approot will become "..") [- Air?]

	//The installer installs the folders which are relative to AppRoot (that's plugins/langs)
	//  relative to the exe folder, and not relative to cwd. So the exe should be default AppRoot. - avih
	fs::path AppRoot()
	{
		//AffinityAssert_AllowFrom_MainUI();

		if (InstallationMode == InstallMode_Registered)
		{
			//static const std::string cwdCache( (std::string)Path::Normalize(wxGetCwd()) );
			//return cwdCache;
		}
		else if (InstallationMode == InstallMode_Portable)

			if (InstallationMode == InstallMode_Registered || InstallationMode == InstallMode_Portable)
			{
				static const std::string appCache((std::string)
													  std::string(wxStandardPaths::Get().GetExecutablePath()));
				return appCache;
			}
			else
				pxFail("Unimplemented user local folder mode encountered.");

		static const std::string dotFail(".");
		return dotFail;
	}

	// Specifies the main configuration folder.
	fs::path GetUserLocalDataDir()
	{
		fs::path temp = wxStandardPaths::Get().GetUserLocalDataDir().ToStdString();
		return temp;
	}

	// Fetches the path location for user-consumable documents -- stuff users are likely to want to
	// share with other programs: screenshots, memory cards, and savestates.
	fs::path GetDocuments(DocsModeType mode)
	{
		switch (mode)
		{
#ifdef XDG_STD
			// Move all user data file into central configuration directory (XDG_CONFIG_DIR)
			case DocsFolder_User:
				return GetUserLocalDataDir();
#else
			case DocsFolder_User:
				return std::string((wxStandardPaths::Get().GetDocumentsDir(), pxGetAppName()));
#endif
			case DocsFolder_Custom:
				return CustomDocumentsFolder;

				jNO_DEFAULT
		}

		return GetDocuments(mode);
	}

	fs::path GetDocuments()
	{
		return GetDocuments(DocsFolderMode);
	}

	fs::path GetProgramDataDir()
	{
#ifndef GAMEINDEX_DIR_COMPILATION
		return AppRoot();
#else
		// Each linux distributions have his rules for path so we give them the possibility to
		// change it with compilation flags. -- Gregory
#define xGAMEINDEX_str(s) GAMEINDEX_DIR_str(s)
#define GAMEINDEX_DIR_str(s) #s
		return std::string(xGAMEINDEX_str(GAMEINDEX_DIR_COMPILATION));
#endif
	}

	fs::path GetSnapshots()
	{
		return (GetDocuments().parent_path() / "snapshots").make_preferred();
	}

	fs::path GetBios()
	{
		return (GetDocuments().parent_path() / "bios").make_preferred();
	}

	fs::path GetCheats()
	{
		return (GetDocuments().parent_path() / "cheats").make_preferred();
	}

	fs::path GetCheatsWS()
	{
		return (GetDocuments().parent_path() / "cheats_ws").make_preferred();
	}

	fs::path GetDocs()
	{
		return (AppRoot().parent_path() / "docs").make_preferred();
	}

	fs::path GetSavestates()
	{
		return (GetDocuments().parent_path() / "sstates").make_preferred();
	}

	fs::path GetMemoryCards()
	{
		return (GetDocuments().parent_path() / "memcards").make_preferred();
	}

	fs::path GetPlugins()
	{
		// Each linux distributions have his rules for path so we give them the possibility to
		// change it with compilation flags. -- Gregory
#ifndef PLUGIN_DIR_COMPILATION
		return (AppRoot().parent_path() / "plugins").make_preferred();
#else
#define xPLUGIN_DIR_str(s) PLUGIN_DIR_str(s)
#define PLUGIN_DIR_str(s) #s
		return std::string(xPLUGIN_DIR_str(PLUGIN_DIR_COMPILATION));
#endif
	}

	fs::path GetSettings()
	{
		fs::path docPath = GetDocuments();
		fs::path path = GetDocuments() / "settings";
		// make_preferred() is causing issues?
		return path;
	}

	fs::path GetLogs()
	{
		return (GetDocuments().parent_path() / "logs").make_preferred();
	}

	fs::path GetLangs()
	{
		return (AppRoot().parent_path() / "langs").make_preferred();
	}

	std::string Get(FoldersEnum_t folderidx)
	{
		switch (folderidx)
		{
			case FolderId_Plugins:
				return GetPlugins();
			case FolderId_Settings:
				return GetSettings();
			case FolderId_Bios:
				return GetBios();
			case FolderId_Snapshots:
				return GetSnapshots();
			case FolderId_Savestates:
				return GetSavestates();
			case FolderId_MemoryCards:
				return GetMemoryCards();
			case FolderId_Logs:
				return GetLogs();
			case FolderId_Langs:
				return GetLangs();
			case FolderId_Cheats:
				return GetCheats();
			case FolderId_CheatsWS:
				return GetCheatsWS();

			case FolderId_Documents:
				return CustomDocumentsFolder;

				jNO_DEFAULT
		}
		return std::string(); // Aw hell naw
	}
}; // namespace PathDefs
std::string& FolderOptions::operator[](FoldersEnum_t folderidx)
{
	switch (folderidx)
	{
		case FolderId_Plugins:
			return PluginsFolder;
		case FolderId_Settings:
			return SettingsFolder;
		case FolderId_Bios:
			return Bios;
		case FolderId_Snapshots:
			return Snapshots;
		case FolderId_Savestates:
			return Savestates;
		case FolderId_MemoryCards:
			return MemoryCards;
		case FolderId_Logs:
			return Logs;
		case FolderId_Langs:
			return Langs;
		case FolderId_Cheats:
			return Cheats;
		case FolderId_CheatsWS:
			return CheatsWS;

		case FolderId_Documents:
			return CustomDocumentsFolder;

			jNO_DEFAULT
	}
	return PluginsFolder; // unreachable, but suppresses warnings.
}

const std::string& FolderOptions::operator[](FoldersEnum_t folderidx) const
{
	return const_cast<FolderOptions*>(this)->operator[](folderidx);
}

bool FolderOptions::IsDefault(FoldersEnum_t folderidx) const
{
	switch (folderidx)
	{
		case FolderId_Plugins:
			return UseDefaultPluginsFolder;
		case FolderId_Settings:
			return UseDefaultSettingsFolder;
		case FolderId_Bios:
			return UseDefaultBios;
		case FolderId_Snapshots:
			return UseDefaultSnapshots;
		case FolderId_Savestates:
			return UseDefaultSavestates;
		case FolderId_MemoryCards:
			return UseDefaultMemoryCards;
		case FolderId_Logs:
			return UseDefaultLogs;
		case FolderId_Langs:
			return UseDefaultLangs;
		case FolderId_Cheats:
			return UseDefaultCheats;
		case FolderId_CheatsWS:
			return UseDefaultCheatsWS;

		case FolderId_Documents:
			return false;

			jNO_DEFAULT
	}
	return false;
}

void FolderOptions::Set(FoldersEnum_t folderidx, const std::string& src, bool useDefault)
{
	switch (folderidx)
	{
		case FolderId_Plugins:
			PluginsFolder = src;
			UseDefaultPluginsFolder = useDefault;
			break;

		case FolderId_Settings:
			SettingsFolder = src;
			UseDefaultSettingsFolder = useDefault;
			break;

		case FolderId_Bios:
			Bios = src;
			UseDefaultBios = useDefault;
			break;

		case FolderId_Snapshots:
			Snapshots = src;
			UseDefaultSnapshots = useDefault;
			break;

		case FolderId_Savestates:
			Savestates = src;
			UseDefaultSavestates = useDefault;
			break;

		case FolderId_MemoryCards:
			MemoryCards = src;
			UseDefaultMemoryCards = useDefault;
			break;

		case FolderId_Logs:
			Logs = src;
			UseDefaultLogs = useDefault;
			break;

		case FolderId_Langs:
			Langs = src;
			UseDefaultLangs = useDefault;
			break;

		case FolderId_Documents:
			CustomDocumentsFolder = src;
			break;

		case FolderId_Cheats:
			Cheats = src;
			UseDefaultCheats = useDefault;
			break;

		case FolderId_CheatsWS:
			CheatsWS = src;
			UseDefaultCheatsWS = useDefault;
			break;

			jNO_DEFAULT
	}
}

// --------------------------------------------------------------------------------------
//  Default Filenames
// --------------------------------------------------------------------------------------
namespace FilenameDefs
{
	std::string GetUiConfig()
	{
		return pxGetAppName().ToStdString() + "_ui";
	}

	std::string GetUiKeysConfig()
	{
		return pxGetAppName().ToStdString() + "_keys";
	}

	std::string GetVmConfig()
	{
		return pxGetAppName().ToStdString() + "_vm";
	}

	std::string GetUsermodeConfig()
	{
		return ("usermode");
	}

	const std::string& Memcard(uint port, uint slot)
	{
		static const std::string retval[2][4] =
			{
				{
					std::string("Mcd001.ps2"),
					std::string("Mcd003.ps2"),
					std::string("Mcd005.ps2"),
					std::string("Mcd007.ps2"),
				},
				{
					std::string("Mcd002.ps2"),
					std::string("Mcd004.ps2"),
					std::string("Mcd006.ps2"),
					std::string("Mcd008.ps2"),
				}};

		IndexBoundsAssumeDev(L"FilenameDefs::Memcard", port, 2);
		IndexBoundsAssumeDev(L"FilenameDefs::Memcard", slot, 4);

		return retval[port][slot];
	}
}; // namespace FilenameDefs


std::string GuiConfig::FullpathTo(PluginsEnum_t pluginidx) const
{
	return Path::Combine(PluginsFolder, BaseFilenames[pluginidx]);
}

// returns true if the filenames are quite absolutely the equivalent.  Works for all
// types of filenames, relative and absolute.  Very important that you use this function
// rather than any other type of more direct string comparison!
bool GuiConfig::FullpathMatchTest(PluginsEnum_t pluginId, const wxString& cmpto)
{
	// Implementation note: wxFileName automatically normalizes things as needed in it's
	// equality comparison implementations, so we can do a simple comparison as follows:

	return wxFileName(cmpto).SameAs((wxFileName)FullpathTo(pluginId));
}

static std::string GetResolvedFolder(FoldersEnum_t id)
{
	return g_Conf->gui->Folders.IsDefault(id) ? PathDefs::Get(id) : g_Conf->gui->Folders[id];
}

fs::path GetLogFolder()
{
	return GetResolvedFolder(FolderId_Logs);
}

fs::path GetCheatsFolder()
{
	return GetResolvedFolder(FolderId_Cheats);
}

fs::path GetCheatsWsFolder()
{
	return GetResolvedFolder(FolderId_CheatsWS);
}

fs::path GetSettingsFolder()
{
	if (!wxGetApp().Overrides.SettingsFolder.empty())
		return wxGetApp().Overrides.SettingsFolder;

	return UseDefaultSettingsFolder ? PathDefs::GetSettings().string() : SettingsFolder;
}

fs::path GetVmSettingsFilename()
{
	fs::path fname(!wxGetApp().Overrides.VmSettingsFile.empty() ? wxGetApp().Overrides.VmSettingsFile : FilenameDefs::GetVmConfig());
	std::cout << "Path: " << Path::Combine(GetSettingsFolder(), fname) << std::endl;
	return Path::Combine(GetSettingsFolder(), fname);
}

fs::path GetUiSettingsFilename()
{
	fs::path fname(FilenameDefs::GetUiConfig());
	return (GetSettingsFolder() / fname).make_preferred();
}

fs::path GetUiKeysFilename()
{
	fs::path fname(FilenameDefs::GetUiKeysConfig());
	return (GetSettingsFolder() / fname).make_preferred();
}

std::string GuiConfig::FullpathToBios() const
{
	return Path::Combine(Folders.Bios, BaseFilenames.Bios);
}

std::string GuiConfig::FullpathToMcd(uint slot) const
{
	return Path::Combine(Folders.MemoryCards, Mcd[slot].Filename.GetFullName().ToStdString());
}

// ------------------------------------------------------------------------
void FolderOptions::ApplyDefaults()
{
	if (UseDefaultBios)
		Bios = PathDefs::GetBios();
	if (UseDefaultSnapshots)
		Snapshots = PathDefs::GetSnapshots();
	if (UseDefaultSavestates)
		Savestates = PathDefs::GetSavestates();
	if (UseDefaultMemoryCards)
		MemoryCards = PathDefs::GetMemoryCards();
	if (UseDefaultLogs)
		Logs = PathDefs::GetLogs();
	if (UseDefaultLangs)
		Langs = PathDefs::GetLangs();
	if (UseDefaultPluginsFolder)
		PluginsFolder = PathDefs::GetPlugins();
	if (UseDefaultCheats)
		Cheats = PathDefs::GetCheats();
	if (UseDefaultCheatsWS)
		CheatsWS = PathDefs::GetCheatsWS();
}

// ------------------------------------------------------------------------
FolderOptions::FolderOptions()
	: Bios(PathDefs::GetBios())
	, Snapshots(PathDefs::GetSnapshots())
	, Savestates(PathDefs::GetSavestates())
	, MemoryCards(PathDefs::GetMemoryCards())
	, Langs(PathDefs::GetLangs())
	, Logs(PathDefs::GetLogs())
	, Cheats(PathDefs::GetCheats())
	, CheatsWS(PathDefs::GetCheatsWS())

	, RunIso(PathDefs::GetDocuments()) // raw default is always the Documents folder.
	, RunELF(PathDefs::GetDocuments()) // raw default is always the Documents folder.
	, RunDisc(PathDefs::GetDocuments())
{
	//bitset = 0xffffffff;
}

#ifndef DISABLE_RECORDING
InputRecordingOptions::InputRecordingOptions()
	: VirtualPadPosition(wxDefaultPosition)
{
}

#endif

// ----------------------------------------------------------------------------
FramerateOptions::FramerateOptions()
{
	NominalScalar = 1.0;
	TurboScalar = 2.0;
	SlomoScalar = 0.50;

	SkipOnLimit = false;
	SkipOnTurbo = false;
}

void FramerateOptions::SanityCheck()
{
	// Ensure Conformation of various options...

	//NominalScalar	.ConfineTo( 0.05, 10.0 );
	//TurboScalar		.ConfineTo( 0.05, 10.0 );
	//SlomoScalar		.ConfineTo( 0.05, 10.0 );
}

UiTemplateOptions::UiTemplateOptions()
{
	LimiterUnlimited = "Max";
	LimiterTurbo = "Turbo";
	LimiterSlowmo = "Slowmo";
	LimiterNormal = "Normal";
	OutputFrame = "Frame";
	OutputField = "Field";
	OutputProgressive = "Progressive";
	OutputInterlaced = "Interlaced";
	Paused = "<PAUSED> ";
	TitleTemplate = "Slot: ${slot} | Speed: ${speed} (${vfps}) | ${videomode} | Limiter: ${limiter} | ${gsdx} | ${omodei} | ${cpuusage}";
#ifndef DISABLE_RECORDING
	RecordingTemplate = "Slot: ${slot} | Frame: ${frame}/${maxFrame} | Rec. Mode: ${mode} | Speed: ${speed} (${vfps}) | Limiter: ${limiter}";
#endif
}

int GuiConfig::GetMaxPresetIndex()
{
	return 5;
}

bool GuiConfig::isOkGetPresetTextAndColor(int n, std::string& label, wxColor& c)
{
	const std::string presetNamesAndColors[][2] =
		{
			{("Safest (No hacks)"), "Blue"},
			{("Safe (Default)"), "Dark Green"},
			{("Balanced"), "Forest Green"},
			{("Aggressive"), "Orange"},
			{("Very Aggressive"), "Red"},
			{("Mostly Harmful"), "Purple"}};
	if (n < 0 || n > GetMaxPresetIndex())
		return false;

	label = wxsFormat(L"%d - ", n + 1) + presetNamesAndColors[n][0];
	c = wxColor(presetNamesAndColors[n][1]);

	return true;
}

ConsoleLogOptions::ConsoleLogOptions()
	: DisplayPosition{100, 100}
	, DisplaySize{680, 560}
	, Theme("Default")
{
	Visible = true;
	AutoDock = true;
	FontSize = 8;
}

GSWindowOptions::GSWindowOptions()
{
	CloseOnEsc = true;
	DefaultToFullscreen = false;
	AlwaysHideMouse = false;
	DisableResizeBorders = false;
	DisableScreenSaver = true;

	AspectRatio = AspectRatio_4_3;
	FMVAspectRatioSwitch = FMV_AspectRatio_Switch_Off;
	Zoom = 100;
	StretchY = 100;
	OffsetX = 0;
	OffsetY = 0;

	WindowSize = wxSize(640, 480);
	WindowPos = wxDefaultPosition;
	IsMaximized = false;
	IsFullscreen = false;
	EnableVsyncWindowFlag = false;

	IsToggleFullscreenOnDoubleClick = true;
}

void GSWindowOptions::SanityCheck()
{
	// Ensure Conformation of various options...

	WindowSize.x = std::max(WindowSize.x, 8);
	WindowSize.x = std::min(WindowSize.x, wxGetDisplayArea().GetWidth() - 16);

	WindowSize.y = std::max(WindowSize.y, 8);
	WindowSize.y = std::min(WindowSize.y, wxGetDisplayArea().GetHeight() - 48);

	// Make sure the upper left corner of the window is visible enought o grab and
	// move into view:
	if (!wxGetDisplayArea().Contains(wxRect(WindowPos, wxSize(48, 48))))
		WindowPos = wxDefaultPosition;

	if ((uint)AspectRatio >= (uint)AspectRatio_MaxCount)
		AspectRatio = AspectRatio_4_3;
}


// ------------------------------------------------------------------------
const std::string& FilenameOptions::operator[](PluginsEnum_t pluginidx) const
{
	IndexBoundsAssumeDev("Filename[Plugin]", pluginidx, PluginId_Count);
	return Plugins[pluginidx];
}

void RelocateLogfile()
{
	if (!folderUtils.DoesExist(g_Conf->gui->Folders.Logs))
	{
		if (!folderUtils.CreateFolder(g_Conf->gui->Folders.Logs))
		{
			return;
		}
	}
	std::string newLogName = (g_Conf->gui->Folders.Logs + "emuLog.txt");

	if ((emuLog != nullptr) && (emuLogName != newLogName))
	{
		wxString logName(newLogName);

		Console.WriteLn(L"\nRelocating Logfile...\n\tFrom: %s\n\tTo  : %s\n", WX_STR(emuLogName), WX_STR(logName));
		wxGetApp().DisableDiskLogging();

		fclose(emuLog);
		emuLog = nullptr;
	}

	if (emuLog == nullptr)
	{
		emuLogName = newLogName;
		emuLog = wxFopen(emuLogName, "wb");
	}

	wxGetApp().EnableAllLogging();
}

// Parameters:
//   overwrite - this option forces the current settings to overwrite any existing settings
//      that might be saved to the configured ini/settings folder.
//
// Notes:
//   The overwrite option applies to PCSX2 options only.  Plugin option behavior will depend
//   on the plugins.
//
void AppConfig_OnChangedSettingsFolder(bool overwrite)
{
	if (!folderUtils.DoesExist(PathDefs::GetDocuments().string()))
	{
		if (!folderUtils.CreateFolder(PathDefs::GetDocuments()))
		{
			return;
		}
	}
	if (!folderUtils.DoesExist(GetSettingsFolder()))
	{
		if (folderUtils.CreateFolder(GetSettingsFolder()))
		{
			return;
		}
	}
	std::string uiFileName = GetUiSettingsFilename();
	if (overwrite)
	{
		if (folderUtils.DoesExist(uiFileName) && !fs::remove(uiFileName))
			throw Exception::AccessDenied(uiFileName)
				.SetBothMsgs(pxL("Failed to overwrite existing settings file; permission was denied."));

		std::string vmFileName = GetVmSettingsFilename();

		if (folderUtils.DoesExist(vmFileName) && !fs::remove(vmFileName))
			throw Exception::AccessDenied(vmFileName)
				.SetBothMsgs(pxL("Failed to overwrite existing settings file; permission was denied."));
	}

	// Bind into wxConfigBase to allow wx to use our config internally, and delete whatever
	// comes out (cleans up prev config, if one).
	//delete wxConfigBase::Set( OpenFileConfig( (std::string)jsonFilename ) );
	//GetAppConfig()->SetRecordDefaults(true);

	AppApplySettings();
	AppSaveSettings(); //Make sure both Yaml files are created if needed.
}
// --------------------------------------------------------------------------------------
//  pxDudConfig
// --------------------------------------------------------------------------------------
// Used to handle config actions prior to the creation of the ini file (for example, the
// first time wizard).  Attempts to save ini settings are simply ignored through this
// class, which allows us to give the user a way to set everything up in the wizard, apply
// settings as usual, and only *save* something once the whole wizard is complete.
//
class pxDudConfig : public wxConfigBase
{
protected:
	wxString m_empty;

public:
	virtual ~pxDudConfig() = default;

	virtual void SetPath(const wxString&) {}
	virtual const wxString& GetPath() const { return m_empty; }

	virtual bool GetFirstGroup(wxString&, long&) const { return false; }
	virtual bool GetNextGroup(wxString&, long&) const { return false; }
	virtual bool GetFirstEntry(wxString&, long&) const { return false; }
	virtual bool GetNextEntry(wxString&, long&) const { return false; }
	virtual size_t GetNumberOfEntries(bool) const { return 0; }
	virtual size_t GetNumberOfGroups(bool) const { return 0; }

	virtual bool HasGroup(const wxString&) const { return false; }
	virtual bool HasEntry(const wxString&) const { return false; }

	virtual bool Flush(bool) { return false; }

	virtual bool RenameEntry(const wxString&, const wxString&) { return false; }

	virtual bool RenameGroup(const wxString&, const wxString&) { return false; }

	virtual bool DeleteEntry(const wxString&, bool bDeleteGroupIfEmpty = true) { return false; }
	virtual bool DeleteGroup(const wxString&) { return false; }
	virtual bool DeleteAll() { return false; }

protected:
	virtual bool DoReadString(const wxString&, wxString*) const { return false; }
	virtual bool DoReadLong(const wxString&, long*) const { return false; }

	virtual bool DoWriteString(const wxString&, const wxString&) { return false; }
	virtual bool DoWriteLong(const wxString&, long) { return false; }

#if wxUSE_BASE64
	virtual bool DoReadBinary(const wxString& key, wxMemoryBuffer* buf) const
	{
		return false;
	}
	virtual bool DoWriteBinary(const wxString& key, const wxMemoryBuffer& buf) { return false; }
#endif
};

static pxDudConfig _dud_config;

/*static void LoadUiSettings(wxConfigBase* conf)
{
	//ConLog_LoadSaveSettings(conf);
	//SysTraceLog_LoadSaveSettings();

	g_Conf->gui->Load();

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
}*/

/*static void SaveUiSettings()
{
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

	sApp.GetRecentIsoManager().Add(g_Conf->gui->CurrentIso);

	g_Conf->gui->Save();

	//std::string toSave = saver.dump();

	//sApp.DispatchUiSettingsEvent( saver );
	//fileUtils.Save(GetUiSettingsFilename(), toSave);
}*/

static void SaveRegSettings()
{
	bool conf_install;

	if (InstallationMode == InstallMode_Portable)
		return;

	// sApp. macro cannot be use because you need the return value of OpenInstallSettingsFile method
	//if( Pcsx2App* __app_ = (Pcsx2App*)wxApp::GetInstance() ) conf_install = std::unique_ptr<nlohmann::json>((*__app_).OpenInstallSettingsFile());
	//conf_install->SetRecordDefaults(false);

	//App_SaveInstallSettings( conf_install.get() );
}

void AppSaveSettings()
{
	// If multiple SaveSettings messages are requested, we want to ignore most of them.
	// Saving settings once when the GUI is idle should be fine. :)

	static std::atomic<bool> isPosted(false);

	if (!wxThread::IsMain())
	{
		if (!isPosted.exchange(true))
			wxGetApp().PostIdleMethod(AppSaveSettings);

		return;
	}

	Console.WriteLn("Saving config files...");

	SaveRegSettings(); // save register because of PluginsFolder change

	isPosted = false;
}

// Returns the current application configuration file.  This is preferred over using
// wxConfigBase::GetAppConfig(), since it defaults to *not* creating a config file
// automatically (which is typically highly undesired behavior in our system)
std::shared_ptr<wxConfigBase> GetAppConfig()
{
	return nullptr;
}

GuiConfig::GuiConfig()
	: MainGuiPosition(wxDefaultPosition)
	, SysSettingsTabName("Cpu")
	, McdSettingsTabName("none")
	, ComponentsTabName("Plugins")
	, AppSettingsTabName("none")
	, GameDatabaseTabName("none")
{
	LanguageId = wxLANGUAGE_DEFAULT;
	LanguageCode = "default";
	RecentIsoCount = 20;
	Listbook_ImageSize = 32;
	Toolbar_ImageSize = 24;
	Toolbar_ShowLabels = true;

#ifdef __WXMSW__
	McdCompressNTFS = true;
#endif
	EnableSpeedHacks = true;
	EnableGameFixes = false;
	EnableFastBoot = true;

	EnablePresets = true;
	PresetIndex = 1;

	CdvdSource = CDVD_SourceType::Iso;

	// To be moved to FileMemoryCard pluign (someday)
	for (uint slot = 0; slot < 8; ++slot)
	{
		Mcd[slot].Enabled = !FileMcd_IsMultitapSlot(slot); // enables main 2 slots
		Mcd[slot].Filename = FileMcd_GetDefaultName(slot);

		// Folder memory card is autodetected later.
		Mcd[slot].Type = MemoryCardType::MemoryCard_File;
	}

	GzipIsoIndexTemplate = "$(f).pindex.tmp";
}

std::shared_ptr<wxConfigBase> GuiConfig::Init()
{
	fs::path programFullPath = wxStandardPaths::Get().GetExecutablePath().ToStdString();
	std::string programDir(Path::Combine(programFullPath.parent_path(), "settings/PCSX2_ui.ini"));

    if (conf == nullptr)
	{
        conf = std::make_shared<wxFileConfig>(L"", L"", programDir, L"", wxCONFIG_USE_LOCAL_FILE );
	}

	isInit = true;
	return conf;
}

void GuiConfig::SetCategory(const wxString &path)
{
	conf = nullptr;

	conf = Init();

    if (conf)
	{
		conf->SetPath(path);
		conf->Set(conf.get());
	}

}

void GuiConfig::Load()
{
	Init();

	if (conf)
	{
		bool rel;
	    
		wxString group = "FolderOptions";
		long index = 0;



		conf->GetFirstGroup(group, index);
		
		Folders.UseDefaultBios = conf->Read("UseDefaultBios", Folders.UseDefaultBios);
		Folders.UseDefaultSavestates = conf->Read("UseDefaultSavestates", Folders.UseDefaultSavestates);
		Folders.UseDefaultMemoryCards = conf->Read("UseDefaultMemoryCards", Folders.UseDefaultMemoryCards);
		Folders.UseDefaultLogs = conf->Read("UseDefaultLogs", Folders.UseDefaultLogs);
		Folders.UseDefaultLangs = conf->Read("UseDefaultLangs", Folders.UseDefaultLangs);
		UseDefaultPluginsFolder = conf->Read("UseDefaultPluginsFolder", UseDefaultPluginsFolder);
		Folders.UseDefaultCheats = conf->Read("UseDefaultCheats", Folders.UseDefaultCheats);
		Folders.UseDefaultCheatsWS = conf->Read("UseDefaultCheatsWS", Folders.UseDefaultCheatsWS);

		Folders.Bios = conf->Read(Folders.Bios, rel);
		Folders.Snapshots = conf->Read(Folders.Snapshots, rel);
		Folders.Savestates = conf->Read(Folders.Savestates, rel);
		Folders.MemoryCards = conf->Read(Folders.MemoryCards, rel);
		Folders.Logs = conf->Read(Folders.Logs, rel);
		Folders.Langs = conf->Read(Folders.Langs, rel);
		Folders.Cheats = conf->Read(Folders.Cheats, rel);
		Folders.CheatsWS = conf->Read(Folders.CheatsWS, rel);
		PluginsFolder = conf->Read(PluginsFolder, wxString(Path::Combine(InstallFolder, "plugins")));

		Folders.RunIso.string() = conf->Read(Folders.RunIso.string(), rel);
		Folders.RunELF.string() = conf->Read(Folders.RunELF.string(), rel);

		conf = nullptr;
		g_Conf->gui->SetCategory("GSWindowOptions");

		gsWindow.Zoom = conf->Read("Zoom", gsWindow.Zoom);
		gsWindow.OffsetX = conf->Read("OffsetX", gsWindow.OffsetX);
		gsWindow.OffsetY = conf->Read("OffsetY", gsWindow.OffsetY);
		gsWindow.StretchY = conf->Read("StretchY", gsWindow.StretchY);
		gsWindow.WindowPos.x = conf->Read("WindowPosX", gsWindow.WindowPos.x);
		gsWindow.WindowPos.y = conf->Read("WindowPosY", gsWindow.WindowPos.y);
		gsWindow.WindowSize.x = conf->Read("WindowSizeX", gsWindow.WindowSize.x);
		gsWindow.WindowSize.y = conf->Read("WindowSizeY", gsWindow.WindowSize.y);
		gsWindow.CloseOnEsc = conf->Read("CloseOnEsc", gsWindow.CloseOnEsc);
		conf->Read("AspectRatio", (int)&gsWindow.AspectRatio);
		gsWindow.IsMaximized = conf->Read("IsMaximized", gsWindow.IsMaximized);
		gsWindow.IsFullscreen = conf->Read("IsFullscreen", gsWindow.IsFullscreen);
		gsWindow.AlwaysHideMouse = conf->Read("AlwaysHideMouse", gsWindow.AlwaysHideMouse);
		gsWindow.DisableScreenSaver = conf->Read("DisableScreenSaver", gsWindow.DisableScreenSaver);
		gsWindow.DefaultToFullscreen = conf->Read("DefaultToFullScreen", gsWindow.DefaultToFullscreen);
		gsWindow.DisableResizeBorders = conf->Read("DisableResizeBorders", gsWindow.DisableResizeBorders);
		conf->Read("FMVAspectRatioSwitch", (int)&gsWindow.FMVAspectRatioSwitch);
		gsWindow.EnableVsyncWindowFlag = conf->Read("EnableVsyncWindowFlag", gsWindow.EnableVsyncWindowFlag);
		gsWindow.IsToggleFullscreenOnDoubleClick = conf->Read("IsToggleFullscreenOnDoubleClick", gsWindow.IsToggleFullscreenOnDoubleClick);

		conf = nullptr;
		g_Conf->gui->SetCategory("UiTemplateOptions");
		Templates.LimiterUnlimited = conf->Read("LimiterUnlimited", Templates.LimiterUnlimited);
		Templates.LimiterTurbo = conf->Read("LimiterTurbo", Templates.LimiterTurbo);
		Templates.LimiterSlowmo = conf->Read("LimiterSlowmo", Templates.LimiterSlowmo);
		Templates.LimiterNormal = conf->Read("LimiterNormal", Templates.LimiterNormal);
		Templates.OutputFrame = conf->Read("OutputFrame", Templates.OutputFrame);
		Templates.OutputField = conf->Read("OutputField", Templates.OutputField);
		Templates.OutputProgressive = conf->Read("OutputProgressive", Templates.OutputProgressive);
		Templates.OutputInterlaced = conf->Read("OutputInterlaced", Templates.OutputInterlaced);
		Templates.Paused = conf->Read("Paused", Templates.Paused);
		Templates.TitleTemplate = conf->Read("TitleTemplate", Templates.TitleTemplate);
#ifndef DISABLE_RECORDING
		Templates.RecordingTemplate = conf->Read("RecordingTemplate", Templates.RecordingTemplate);
#endif
		g_Conf->gui->SetCategory("FilenameOptions");
		BaseFilenames.Bios = conf->Read("BIOS", BaseFilenames.Bios);

		g_Conf->gui->SetCategory("ConsoleLogOptions");
		console.Theme = conf->Read("Theme", console.Theme);
		console.FontSize = conf->Read("FontSize", console.FontSize);
		console.Visible = conf->Read("IsVisible", console.Visible);
		console.AutoDock = conf->Read("Autodock", console.AutoDock);
		console.DisplaySize.x = conf->ReadObject("DisplaySizeX", console.DisplaySize.x);
		console.DisplaySize.y = conf->ReadObject("DisplaySizeY", console.DisplaySize.y);
		console.DisplayPosition.x = conf->ReadObject("DisplayPositionX", console.DisplayPosition.x);
		console.DisplayPosition.y = conf->ReadObject("DisplayPositionY", console.DisplayPosition.y);

		conf->Read("SlomoScalar", Framerate.SlomoScalar);

		g_Conf->gui->SetCategory("Core");
		MainGuiPosition.x = conf->Read("MainGuiPositionX", MainGuiPosition.x);
		MainGuiPosition.y = conf->Read("MainGuiPositionY", MainGuiPosition.y);
		SysSettingsTabName = conf->Read("SysSettingsTabName", SysSettingsTabName);
		McdSettingsTabName = conf->Read("McdSettingsTabName", McdSettingsTabName);
		ComponentsTabName = conf->Read("ComponentsTabName", ComponentsTabName);
		AppSettingsTabName = conf->Read("AppSettingsTabName", AppSettingsTabName);
		GameDatabaseTabName = conf->Read("GameDatabaseTabName", GameDatabaseTabName);
		conf->Read("LanguageId", (int)&LanguageId);
		LanguageCode = conf->Read("LanguageCode", LanguageCode);
	}
}

void GuiConfig::Save()
{
	g_Conf->gui->SetCategory("GSWindowOptions");
	conf->Write("Zoom", gsWindow.Zoom);
	conf->Write("OffsetX", gsWindow.OffsetX);
	conf->Write("OffsetY", gsWindow.OffsetY);
	conf->Write("StretchY", gsWindow.StretchY);
	conf->Write("WindowPosX", gsWindow.WindowPos.x);
	conf->Write("WindowPosY", gsWindow.WindowPos.y);
	conf->Write("WindowSizeX", gsWindow.WindowSize.x);
	conf->Write("WindowSizeY", gsWindow.WindowSize.y);
	conf->Write("CloseOnEsc", gsWindow.CloseOnEsc);
	conf->Write("AspectRatio", (int)gsWindow.AspectRatio);
	conf->Write("IsMaximized", gsWindow.IsMaximized);
	conf->Write("IsFullscreen", gsWindow.IsFullscreen);
	conf->Write("AlwaysHideMouse", gsWindow.AlwaysHideMouse);
	conf->Write("DisableScreenSaver", gsWindow.DisableScreenSaver);
	conf->Write("DefaultToFullScreen", gsWindow.DefaultToFullscreen);
	conf->Write("DisableResizeBorders", gsWindow.DisableResizeBorders);
	conf->Write("FMVAspectRatioSwitch", (int)gsWindow.FMVAspectRatioSwitch);
	conf->Write("EnableVsyncWindowFlag", gsWindow.EnableVsyncWindowFlag);
	conf->Write("IsToggleFullscreenOnDoubleClick", gsWindow.IsToggleFullscreenOnDoubleClick);

	g_Conf->gui->SetCategory("ConsoleLogOptions");
	conf->Write("Theme", console.Theme);
	conf->Write("FontSize", console.FontSize);
	conf->Write("IsVisible", console.Visible);
	conf->Write("Autodock", console.AutoDock);
	conf->Write("DisplaySizeX", console.DisplaySize.x);
	conf->Write("DisplaySizeY", console.DisplaySize.y);
	conf->Write("DisplayPositionX", console.DisplayPosition.x);
	conf->Write("DisplayPositionY", console.DisplayPosition.y);

	g_Conf->gui->SetCategory("UiTemplateOptions");
	conf->Write(L"LimiterUnlimited", wxString(Templates.LimiterUnlimited));
	conf->Write(L"LimiterTurbo", wxString(Templates.LimiterTurbo));
	conf->Write(L"LimiterSlowmo", wxString(Templates.LimiterSlowmo));
	conf->Write(L"LimiterNormal", wxString(Templates.LimiterNormal));
	conf->Write(L"OutputFrame", wxString(Templates.OutputFrame));
	conf->Write(L"OutputField", wxString(Templates.OutputField));
	conf->Write(L"OutputProgressive", wxString(Templates.OutputProgressive));
	conf->Write(L"OutputInterlaced", wxString(Templates.OutputInterlaced));
	conf->Write(L"Paused", wxString(Templates.Paused));
	conf->Write(L"TitleTemplate", wxString(Templates.TitleTemplate));
#ifndef DISABLE_RECORDING
	conf->Write(L"RecordingTemplate", wxString(Templates.RecordingTemplate));
#endif

	// when saving in portable mode, we save relative paths if possible
	// --> on load, these relative paths will be expanded relative to the exe folder.
	bool rel = true;

	g_Conf->gui->SetCategory("FolderOptions");
	conf->Write("UseDefaultBios", Folders.UseDefaultBios);
	conf->Write("UseDefaultSavestates", Folders.UseDefaultSavestates);
	conf->Write("UseDefaultMemoryCards", Folders.UseDefaultMemoryCards);
	conf->Write("UseDefaultLogs", Folders.UseDefaultLogs);
	conf->Write("UseDefaultLangs", Folders.UseDefaultLangs);
	conf->Write("UseDefaultPluginsFolder", UseDefaultPluginsFolder);
	conf->Write("UseDefaultCheats", Folders.UseDefaultCheats);
	conf->Write("UseDefaultCheatsWS", Folders.UseDefaultCheatsWS);

	conf->Write(Folders.Bios, rel);
	conf->Write(Folders.Snapshots, rel);
	conf->Write(Folders.Savestates, rel);
	conf->Write(Folders.MemoryCards, rel);
	conf->Write(Folders.Logs, rel);
	conf->Write(Folders.Langs, rel);
	conf->Write(Folders.Cheats, rel);
	conf->Write(Folders.CheatsWS, rel);
	conf->Write(PluginsFolder, wxString(Path::Combine(InstallFolder, "plugins")));

	conf->Write(Folders.RunIso.string(), rel);
	conf->Write(Folders.RunELF.string(), rel);

	// ApplyDefaults();

	for (int i = 0; i < FolderId_COUNT; ++i)
	{
		Folders.operator[]((FoldersEnum_t)i);
	}

	g_Conf->gui->SetCategory("MemcardOptions");

	for (uint slot = 0; slot < 2; ++slot)
	{
		conf->Write(wxString(fmt::format("Slot{}u_Enable", slot)), Mcd[slot].Enabled);
		conf->Write(wxString(fmt::format("Slot{}u_Filename", slot)), Mcd[slot].Filename.GetFullName());
	}

	for (uint slot = 2; slot < 8; ++slot)
	{
		int mtport = FileMcd_GetMtapPort(slot) + 1;
		int mtslot = FileMcd_GetMtapSlot(slot) + 1;

		conf->Write(wxString(fmt::format("Multitap{}_Slot{}_Enable", mtport, mtslot)), Mcd[slot].Enabled);
		conf->Write(wxString(fmt::format("Multitap{}_Slot{}_Filename", mtport, mtslot)), Mcd[slot].Filename.GetFullName());
	}

	static const std::string pc("Please Configure");
	g_Conf->gui->SetCategory("FilenameOptions");

	//when saving in portable mode, we just save the non-full-path filename
	//  --> on load they'll be initialized with default (relative) paths (works both for plugins and bios)
	//note: this will break if converting from install to portable, and custom folders are used. We can live with that.
	bool needRelativeName = false; //ini.IsSaving() && IsPortable();

	for (int i = 0; i < PluginId_Count; ++i)
	{
		wxString pluginShortName = static_cast<std::string>(tbl_PluginInfo[i].GetShortname());
		if (needRelativeName)
		{
			wxFileName plugin_filename = wxFileName(BaseFilenames.Plugins[i]);
			conf->Write(pluginShortName, plugin_filename.GetFullPath());
		}
		else
			conf->Write(pluginShortName, wxString(BaseFilenames.Plugins[i]));
	}

	if (needRelativeName)
	{
		wxString bios_filename = BaseFilenames.Bios;
		conf->Write("BIOS", bios_filename);
	}
	else
		conf->Write("BIOS", wxString(BaseFilenames.Bios));

	g_Conf->gui->SetCategory("InputRecordingOptions");
	conf->Write("VirtualPadPositionX", inputRecording.VirtualPadPosition.x);
	conf->Write("VirtualPadPositionY", inputRecording.VirtualPadPosition.y);

	g_Conf->gui->SetCategory("FramerateOptions");
	conf->Write("SlomoScalar", Framerate.SlomoScalar);


	g_Conf->gui->SetCategory("Core");
	conf->Write("RecentIsoCount", RecentIsoCount);
	conf->Write("Listbook_ImageSize", Listbook_ImageSize);
	conf->Write("Toolbar_ImageSize", Toolbar_ImageSize);
	conf->Write("Toolbar_ShowLabels", Toolbar_ShowLabels);
	conf->Write("CurrentIso", wxString(CurrentIso.string())); // TODO - missing the allow relative flag
	conf->Write("CurrentBlockdump", wxString(CurrentBlockdump));
	conf->Write("CurrentELF", wxString(CurrentELF));
	conf->Write("CurrentIRX", wxString(CurrentIRX));
	conf->Write("EnableSpeedHacks", EnableSpeedHacks);
	conf->Write("EnableGameFixes", EnableGameFixes);
	conf->Write("EnableFastBoot", EnableFastBoot);
	conf->Write("EnablePresets", EnablePresets);
	conf->Write("PresetIndex", PresetIndex);
	conf->Write("AskOnBoot", AskOnBoot);
	conf->Write("AskOnBoot", AskOnBoot);
	conf->Write("AskOnBoot", AskOnBoot);
	conf->Write("AskOnBoot", AskOnBoot);
	conf->Write("MainGuiPositionX", MainGuiPosition.x);
	conf->Write("MainGuiPositionY", MainGuiPosition.y);
	conf->Write("SysSettingsTabName", SysSettingsTabName);
	conf->Write("McdSettingsTabName", McdSettingsTabName);
	conf->Write("ComponentsTabName", ComponentsTabName);
	conf->Write("AppSettingsTabName", AppSettingsTabName);
	conf->Write("GameDatabaseTabName", GameDatabaseTabName);
	conf->Write("LanguageId", (int)LanguageId);
	conf->Write("LanguageCode", LanguageCode);

	// TODO - these are not basic types at all
	//base->Write("CdvdSource", CdvdSource, CDVD_SourceLabels, CdvdSource );

	conf->Flush();
}

GuiConfig::~GuiConfig()
{
}