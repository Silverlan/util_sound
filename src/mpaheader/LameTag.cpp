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
#include ".\lametag.h"

#define UNUSED(a) a

LPCTSTR CLAMETag::m_szVBRInfo[10] = 
{
	_T("Unknown"),
	_T("CBR"),
	_T("ABR"),
	_T("VBR1"),
	_T("VBR2"),
	_T("VBR3"),
	_T("VBR4"),
	_T("Reserved"),
	_T("CBR2Pass"),
	_T("ABR2Pass")
};


CLAMETag* CLAMETag::FindTag(CMPAStream* pStream, bool bAppended, DWORD dwBegin, DWORD dwEnd)
{
	UNUSED(dwEnd);
	// check for LAME Tag extension (always 120 bytes after XING ID)
	DWORD dwOffset = dwBegin + 120;

	BYTE* pBuffer = pStream->ReadBytes(9, dwOffset, false);
	if (memcmp(pBuffer, "LAME", 4) == 0)
		return new CLAMETag(pStream, bAppended, dwOffset);

	return NULL;
}

CLAMETag::CLAMETag(CMPAStream* pStream, bool bAppended, DWORD dwOffset) :
	CTag(pStream, _T("LAME"), bAppended, dwOffset)
{
	BYTE* pBuffer = pStream->ReadBytes(20, dwOffset, false);

	CString strVersion = CString((char*)pBuffer+4, 4);
	m_fVersion = (float)_tstof(strVersion);
	
	// LAME prior to 3.90 writes only a 20 byte encoder string
	if (m_fVersion < 3.90)
	{
		m_bSimpleTag = true;
		m_strEncoder = CString((char*)pBuffer, 20);
	}
	else
	{
		m_bSimpleTag = false;
		m_strEncoder = CString((char*)pBuffer, 9);
		dwOffset += 9;

		// cut off last period
		if (m_strEncoder[8] == '.')
			m_strEncoder.Delete(8);

		// version information
		BYTE bInfoAndVBR = *(pStream->ReadBytes(1, dwOffset));

		// revision info in 4 MSB
		m_bRevision = bInfoAndVBR & 0xF0;
		// invalid value
		if (m_bRevision == 15)
			throw NULL;

		// VBR info in 4 LSB
		m_bVBRInfo = bInfoAndVBR & 0x0F;

		// lowpass information
		m_dwLowpassFilterHz = *(pStream->ReadBytes(1, dwOffset)) * 100;

		// skip replay gain values
		dwOffset += 8;

		// skip encoding flags
		dwOffset += 1;

		// average bitrate for ABR, bitrate for CBR and minimal bitrat for VBR [in kbps]
		// 255 means 255 kbps or more
		m_bBitrate = *(pStream->ReadBytes(1, dwOffset)); 
	}
}

CLAMETag::~CLAMETag(void)
{
}

bool CLAMETag::IsVBR() const
{
	if (m_bVBRInfo >= 3 && m_bVBRInfo <= 6)
		return true;
	return false;
}

bool CLAMETag::IsABR() const
{
	if (m_bVBRInfo == 2 || m_bVBRInfo == 9)
		return true;
	return false;
}

bool CLAMETag::IsCBR() const
{
	if (m_bVBRInfo == 1 || m_bVBRInfo == 8)
		return true;
	return false;
}

LPCTSTR CLAMETag::GetVBRInfo() const
{
	if (m_bVBRInfo > 9)
		return m_szVBRInfo[0];

	return m_szVBRInfo[m_bVBRInfo];
}
#endif
#endif
