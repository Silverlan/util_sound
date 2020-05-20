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
#include "mpaexception.h"
#include "mpafilestream.h"
#include "mpaframe.h"
#include "tags.h"

class CMPAFile
{
public:
	CMPAFile(LPCTSTR szFile);
	CMPAFile(CMPAStream* pStream);
	~CMPAFile(void);

	DWORD GetBegin() const { return m_pTags->GetBegin(); };
	DWORD GetEnd() const { return m_pTags->GetEnd(); };
	DWORD GetFileSize() const { return (GetEnd() - GetBegin()); };
	DWORD GetLengthSec() const { return (GetFileSize() / m_dwBytesPerSec); };
	
	enum GetType
	{
		First,
		Last,
		Next,
		Prev,
		Resync
	};

	CMPAFrame* GetFrame(GetType Type, CMPAFrame* pFrame = NULL, bool bDeleteOldFrame = true, DWORD dwOffset = 0);
	
private:
	void CalcBytesPerSec();

	CMPAStream* m_pStream;
	DWORD m_dwBytesPerSec;
	
public:	
	CTags* m_pTags;					// contain list of present tags
	CMPAFrame* m_pFirstFrame;	// always first frame
	CVBRHeader* m_pVBRHeader;		// XING or VBRI or NULL
};
