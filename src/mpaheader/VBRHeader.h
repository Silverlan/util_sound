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

#pragma once

// class CMPAFrame must be included first and must be known here
class CMPAFrame;

class CVBRHeader
{
public:
	static CVBRHeader* FindHeader(const CMPAFrame* pFrame);
	virtual ~CVBRHeader(void);
	bool SeekPosition(float& fPercent, DWORD& dwSeekPoint) const;

	DWORD m_dwBytes;		// total number of bytes
	DWORD m_dwFrames;		// total number of frames

protected:	
	CVBRHeader(CMPAStream* pStream, DWORD dwOffset);

	static bool CheckID(CMPAStream* pStream, DWORD dwOffset, char ch0, char ch1, char ch2, char ch3);
	virtual DWORD SeekPosition(float& fPercent) const = 0;
	CMPAStream* m_pStream;

public:	
	DWORD m_dwOffset;
	DWORD m_dwQuality;		// quality (0..100)
	int* m_pnToc;			// TOC points for seeking (must be freed)
	DWORD m_dwTableSize;	// size of table (number of entries)	


	
};
