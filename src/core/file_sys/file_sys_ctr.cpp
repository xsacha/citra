// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "core/file_sys/file_sys_ctr.h"
#include "core/file_sys/volume.h"

namespace FileSys {

FileSystem_CTR::FileSystem_CTR(const Volume* volume) : FileSystem(volume), initialized(false) {
}

FileSystem_CTR::~FileSystem_CTR() {
    file_info_vector.clear();
}

u64 FileSystem_CTR::GetFileSize(const std::string& full_path) {
    if (!initialized)
        InitFileSystem();

    const FileInfo* file_info = FindFileInfo(full_path);

    if (file_info != nullptr && !file_info->IsDirectory())
        return file_info->size;

    return 0;
}

size_t FileSystem_CTR::GetFileList(std::vector<const FileInfo *>& filenames) {
    if (!initialized)
        InitFileSystem();

    if (filenames.size())
        PanicAlert("GetFileList : input list has contents?");
    filenames.clear();
    filenames.reserve(file_info_vector.size());

    for (auto& fileInfo : file_info_vector)
        filenames.push_back(&fileInfo);

    return file_info_vector.size();
}

u64 FileSystem_CTR::ReadFile(const std::string& full_path, u8* buffer, size_t max_buffer_size) {
    if (!initialized)
        InitFileSystem();

    const FileInfo* file_info = FindFileInfo(full_path);
    if (file_info == nullptr)
        return 0;

    if (file_info->size > max_buffer_size)
        return 0;

    DEBUG_LOG(FILESYS, "Filename: %s. Offset: %d. Size: %d", full_path.c_str(), file_info->offset,
        file_info->size);

    volume->Read(file_info->offset, file_info->size, buffer);
    return file_info->size;
}

bool FileSystem_CTR::IsValid() const {
    return true;
}

u32 FileSystem_CTR::Read32(u64 offset) const {
    u32 res = 0;
    volume->Read(offset, 4, (u8*)&res);
    return res;
}

const FileInfo* FileSystem_CTR::FindFileInfo(const std::string& full_path) {
    if (!initialized)
        InitFileSystem();

    for (auto& file_info : file_info_vector) {
        if (!strcasecmp(file_info.full_path.c_str(), full_path.c_str()))
            return &file_info;
    }

    return nullptr;
}

void FileSystem_CTR::InitFileSystem() {
    initialized = true;
}

} // namespace FileSys
