/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

module;

#include "definitions.hpp"
#include <string>

export module pragma.audio.util;

export namespace pragma::audio::util {
	DLLUSND bool get_duration(const std::string path, float &duration);
};
