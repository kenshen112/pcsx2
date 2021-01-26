#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include "FixedPointTypes.inl"
#include "ghc/filesystem.h"
#include "Path.h"

namespace fs = ghc::filesystem;

class FolderUtils
{
protected:
	fs::path folder;

public:
	bool CreateFolder(fs::path path);
	// Is the folder empty
	bool Empty(std::string);          
	// Does the path exist
	bool DoesExist(std::string path);
	// Does the path exist
	bool DoesExist(fs::path path);
};

namespace Path
{
	extern bool IsRelative(const std::string& path);
	extern s64 GetFileSize(const std::string& path);

	extern wxString Normalize(const wxString& srcpath);
	extern wxString Normalize(const wxDirName& srcpath);
	extern std::string MakeAbsolute(const std::string& srcpath);

	extern fs::path Combine(fs::path &srcPath, fs::path &srcFile);
	extern std::string Combine(const std::string& srcPath, const std::string& srcFile);
	extern std::string ReplaceExtension(const wxString& src, const wxString& ext);
	extern std::string ReplaceFilename(const wxString& src, const wxString& newfilename);
	extern std::string GetFilename(const std::string& src);
	extern std::string GetDirectory(const std::string& src);
	extern wxString GetFilenameWithoutExt(const wxString& src);
	extern std::string GetRootDirectory(const wxString& src);
	extern fs::path GetExecutableDirectory();
	extern fs::path isPortable(fs::path p, bool isPortable);
} // namespace Path

