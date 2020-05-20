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
#include ".\id3v1tag.h"

#define UNUSED(a) a

CID3V1Tag* CID3V1Tag::FindTag(CMPAStream* pStream, bool bAppended, DWORD dwBegin, DWORD dwEnd)
{
	UNUSED(dwBegin);
	if (bAppended)
	{
		// stands 128 byte before file end
		DWORD dwOffset = dwEnd - 128;
		BYTE* pBuffer = pStream->ReadBytes(3, dwOffset, false);
		if (memcmp("TAG", pBuffer, 3)==0)
			return new CID3V1Tag(pStream, dwOffset);
	}
	return NULL;
}


CID3V1Tag::CID3V1Tag(CMPAStream* pStream, DWORD dwOffset) :
	CTag(pStream, _T("ID3"), true, dwOffset, 128)
{
	dwOffset += 125;
	BYTE* pBuffer = pStream->ReadBytes(2, dwOffset, false);

	bool bIsV11 = false;
	// check if V1.1 tag (byte 125 = 0 and byte 126 != 0)
	if (pBuffer[0] == '\0' && pBuffer[1] != '\0')
		bIsV11 = true;
	SetVersion(1,(bIsV11?1:0));
}

CID3V1Tag::~CID3V1Tag(void)
{
}
#endif
#endif