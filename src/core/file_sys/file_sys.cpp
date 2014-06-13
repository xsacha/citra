// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "core/file_sys/file_sys.h"
#include "core/file_sys/file_sys_ctr.h"

namespace FileSys {

FileSystem::FileSystem(const Volume*volume) : volume(volume) {
}

FileSystem::~FileSystem() {
}

FileSystem* CreateFileSystem(const Volume* volume) {
    FileSystem* file_system = new FileSystem_CTR(volume);

    if (!file_system)
        return nullptr;

    if (!file_system->IsValid()) {
        delete file_system;
        file_system = nullptr;
    }

    return file_system;
}

} // namespace