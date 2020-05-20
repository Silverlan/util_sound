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

class CMPAFileStream :
	public CMPAStream
{
public:
	CMPAFileStream(LPCTSTR szFilename);
	CMPAFileStream(LPCTSTR szFilename, HANDLE hFile);
	virtual ~CMPAFileStream(void);

private:
	static const DWORD INIT_BUFFERSIZE;
	HANDLE m_hFile;
	bool m_bMustReleaseFile;

	// concerning read-buffer
	mutable BYTE* m_pBuffer;
	mutable DWORD m_dwOffset;	// offset (beginning if m_pBuffer)
	mutable DWORD m_dwBufferSize;

	void Init();
	DWORD Read(LPVOID pData, DWORD dwOffset, DWORD dwSize) const;
	bool FillBuffer(DWORD dwOffset, DWORD dwSize, bool bReverse) const;
	void SetPosition(DWORD dwOffset) const;

protected:
	// methods for file access (must be implemented by all derived classes)
	
	virtual BYTE* ReadBytes(DWORD dwSize, DWORD& dwOffset, bool bMoveOffset = true, bool bReverse = false) const;
	virtual DWORD GetSize() const;
};
