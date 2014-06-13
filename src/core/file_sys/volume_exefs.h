// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <string>
#include <vector>

#include "common/common.h"
#include "core/file_sys/volume.h"

namespace FileSys {

class BlobReader;

class Volume_ExeFS : public Volume {
public:
    Volume_ExeFS(BlobReader* reader);
    ~Volume_ExeFS();

    bool Read(u64 offset, u64 length, u8* buffer) const;
    u64 GetSize() const;

private:
    BlobReader* reader;
};

} // namespace FileSys
