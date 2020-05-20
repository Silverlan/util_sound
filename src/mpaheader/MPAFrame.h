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
#include "mpastream.h"
#include "mpaheader.h"
#include "vbrheader.h"
#include "vbriheader.h"
#include "xingheader.h"

class CMPAFrame
{
public:
	CMPAFrame(CMPAStream* pStream, DWORD& dwOffset, bool bFindSubsequentFrame, bool bExactOffset, bool bReverse, CMPAHeader* pCompareHeader);
	~CMPAFrame(void);

	CVBRHeader* FindVBRHeader() const;
	
	DWORD GetSubsequentHeaderOffset() const { return m_dwOffset + m_dwFrameSize; };
	bool CheckCRC() const;
	bool IsLast() const { return m_bIsLast; };
	
public:
	CMPAHeader* m_pHeader;
	CMPAStream* m_pStream;

	DWORD m_dwOffset;	// offset in bytes where frame begins
	DWORD m_dwFrameSize;// calculated frame size
	
private:
	static const DWORD m_dwProtectedBitsLayer2[5][2];
	static WORD CalcCRC16(BYTE* pBuffer, DWORD dwSize);
	bool m_bIsLast;		// true, if it is last frame
};
