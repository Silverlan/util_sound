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
#include "MPAVFileStream.hpp"
#include <mpaheader/mpaexception.h>
#include <mpaheader/mpaendoffileexception.h>
#include <fsys/filesystem.h>

// 1KB is inital buffersize
const DWORD CMPAVFileStream::INIT_BUFFERSIZE = 1024;	

CMPAVFileStream::CMPAVFileStream(VFilePtr f) :
	CMPAStream(""),m_dwOffset(0),m_hFile(f),m_bMustReleaseFile(true)
{
	Init();
}

void CMPAVFileStream::Init() 
{
	m_dwBufferSize = INIT_BUFFERSIZE;
	// fill buffer for first time
	m_pBuffer = new BYTE[m_dwBufferSize];
	FillBuffer(m_dwOffset, m_dwBufferSize, false);
}

CMPAVFileStream::~CMPAVFileStream(void)
{
	if (m_pBuffer)
		delete[] m_pBuffer;
	
	// close file
	if (m_bMustReleaseFile)
		m_hFile.reset();
}

// VC++6 doesn't contain this definition
#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

// set file position
void CMPAVFileStream::SetPosition(DWORD dwOffset) const
{
	// convert from unsigned DWORD to signed 64bit long
	m_hFile->Seek(dwOffset);
}


BYTE* CMPAVFileStream::ReadBytes(DWORD dwSize, DWORD& dwOffset, bool bMoveOffset, bool bReverse) const
{
	// enough bytes in buffer, otherwise read from file
	if (m_dwOffset > dwOffset || ( ((int)((m_dwOffset + m_dwBufferSize) - dwOffset)) < (int)dwSize))
	{
		if (!FillBuffer(dwOffset, dwSize, bReverse)) 
		{
			throw CMPAEndOfFileException(m_szFile);
		}
	}

	BYTE* pBuffer = m_pBuffer + (dwOffset-m_dwOffset);
	if (bMoveOffset)
		dwOffset += dwSize;
	
	return pBuffer;
}

DWORD CMPAVFileStream::GetSize() const
{
	return static_cast<DWORD>(m_hFile->GetSize());
}

// fills internal buffer, returns false if EOF is reached, otherwise true. Throws exceptions
bool CMPAVFileStream::FillBuffer(DWORD dwOffset, DWORD dwSize, bool bReverse) const
{
	// calc new buffer size
	if (dwSize > m_dwBufferSize)
	{
        m_dwBufferSize = dwSize;
		
		// release old buffer 
		delete[] m_pBuffer;

		// reserve new buffer
		m_pBuffer = new BYTE[m_dwBufferSize];
	}	

	if (bReverse)
	{
		if (dwOffset + dwSize < m_dwBufferSize)
			dwOffset = 0;
		else
			dwOffset = dwOffset + dwSize - m_dwBufferSize;
	}

	// read <m_dwBufferSize> bytes from offset <dwOffset>
	m_dwBufferSize = Read(m_pBuffer, dwOffset, m_dwBufferSize);

	// set new offset
	m_dwOffset = dwOffset;

	if (m_dwBufferSize < dwSize)
		return false;

	return true;
}

// read from file, return number of bytes read
DWORD CMPAVFileStream::Read(LPVOID pData, DWORD dwOffset, DWORD dwSize) const
{
	DWORD dwBytesRead = 0;
	
	// set position first
	SetPosition(dwOffset);

	dwBytesRead = static_cast<DWORD>(m_hFile->Read(pData,dwSize));
	
	return dwBytesRead;
}
#endif
#endif
