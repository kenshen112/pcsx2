#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include "FixedPointTypes.inl"
#include "ghc/filesystem.h"
#include "Path.h"

namespace fs = ghc::filesystem;


namespace Path
{
	extern bool IsRelative(const std::string& path);
	// Returns -1 if the file does not exist.
	extern s64 GetFileSize(const fs::path& file);

	extern wxString Normalize(const wxString& srcpath);
	extern wxString Normalize(const wxDirName& srcpath);
	extern std::string MakeAbsolute(const std::string& srcpath);

	extern fs::path Combine(const fs::path &srcPath, const fs::path &srcFile);
	extern std::string ReplaceExtension(const wxString& src, const wxString& ext);
	extern std::string ReplaceFilename(const wxString& src, const wxString& newfilename);
	extern std::string GetFilename(const std::string& src);
	extern std::string GetDirectory(const std::string& src);
	extern wxString GetFilenameWithoutExt(const wxString& src);
	extern std::string GetRootDirectory(const wxString& src);
	extern fs::path GetExecutableDirectory();
	extern fs::path getPath(const fs::path &p, const fs::path &b, bool isPortable);
	extern wxString ToWxString(const fs::path&);
	extern bool CreateFolder(const fs::path& path);
	extern fs::path FromWxString(const wxString&);
	// Is the folder empty
	extern bool Empty(std::string);
	// Does the path exist
	extern bool DoesExist(const fs::path& path);
} // namespace Path

