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
#include "mpaframe.h"
#include "vbrheader.h"


class CVBRIHeader :
	public CVBRHeader
{
public:
	static CVBRIHeader* FindHeader(const CMPAFrame* pFrame);

	CVBRIHeader(const CMPAFrame* pFrame, DWORD dwOffset);
	virtual ~CVBRIHeader(void);

	virtual DWORD SeekPosition(float& fPercent) const;
	DWORD SeekPositionByTime(float fEntryTimeMS) const;

	// these values exist only in VBRI headers
	float m_fDelay;	
	DWORD m_dwTableScale;	// for seeking
	DWORD m_dwBytesPerEntry;
    DWORD m_dwFramesPerEntry;
	DWORD m_dwVersion;
private:
	DWORD m_dwLengthSec;
};
