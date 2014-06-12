// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "common/common.h"
#include "core/file_sys/file_sys.h"

namespace FileSys {

class Volume;

class FileSystem_CTR : public FileSystem {
public:
    FileSystem_CTR(const Volume* volume);
    virtual ~FileSystem_CTR();

    virtual bool IsValid() const override;
    virtual u64 GetFileSize(const std::string& full_path) override;
    virtual size_t GetFileList(std::vector<const FileInfo *>& filenames) override;
    virtual u64 ReadFile(const std::string& full_path, u8* buffer, size_t max_buffer_size) override;

private:
    bool initialized;

    std::vector <FileInfo> file_info_vector;
    u32 Read32(u64 offset) const;
    const FileInfo* FindFileInfo(const std::string& full_path);
    void InitFileSystem();
};

} // namespace FileSys
