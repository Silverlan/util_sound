/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __UTIL_SOUND_DEFINITIONS_HPP__
#define __UTIL_SOUND_DEFINITIONS_HPP__

#define USOUND_MP3_SUPPORT_ENABLED 1

#ifdef USND_STATIC
#define DLLUSND
#elif USND_DLL
#ifdef __linux__
#define DLLUSND __attribute__((visibility("default")))
#else
#define DLLUSND __declspec(dllexport)
#endif
#else
#ifdef __linux__
#define DLLUSND
#else
#define DLLUSND __declspec(dllimport)
#endif
#endif

#endif
