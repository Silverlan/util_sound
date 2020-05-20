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
#include ".\apetag.h"

CAPETag* CAPETag::FindTag(CMPAStream* pStream, bool bAppended, DWORD dwBegin, DWORD dwEnd)
{
	DWORD dwOffset;

	if (!bAppended)
	{
		// stands at the beginning of file (complete header is 32 bytes)
		dwOffset = dwBegin;
	}
	else
	{
		// stands at the end of the file (complete footer is 32 bytes)
		dwOffset = dwEnd - 32;		
	}
	BYTE* pBuffer = pStream->ReadBytes(8, dwOffset, false);

	if (memcmp("APETAGEX", pBuffer, 8) == 0)
		return new CAPETag(pStream, bAppended, dwOffset);
	return NULL;
}

CAPETag::CAPETag(CMPAStream* pStream, bool bAppended, DWORD dwOffset) :
	CTag(pStream, _T("APE"), bAppended, dwOffset)
{
	dwOffset += 8;
	DWORD dwVersion = pStream->ReadLEValue(4, dwOffset);
	
	// first byte is version number
	m_fVersion = dwVersion/1000.0f;
	
	// get size
	m_dwSize = pStream->ReadLEValue(4, dwOffset);

	/*DWORD dwNumItems = */pStream->ReadLEValue(4, dwOffset);

	// only valid for version 2
	DWORD dwFlags = pStream->ReadLEValue(4, dwOffset);
	bool bHeader, bFooter;
	if (m_fVersion > 1.0f)
	{
		bHeader = dwFlags >> 31 & 0x1;
		bFooter = dwFlags >> 30 & 0x1;
	}
	else
	{
		bHeader = false;
		bFooter = true;
	}

	if (bHeader)
		m_dwSize += 32;	// add header

	if (bAppended)
		m_dwOffset -= (m_dwSize - 32);
}

CAPETag::~CAPETag(void)
{
}
#endif
#endif