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
#include "lametag.h"


class CXINGHeader :
	public CVBRHeader
{
public:
	static CXINGHeader* FindHeader(const CMPAFrame* pFrame);

	CXINGHeader(const CMPAFrame* pFrame, DWORD dwOffset);
	virtual ~CXINGHeader(void);

	virtual DWORD SeekPosition(float& fPercent) const;

	CLAMETag* m_pLAMETag;
};
