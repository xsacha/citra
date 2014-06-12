// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "common/common.h"

namespace FileSys {

class Volume : NonCopyable {
public:
    Volume() {}
    virtual ~Volume() {}

    virtual bool Read(u64 offset, u64 length, u8* buffer) const = 0;
    virtual u64 GetSize() const = 0;
};

} // namespace FileSys
