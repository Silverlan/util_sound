/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "util_sound.hpp"
#include <fsys/filesystem.h>
#include <array>
#include <sharedutils/util_file.h>
#include <vorbis/vorbisfile.h>
#include <cstring>

// Sound duration for mp3 files
#if USOUND_MP3_SUPPORT_ENABLED == 1

#ifdef _WIN32
	template<class T,class V>
		BOOL MoveFile(const T&,const V&) {return FALSE;};
	#include <atlstr.h>
	#include <Windows.h>
	#include <mpaheader/mpafile.h>
	#include "MPAVFileStream.hpp"
#endif

#endif
//

static void get_ogg_file_data(VFilePtr f,vorbis_info **pInfo,OggVorbis_File &oggFile)
{
	ov_callbacks cb;
	cb.close_func = [](void *dataSource) -> int {
		auto &f = *static_cast<VFilePtr*>(dataSource);
		f.reset();
		return 0;
	};
	cb.read_func = [](void *ptr,size_t size,size_t nmemb,void *dataSource) -> size_t {
		auto &f = *static_cast<VFilePtr*>(dataSource);
		return f->Read(ptr,size *nmemb);
	};
	cb.seek_func = [](void *dataSource,ogg_int64_t offset,int whence) -> int {
		auto &f = *static_cast<VFilePtr*>(dataSource);
		f->Seek(offset,whence);
	return 0;
	};
	cb.tell_func = [](void *dataSource) -> long {
		auto f = *static_cast<VFilePtr*>(dataSource);
		return static_cast<long>(f->Tell());
	};
	ov_open_callbacks(&f,&oggFile,nullptr,0,cb);
	*pInfo = ov_info(&oggFile,-1);
}

static bool get_ogg_sound_duration(VFilePtr f,float &duration)
{
	duration = 0.f;
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	get_ogg_file_data(f,&pInfo,oggFile);
	auto bitrate = ov_bitrate(&oggFile,-1);
	if(bitrate == 0) // This shouldn't happen, but it does for some files. Reason unknown
		bitrate = pInfo->bitrate_nominal;
	auto numBytes = ov_raw_total(&oggFile,-1);
	ov_clear(&oggFile);
	auto d = bitrate /8.f;
	if(d == 0.f)
		return false;
	duration = float(numBytes) /d;
	return true;
}

bool util::sound::get_duration(const std::string path,float &duration)
{
	duration = 0.f;
	auto f = FileManager::OpenFile(path.c_str(),"rb");
	if(f == nullptr)
		return false;
	std::array<char,5> header;
	f->Read(header.data(),4);
	f->Seek(0);
	header.at(4) = '\0';
	if(strcmp(header.data(),"OggS") == 0)
		return get_ogg_sound_duration(f,duration);
	std::string ext;
	if(ufile::get_extension(path,&ext) == false)
		return false;
	if(ext == "mp3")
	{
#if USOUND_MP3_SUPPORT_ENABLED == 1
#ifdef _WIN32
		try
		{
			auto *stream = new CMPAVFileStream(f); // The stream closes the file automatically
			CMPAFile fInfo(stream); // Destructor gets rid of stream as well
			duration = static_cast<float>(fInfo.GetLengthSec());
			return true;
		}
		catch(CMPAException &e)
		{
			UNUSED(e);
			return false;
		}
#else
		return false;
#endif
#endif
	}
	else if(ext == "wav")
	{
		auto identifier = f->Read<int32_t>();
		if(identifier != 1179011410)
			return false;
		f->Seek(22);
		auto numChannels = f->Read<int16_t>();
		auto sampleRate = f->Read<int32_t>();
		f->Seek(34);
		auto bitsPerSample = f->Read<int16_t>();
		f->Seek(40);
		auto size = f->Read<int32_t>();
		duration = size /(sampleRate *numChannels *bitsPerSample /8.f);
		return true;
	}
	return false;
}
