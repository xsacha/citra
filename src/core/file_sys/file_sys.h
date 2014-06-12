// Copyright 2014 Dolphin Emulator Project / Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#include "common/common.h"

namespace FileSys {

class Volume;

// File info of an FST entry
struct FileInfo {
    u64 name_offset;
    u64 offset;
    u64 size;
    std::string full_path;

    bool IsDirectory() const { return (name_offset & 0xFF000000) != 0; }

    FileInfo() : name_offset(0), offset(0), size(0) { }

    FileInfo(const FileInfo& rhs) : name_offset(rhs.name_offset),
        offset(rhs.offset), size(rhs.size), full_path(rhs.full_path) { }
};

class FileSystem : NonCopyable {
public:
    FileSystem(const Volume* volume);

    virtual ~FileSystem();
    virtual bool IsValid() const = 0;
    virtual size_t GetFileList(std::vector<const FileInfo *>& filenames) = 0;
    virtual u64 GetFileSize(const std::string& full_path) = 0;
    virtual u64 ReadFile(const std::string& full_path, u8* buffer, size_t max_buffer_size) = 0;

    virtual const Volume* GetVolume() const { return volume; }

protected:
    const Volume* volume;

};

FileSystem* CreateFileSystem(const Volume *_rVolume);

} // namespace FileSys
