/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2020  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PrecompiledHeader.h"
#include "../Global.h"
#include "Dialogs.h"
#include "Config.h"
#include "Utilities/PathUtils.h"

bool DebugEnabled = false;
bool _MsgToConsole = false;
bool _MsgKeyOnOff = false;
bool _MsgVoiceOff = false;
bool _MsgDMA = false;
bool _MsgAutoDMA = false;
bool _MsgOverruns = false;
bool _MsgCache = false;

bool _AccessLog = false;
bool _DMALog = false;
bool _WaveLog = false;

bool _CoresDump = false;
bool _MemDump = false;
bool _RegDump = false;

// this is set true if PCSX2 invokes the SetLogDir callback, which tells SPU2 to use that over
// the configured crap in the ini file.
static bool LogLocationSetByPcsx2 = false;

static std::string LogsFolder;
static std::string DumpsFolder;

wxString AccessLogFileName;
wxString WaveLogFileName;
wxString DMA4LogFileName;
wxString DMA7LogFileName;

wxString CoresDumpFileName;
wxString MemDumpFileName;
wxString RegDumpFileName;

void CfgSetLogDir(const char* dir)
{
	LogsFolder = (dir == nullptr) ? wxString(L"logs") : fromUTF8(dir);
	DumpsFolder = (dir == nullptr) ? wxString(L"logs") : fromUTF8(dir);
	LogLocationSetByPcsx2 = (dir != nullptr);
}

FILE* OpenBinaryLog(const std::string& logfile)
{
	return wxFopen(Path::Combine(LogsFolder, logfile), L"wb");
}

FILE* OpenLog(const std::string& logfile)
{
	return wxFopen(Path::Combine(LogsFolder, logfile), L"w");
}

FILE* OpenDump(const std::string& logfile)
{
	return wxFopen(Path::Combine(DumpsFolder, logfile), L"w");
}

namespace DebugConfig
{
	static const wchar_t* Section = L"DEBUG";

	static void set_default_filenames()
	{
		AccessLogFileName = L"SPU2Log.txt";
		WaveLogFileName = L"SPU2log.wav";
		DMA4LogFileName = L"SPU2dma4.dat";
		DMA7LogFileName = L"SPU2dma7.dat";

		CoresDumpFileName = L"SPU2Cores.txt";
		MemDumpFileName = L"SPU2mem.dat";
		RegDumpFileName = L"SPU2regs.dat";
	}

	void ReadSettings()
	{
		DebugEnabled = CfgReadBool(Section, L"Global_Enable", 0);
		_MsgToConsole = CfgReadBool(Section, L"Show_Messages", 0);
		_MsgKeyOnOff = CfgReadBool(Section, L"Show_Messages_Key_On_Off", 0);
		_MsgVoiceOff = CfgReadBool(Section, L"Show_Messages_Voice_Off", 0);
		_MsgDMA = CfgReadBool(Section, L"Show_Messages_DMA_Transfer", 0);
		_MsgAutoDMA = CfgReadBool(Section, L"Show_Messages_AutoDMA", 0);
		_MsgOverruns = CfgReadBool(Section, L"Show_Messages_Overruns", 0);
		_MsgCache = CfgReadBool(Section, L"Show_Messages_CacheStats", 0);

		_AccessLog = CfgReadBool(Section, L"Log_Register_Access", 0);
		_DMALog = CfgReadBool(Section, L"Log_DMA_Transfers", 0);
		_WaveLog = CfgReadBool(Section, L"Log_WAVE_Output", 0);

		_CoresDump = CfgReadBool(Section, L"Dump_Info", 0);
		_MemDump = CfgReadBool(Section, L"Dump_Memory", 0);
		_RegDump = CfgReadBool(Section, L"Dump_Regs", 0);

		set_default_filenames();

		CfgReadStr(Section, L"Access_Log_Filename", AccessLogFileName, L"logs/SPU2Log.txt");
		CfgReadStr(Section, L"WaveLog_Filename", WaveLogFileName, L"logs/SPU2log.wav");
		CfgReadStr(Section, L"DMA4Log_Filename", DMA4LogFileName, L"logs/SPU2dma4.dat");
		CfgReadStr(Section, L"DMA7Log_Filename", DMA7LogFileName, L"logs/SPU2dma7.dat");

		CfgReadStr(Section, L"Info_Dump_Filename", CoresDumpFileName, L"logs/SPU2Cores.txt");
		CfgReadStr(Section, L"Mem_Dump_Filename", MemDumpFileName, L"logs/SPU2mem.dat");
		CfgReadStr(Section, L"Reg_Dump_Filename", RegDumpFileName, L"logs/SPU2regs.dat");
	}


	void WriteSettings()
	{
		CfgWriteBool(Section, L"Global_Enable", DebugEnabled);

		CfgWriteBool(Section, L"Show_Messages", _MsgToConsole);
		CfgWriteBool(Section, L"Show_Messages_Key_On_Off", _MsgKeyOnOff);
		CfgWriteBool(Section, L"Show_Messages_Voice_Off", _MsgVoiceOff);
		CfgWriteBool(Section, L"Show_Messages_DMA_Transfer", _MsgDMA);
		CfgWriteBool(Section, L"Show_Messages_AutoDMA", _MsgAutoDMA);
		CfgWriteBool(Section, L"Show_Messages_Overruns", _MsgOverruns);
		CfgWriteBool(Section, L"Show_Messages_CacheStats", _MsgCache);

		CfgWriteBool(Section, L"Log_Register_Access", _AccessLog);
		CfgWriteBool(Section, L"Log_DMA_Transfers", _DMALog);
		CfgWriteBool(Section, L"Log_WAVE_Output", _WaveLog);

		CfgWriteBool(Section, L"Dump_Info", _CoresDump);
		CfgWriteBool(Section, L"Dump_Memory", _MemDump);
		CfgWriteBool(Section, L"Dump_Regs", _RegDump);

		set_default_filenames();
		CfgWriteStr(Section, L"Access_Log_Filename", AccessLogFileName);
		CfgWriteStr(Section, L"WaveLog_Filename", WaveLogFileName);
		CfgWriteStr(Section, L"DMA4Log_Filename", DMA4LogFileName);
		CfgWriteStr(Section, L"DMA7Log_Filename", DMA7LogFileName);

		CfgWriteStr(Section, L"Info_Dump_Filename", CoresDumpFileName);
		CfgWriteStr(Section, L"Mem_Dump_Filename", MemDumpFileName);
		CfgWriteStr(Section, L"Reg_Dump_Filename", RegDumpFileName);
	}

} // namespace DebugConfig
