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

class CTag
{
public:
	DWORD GetOffset() const { return m_dwOffset; };
	DWORD GetEnd() const { return m_dwOffset + m_dwSize; };
	LPCTSTR GetName() const { return m_szName; };
	DWORD GetSize() const { return m_dwSize; };
	float GetVersion() const { return m_fVersion; };
	
	virtual ~CTag(void);
protected:
	CMPAStream* m_pStream;

	CTag(CMPAStream* pStream, LPCTSTR szName, bool bAppended, DWORD dwOffset = 0, DWORD dwSize = 0);

	DWORD m_dwOffset;	// beginning of tag
	DWORD m_dwSize;		// size of tag
	bool m_bAppended;	// true if at the end of file
	float m_fVersion;	// format x.yz
	LPTSTR m_szName;	// name of tag

	void SetVersion(BYTE bVersion1, BYTE bVersion2=0, BYTE bVersion3=0);
};
