// Source: https://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=8295

/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3 of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
* 
* Copyright (c) 2007 Konrad Windszus
*/

#ifdef _WIN32
#include "util_sound_definitions.hpp"
#if USOUND_MP3_SUPPORT_ENABLED == 1
#include <Windows.h>
#include <atlstr.h>
#include ".\tag.h"

CTag::CTag(CMPAStream* pStream, LPCTSTR szName, bool bAppended, DWORD dwOffset, DWORD dwSize):
	m_pStream(pStream), m_bAppended(bAppended), m_dwOffset(dwOffset), m_dwSize(dwSize)
{
	m_szName = _tcsdup(szName);
}

CTag::~CTag(void)
{
	free(m_szName);
}

void CTag::SetVersion(BYTE bVersion1, BYTE bVersion2, BYTE bVersion3)
{
	// only values between 0 and 9 are displayed correctly
	m_fVersion = bVersion1;
	m_fVersion += bVersion2 * 0.1f;
	m_fVersion += bVersion3 * 0.01f;
}
#endif
#endif
