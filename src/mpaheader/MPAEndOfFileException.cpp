#ifdef _WIN32
#include "util_sound_definitions.hpp"
#if USOUND_MP3_SUPPORT_ENABLED == 1
#include <Windows.h>
#include <atlstr.h>
#include "MPAEndOfFileException.h"

CMPAEndOfFileException::CMPAEndOfFileException(LPCTSTR szFile) :
	CMPAException(CMPAException::EndOfFile, szFile)
{
}

CMPAEndOfFileException::~CMPAEndOfFileException(void)
{
}
#endif
#endif
