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
#include ".\xingheader.h"

// for XING VBR Header flags
#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

// offset of XING header: after side information in Layer III


CXINGHeader* CXINGHeader::FindHeader(const CMPAFrame* pFrame)
{
	// where does VBR header begin (XING)
	DWORD dwOffset = pFrame->m_dwOffset + MPA_HEADER_SIZE + pFrame->m_pHeader->GetSideInfoSize();// + (pFrame->m_pHeader->m_bCRC?2:0);

	// check for XING header first
	if (!CheckID(pFrame->m_pStream, dwOffset, 'X', 'i', 'n', 'g') && !CheckID(pFrame->m_pStream, dwOffset,'I', 'n', 'f', 'o'))
		return NULL;
	
	return new CXINGHeader(pFrame, dwOffset);
}

CXINGHeader::CXINGHeader(const CMPAFrame* pFrame, DWORD dwOffset) :
	CVBRHeader(pFrame->m_pStream, dwOffset)
{
	/* XING VBR-Header

	 size	description
	 4		'Xing' or 'Info'
	 4		flags (indicates which fields are used)
	 4		frames (optional)
	 4		bytes (optional)
	 100	toc (optional)
	 4		a VBR quality indicator: 0=best 100=worst (optional)

	*/
	
	// XING ID already checked at this point
	dwOffset += 4;
	DWORD dwFlags;

	// get flags (mandatory in XING header)
	dwFlags = m_pStream->ReadBEValue(4, dwOffset); 

	// extract total number of frames in file
	if(dwFlags & FRAMES_FLAG)
		m_dwFrames = m_pStream->ReadBEValue(4, dwOffset);

	// extract total number of bytes in file
	if(dwFlags & BYTES_FLAG) 
		m_dwBytes = m_pStream->ReadBEValue(4, dwOffset);
	
	// extract TOC (for more accurate seeking)
	if (dwFlags & TOC_FLAG) 
	{
		m_dwTableSize = 100;
		m_pnToc = new int[m_dwTableSize];

		if (m_pnToc)
		{
			for(DWORD i=0;i<m_dwTableSize;i++)
				m_pnToc[i] = *m_pStream->ReadBytes(1, dwOffset);
		}
	}

	if (dwFlags & VBR_SCALE_FLAG)
		m_dwQuality = m_pStream->ReadBEValue(4, dwOffset);

	m_pLAMETag = CLAMETag::FindTag(m_pStream, true, m_dwOffset, 0);
}

CXINGHeader::~CXINGHeader(void)
{
	if (m_pLAMETag)
		delete m_pLAMETag;
}

DWORD CXINGHeader::SeekPosition(float& fPercent) const
{
	// interpolate in TOC to get file seek point in bytes
	int a;
	float fa, fb, fx;

	a = (int)fPercent;
	fa = (float)m_pnToc[a];
	
	if (a < 99) 
	{
		fb = (float)m_pnToc[a+1];
	}
	else 
	{
		fb = 256.0f;
	}

	fx = fa + (fb-fa)*(fPercent-a);

	return (int)((1.0f/256.0f)*fx*m_dwBytes); 
}
#endif
#endif