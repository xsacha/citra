// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "core/file_sys/blob.h"
#include "core/file_sys/volume_exefs.h"

namespace FileSys {

Volume_ExeFS::Volume_ExeFS(BlobReader* reader)  : reader(reader) {
}

Volume_ExeFS::~Volume_ExeFS() {
    delete reader;
    reader = nullptr;
}

bool Volume_ExeFS::Read(u64 offset, u64 length, u8* buffer) const{
    if (reader == nullptr)
        return false;

    return reader->Read(offset, length, buffer);
}

u64 Volume_ExeFS::GetSize() const {
    if (reader)
        return reader->GetDataSize();
    else
        return 0;
}

} // namespace FileSys
