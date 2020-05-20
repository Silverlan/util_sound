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

#define NUMBER_OF_ELEMENTS(X) (sizeof X / sizeof X[0])


#include <vector>

#include "mpastream.h"
#include "tag.h"

class CTags
{
public:
	CTags(CMPAStream* pStream);
	~CTags(void);

	CTag* GetNextTag(unsigned int& nIndex) const;
	template <class TagClass> bool FindTag(TagClass*& pTag) const;

	// get begin offset after prepended tags
	DWORD GetBegin() const { return m_dwBegin; };
	// get end offset before appended tags
	DWORD GetEnd() const { return m_dwEnd; };

	
private:
	bool FindAppendedTag(CMPAStream* pStream);
	bool FindPrependedTag(CMPAStream* pStream);

	// definition of function pointer type
	typedef CTag* (*FindTagFunctionPtr) (CMPAStream*, bool, DWORD, DWORD);
	bool FindTag(FindTagFunctionPtr pFindTag, CMPAStream* pStream, bool bAppended);

	std::vector <CTag*> m_Tags;
	DWORD m_dwBegin, m_dwEnd;
	static const FindTagFunctionPtr m_appendedTagFactories[];
	static const FindTagFunctionPtr m_prependedTagFactories[];
};


// you need to compile with runtime information to use this method
template <class TagClass>
bool CTags::FindTag(TagClass*& pTag) const
{
	for (unsigned int nIndex = 0; nIndex < m_Tags.size(); nIndex++)
	{
		pTag = dynamic_cast<TagClass*>(m_Tags[nIndex]);
		if (pTag)
			return true;
	}
	return false;
}
