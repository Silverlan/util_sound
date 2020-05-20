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
#include <Windows.h>
#include <atlstr.h>
#include ".\tags.h"
#include "mpaexception.h"

#include "id3v1tag.h"
#include "id3v2tag.h"
#include "lyrics3tag.h"
#include "apetag.h"
#include "musicmatchtag.h"

const CTags::FindTagFunctionPtr CTags::m_appendedTagFactories[] = 
{
	(CTags::FindTagFunctionPtr)&CID3V1Tag::FindTag,
	(CTags::FindTagFunctionPtr)&CMusicMatchTag::FindTag,
	(CTags::FindTagFunctionPtr)&CID3V2Tag::FindTag,
	(CTags::FindTagFunctionPtr)&CLyrics3Tag::FindTag,
	(CTags::FindTagFunctionPtr)&CAPETag::FindTag
};

const CTags::FindTagFunctionPtr CTags::m_prependedTagFactories[] = 
{
	(CTags::FindTagFunctionPtr)&CID3V2Tag::FindTag,
	(CTags::FindTagFunctionPtr)&CAPETag::FindTag
};



CTags::CTags(CMPAStream* pStream) :
	m_dwBegin(0), m_dwEnd(pStream->GetSize())
{
	// all appended tags
	/////////////////////////////

	while (FindAppendedTag(pStream))
		;

	// all prepended tags
	/////////////////////////////
	while (FindPrependedTag(pStream))
		;
	
}

CTags::~CTags(void)
{
	for (unsigned int n=0; n<m_Tags.size(); n++)
	{
		delete m_Tags[n];
	}
}

bool CTags::FindPrependedTag(CMPAStream* pStream)
{
	for (int i=0; i < NUMBER_OF_ELEMENTS(m_prependedTagFactories); i++) 
	{
		if (FindTag(m_prependedTagFactories[i], pStream, false))
			return true;
	}
	return false;
}

bool CTags::FindAppendedTag(CMPAStream* pStream)
{
	for (int i=0; i < NUMBER_OF_ELEMENTS(m_appendedTagFactories); i++) 
	{
		if (FindTag(m_appendedTagFactories[i], pStream, true))
			return true;
	}
	return false;
}

bool CTags::FindTag(FindTagFunctionPtr pFindTag, CMPAStream* pStream, bool bAppended)
{
	try
	{
		CTag* pTag = pFindTag(pStream, bAppended, m_dwBegin, m_dwEnd);
		if (pTag)
		{
			if (bAppended)
				m_dwEnd = pTag->GetOffset();
			else
				m_dwBegin = pTag->GetEnd();
			m_Tags.push_back(pTag);
			return true;
		}
	}
	catch(CMPAException& Exc)
	{
		Exc.ShowError();
	}
	return false;
}

CTag* CTags::GetNextTag(unsigned int& nIndex) const
{
	CTag* pTag;
	if (nIndex < m_Tags.size())
		pTag = m_Tags[nIndex];
	else
		pTag = NULL;

	nIndex++;
	return pTag;
}
#endif
#endif
