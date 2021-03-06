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

#include "mpastream.h"
#include "mpaexception.h"

#include <windows.h>	// for CreateFile, CloseHandle, ...

// constructor
CMPAStream::CMPAStream(LPCTSTR szFilename)
{
	// save filename
	m_szFile = _tcsdup(szFilename);
}

CMPAStream::~CMPAStream(void)
{
	free(m_szFile);
}

DWORD CMPAStream::ReadLEValue(DWORD dwNumBytes, DWORD& dwOffset, bool bMoveOffset) const
{
	_ASSERTE(dwNumBytes > 0);
	_ASSERTE(dwNumBytes <= 4);	// max 4 byte

	BYTE* pBuffer = ReadBytes(dwNumBytes, dwOffset, bMoveOffset);

	DWORD dwResult = 0;

	// little endian extract (least significant byte first) (will work on little and big-endian computers)
	DWORD dwNumByteShifts = 0;

	for (DWORD n=0; n < dwNumBytes; n++)
	{
		dwResult |= pBuffer[n] << 8 * dwNumByteShifts++;                                                          
	}
	
	return dwResult;
}

// convert from big endian to native format (Intel=little endian) and return as DWORD (32bit)
DWORD CMPAStream::ReadBEValue(DWORD dwNumBytes, DWORD& dwOffset,  bool bMoveOffset) const
{	
	_ASSERTE(dwNumBytes > 0);
	_ASSERTE(dwNumBytes <= 4);	// max 4 byte

	BYTE* pBuffer = ReadBytes(dwNumBytes, dwOffset, bMoveOffset);

	DWORD dwResult = 0;

	// big endian extract (most significant byte first) (will work on little and big-endian computers)
	DWORD dwNumByteShifts = dwNumBytes - 1;

	for (DWORD n=0; n < dwNumBytes; n++)
	{
		dwResult |= pBuffer[n] << 8*dwNumByteShifts--; // the bit shift will do the correct byte order for you                                                           
	}
	
	return dwResult;
}
#endif
#endif
