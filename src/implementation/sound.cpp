/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

module;

#include "definitions.hpp"
#include <fsys/filesystem.h>
#include <fsys/ifile.hpp>
#include <array>
#include <vorbis/vorbisfile.h>
#include <cstring>
#if USOUND_MP3_SUPPORT_ENABLED == 1
#include <mpafilestream.hpp>
#include <mpafile.hpp>
#endif

module pragma.audio.util;

#if USOUND_MP3_SUPPORT_ENABLED == 1
struct MPHFile : public CMPAIFile {
	MPHFile(std::unique_ptr<ufile::IFile> &&f) : m_file {std::move(f)} {}
	virtual ~MPHFile() = default;
	virtual size_t Read(void *data, size_t size) override { return m_file->Read(data, size); }
	virtual void Seek(size_t offset, Whence whence = Whence::Set) override { m_file->Seek(offset, static_cast<ufile::IFile::Whence>(whence)); }
	virtual size_t GetSize() override { return m_file->GetSize(); }
  private:
	std::unique_ptr<ufile::IFile> m_file;
};
#endif

static void get_ogg_file_data(VFilePtr &f, vorbis_info **pInfo, OggVorbis_File &oggFile)
{
	ov_callbacks cb;
	cb.close_func = [](void *dataSource) -> int {
		auto &f = *static_cast<VFilePtr *>(dataSource);
		f.reset();
		return 0;
	};
	cb.read_func = [](void *ptr, size_t size, size_t nmemb, void *dataSource) -> size_t {
		auto &f = *static_cast<VFilePtr *>(dataSource);
		return f->Read(ptr, size * nmemb);
	};
	cb.seek_func = [](void *dataSource, ogg_int64_t offset, int whence) -> int {
		auto &f = *static_cast<VFilePtr *>(dataSource);
		f->Seek(offset, whence);
		return 0;
	};
	cb.tell_func = [](void *dataSource) -> long {
		auto f = *static_cast<VFilePtr *>(dataSource);
		return static_cast<long>(f->Tell());
	};
	ov_open_callbacks(&f, &oggFile, nullptr, 0, cb);
	*pInfo = ov_info(&oggFile, -1);
}

static bool get_ogg_sound_duration(VFilePtr f, float &duration)
{
	duration = 0.f;
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	get_ogg_file_data(f, &pInfo, oggFile);
	auto bitrate = ov_bitrate(&oggFile, -1);
	if(bitrate == 0) // This shouldn't happen, but it does for some files. Reason unknown
		bitrate = pInfo->bitrate_nominal;
	auto numBytes = ov_raw_total(&oggFile, -1);
	ov_clear(&oggFile);
	auto d = bitrate / 8.f;
	if(d == 0.f)
		return false;
	duration = float(numBytes) / d;
	return true;
}

bool pragma::audio::util::get_duration(const std::string path, float &duration)
{
	duration = 0.f;
	auto f = FileManager::OpenFile(path.c_str(), "rb");
	if(f == nullptr)
		return false;
	std::array<char, 5> header;
	f->Read(header.data(), 4);
	f->Seek(0);
	header.at(4) = '\0';
	if(strcmp(header.data(), "OggS") == 0)
		return get_ogg_sound_duration(f, duration);
	std::string ext;
	if(ufile::get_extension(path, &ext) == false)
		return false;
	if(ext == "mp3") {
#if USOUND_MP3_SUPPORT_ENABLED == 1
		auto mphFile = std::make_unique<MPHFile>(std::make_unique<fsys::File>(f));
		try {
			auto *fstream = new CMPAFileStream {std::move(mphFile)}; // Will be deleted by destructor of CMPAFile
			CMPAFile fInfo {fstream};
			duration = static_cast<float>(fInfo.GetLengthSec());
			return true;
		}
		catch(CMPAException &e) {
			UNUSED(e);
			return false;
		}
#endif
	}
	else if(ext == "wav") {
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
		duration = size / (sampleRate * numChannels * bitsPerSample / 8.f);
		return true;
	}
	return false;
}
