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
#include "tag.h"

class CLAMETag :
	public CTag
{
public:
	static CLAMETag* CLAMETag::FindTag(CMPAStream* pStream, bool bAppended, DWORD dwBegin, DWORD dwEnd);
	~CLAMETag(void);

	CString m_strEncoder;
	DWORD m_dwLowpassFilterHz;
	BYTE m_bBitrate;	// in kbps
	BYTE m_bRevision;

	bool IsVBR() const;
	bool IsABR() const;
	bool IsCBR() const;
	LPCTSTR GetVBRInfo() const;
	bool IsSimpleTag() const { return m_bSimpleTag; };

private:
	CLAMETag(CMPAStream* pStream, bool bAppended, DWORD dwOffset);
	
	BYTE m_bVBRInfo;
	bool m_bSimpleTag;
	static LPCTSTR m_szVBRInfo[10];
};
