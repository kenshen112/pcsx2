/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2021  PCSX2 Dev Team
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

#include "fmt/core.h"

#include <wx/filename.h>

#include "Utilities/StringHelpers.h"
#include "Utilities/FileUtils.h"

TEST(FileUtils, FromUTFConversion)
{
	std::wstring test_path = L"хелло/world.file";
	fs::path test = FileUtils::FromWxString(wxString(test_path));
	EXPECT_EQ(test.wstring(), test_path);
}

TEST(FileUtils, TOUTFConversion)
{
	fs::path test_path = "хелло/world.file";
	std::wstring test = FileUtils::ToWxString(test_path);
	EXPECT_EQ(test_path.wstring(), test);
}

TEST(FileUtils, IsDirInDirTest1)
{

}
