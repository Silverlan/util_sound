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
#include ".\lyrics3tag.h"
#include "mpaexception.h"

#define UNUSED(a) a

CLyrics3Tag* CLyrics3Tag::FindTag(CMPAStream* pStream, bool bAppended, DWORD dwBegin, DWORD dwEnd)
{
	UNUSED(dwBegin);
	UNUSED(bAppended);
	// stands at the end of file
	DWORD dwOffset = dwEnd - 9;
	BYTE* pBuffer = pStream->ReadBytes(9, dwOffset, false, true);

	// is it Lyrics 2 Tag
	if (memcmp("LYRICS200", pBuffer, 9) == 0)
		return new CLyrics3Tag(pStream, dwOffset, true);
	else if (memcmp("LYRICSEND", pBuffer, 9) == 0)
		return new CLyrics3Tag(pStream, dwOffset, false);
	
	return NULL;
}

CLyrics3Tag::CLyrics3Tag(CMPAStream* pStream, DWORD dwOffset, bool bVersion2) :
	CTag(pStream, _T("Lyrics3"), true, dwOffset)
{
	BYTE* pBuffer;
	if (bVersion2)
	{
		SetVersion(2);
		
		// look for size of tag (stands before dwOffset)
		dwOffset -= 6;
		pBuffer = pStream->ReadBytes(6, dwOffset, false); 

		// add null termination
		char szSize[7];
		memcpy(szSize, pBuffer, 6);
		szSize[6] = '\0';

		// convert string to integer
		m_dwSize = atoi(szSize); 
		m_dwOffset = dwOffset - m_dwSize;
		m_dwSize += 6 + 9;	// size must include size info and end string
	}
	else
	{
		SetVersion(1);

		// seek back 5100 bytes and look for LYRICSBEGIN
		m_dwOffset -= 5100;
		pBuffer = pStream->ReadBytes(11, m_dwOffset, false);

		while (memcmp("LYRICSBEGIN", pBuffer, 11) != 0)
		{
			if (dwOffset >= m_dwOffset)
				throw CMPAException(CMPAException::CorruptLyricsTag);
		}
		m_dwSize = (dwOffset - m_dwOffset) + 9;
	}
}


CLyrics3Tag::~CLyrics3Tag(void)
{
}
#endif
#endif