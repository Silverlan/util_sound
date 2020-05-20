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

// exception class
class CMPAException
{
public:
	
	enum ErrorIDs
	{
		ErrOpenFile,
		ErrSetPosition,
		ErrReadFile,
		NoVBRHeader,
		IncompleteVBRHeader,
		NoFrameInTolerance,
		EndOfFile,
		HeaderCorrupt,
		FreeBitrate,
		IncompatibleHeader,
		CorruptLyricsTag,
		NumIDs			// this specifies the total number of possible IDs
	};

	CMPAException(ErrorIDs ErrorID, LPCTSTR szFile = NULL, LPCTSTR szFunction = NULL, bool bGetLastError=false);
	// copy constructor (necessary because of LPSTR members)
	CMPAException(const CMPAException& Source);
	virtual ~CMPAException(void);

	ErrorIDs GetErrorID() const { return m_ErrorID; };
	LPCTSTR GetErrorDescription();
	void ShowError();

private:
	ErrorIDs m_ErrorID;
	bool m_bGetLastError;
	LPTSTR m_szFunction;
	LPTSTR m_szFile;
	LPTSTR m_szErrorMsg;

	static LPCTSTR m_szErrors[CMPAException::NumIDs];
};
