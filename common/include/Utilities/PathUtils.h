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
	/**
	 * @brief Indicates whether dir2 is contained within dir1
	 * @param dir1 Directory that may or may not contain the second directory
	 * @param dir2 The second directory
	 * @return true/false
	*/
	extern bool IsDirectoryWithinDirectory(fs::path base, fs::path dir);

	// Returns -1 if the file does not exist.
	extern s64 GetFileSize(const fs::path& file);

	extern wxString Normalize(const wxString& srcpath);
	extern wxString Normalize(const wxDirName& srcpath);

	extern fs::path Combine(const fs::path& srcPath, const fs::path& srcFile);
	extern std::string ReplaceExtension(const wxString& src, const wxString& ext);
	extern std::string ReplaceFilename(const wxString& src, const wxString& newfilename);
	extern wxString GetFilenameWithoutExt(const wxString& src);
	extern fs::path GetRootDirectory(const wxString& src);
	extern fs::path GetExecutableDirectory();
	extern wxString ToWxString(const fs::path&);
	extern fs::path FromWxString(const wxString&);
} // namespace Path
