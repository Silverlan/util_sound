/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UTIL_SOUND_HPP__
#define __UTIL_SOUND_HPP__

#include "util_sound_definitions.hpp"
#include <string>

namespace util
{
	namespace sound
	{
		bool get_duration(const std::string path,float &duration);
	};
};

#endif
