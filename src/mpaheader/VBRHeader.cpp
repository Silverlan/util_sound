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
#include "mpaframe.h"	// also includes vbrheader.h

#include "xingheader.h"
#include "vbriheader.h"

// first test with this static method, if it does exist
CVBRHeader* CVBRHeader::FindHeader(const CMPAFrame* pFrame)
{
	_ASSERTE(pFrame);
	CVBRHeader* pVBRHeader = NULL;

	pVBRHeader = CXINGHeader::FindHeader(pFrame);
	if (!pVBRHeader)
		pVBRHeader = CVBRIHeader::FindHeader(pFrame);
	
	return pVBRHeader;
}

CVBRHeader::CVBRHeader(CMPAStream* pStream, DWORD dwOffset) :
	m_pStream(pStream), m_pnToc(NULL), m_dwOffset(dwOffset), m_dwFrames(0), m_dwBytes(0), m_dwQuality(0), m_dwTableSize(0)
{
}

bool CVBRHeader::CheckID(CMPAStream* pStream, DWORD dwOffset, char ch0, char ch1, char ch2, char ch3)
{
	BYTE* pBuffer = pStream->ReadBytes(4, dwOffset, false);
	if (pBuffer[0] == ch0 && pBuffer[1] == ch1 && pBuffer[2] == ch2 && pBuffer[3] == ch3)
		return true;
	return false;
}

/*
// currently not used
bool CVBRHeader::ExtractLAMETag( DWORD dwOffset )
{
	// LAME ID found?
	if( !CheckID( m_pMPAFile, dwOffset, 'L', 'A', 'M', 'E' ) && !CheckID( m_pMPAFile, dwOffset, 'G', 'O', 'G', 'O' ) )
		return false;

	return true;
}*/



CVBRHeader::~CVBRHeader(void)
{
	if (m_pnToc)
		delete[] m_pnToc;
}

// get byte position for percentage value (fPercent) of file
bool CVBRHeader::SeekPosition(float& fPercent, DWORD& dwSeekPoint) const
{
	if (!m_pnToc || m_dwBytes == 0)
		return false;

	// check range of fPercent
	if (fPercent < 0.0f)   
		fPercent = 0.0f;
	if (fPercent > 99.0f) 
		fPercent = 99.0f;

	dwSeekPoint = SeekPosition(fPercent);
	return true;
}
#endif
#endif