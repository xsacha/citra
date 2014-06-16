// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cstddef>
#include <cstring>
#include <string>

#include "common/common.h"
#include "common/file_util.h"

#include "core/file_sys/blob.h"

namespace FileSys {

// Provides caching and split-operation-to-block-operations facilities.
// Used for compressed blob reading and direct drive reading.

void SectorReader::SetSectorSize(int block_size) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i] = new u8[block_size];
        cache_tags[i] = (u64)(s64)-1;
    }
    this->block_size = block_size;
}

SectorReader::~SectorReader() {
    for (u8*& block : cache) {
        delete[] block;
    }
}

const u8 *SectorReader::GetBlockData(u64 block_num) {
    // TODO : Expand usage of the cache to more than one block :P
    if (cache_tags[0] == block_num) {
        return cache[0];
    } else {
        GetBlock(block_num, cache[0]);
        cache_tags[0] = block_num;
        return cache[0];
    }
}

bool SectorReader::Read(u64 offset, u64 size, u8* out_ptr) {
    u64 startingBlock = offset / block_size;
    u64 remain = size;

    int position_in_block = (int)(offset % block_size);
    u64 block = startingBlock;

    while (remain > 0) {
        // Check if we are ready to do a large block read. > instead of >= so we don't bother if remain is only one block.
        if (position_in_block == 0 && remain > (u64)block_size) {
            u64 num_blocks = remain / block_size;
            ReadMultipleAlignedBlocks(block, num_blocks, out_ptr);
            block += num_blocks;
            out_ptr += num_blocks * block_size;
            remain -= num_blocks * block_size;
            continue;
        }

        u32 to_copy = block_size - position_in_block;
        if (to_copy >= remain) {
            const u8* data = GetBlockData(block);
            if (!data)
                return false;

            // Yay, we are done!
            memcpy(out_ptr, data + position_in_block, (size_t)remain);
            return true;
        } else {
            const u8* data = GetBlockData(block);
            if (!data)
                return false;

            memcpy(out_ptr, data + position_in_block, to_copy);
            out_ptr += to_copy;
            remain -= to_copy;
            position_in_block = 0;
            block++;
        }
    }

    return true;
}

bool SectorReader::ReadMultipleAlignedBlocks(u64 block_num, u64 num_blocks, u8 *out_ptr) {
    for (u64 i = 0; i < num_blocks; i++) {
        const u8 *data = GetBlockData(block_num + i);
        if (!data)
            return false;
        memcpy(out_ptr + i * block_size, data, block_size);
    }

    return true;
}

BlobReader* CreateBlobReader(const std::string& filename) {
    if (!File::Exists(filename))
        return nullptr;

    //if (IsWbfsBlob(filename))
    //    return WbfsFileReader::Create(filename);

    //if (IsCompressedBlob(filename))
    //    return CompressedBlobReader::Create(filename);

    //if (IsCISOBlob(filename))
    //    return CISOFileReader::Create(filename);

    //// Still here? Assume plain file - since we know it exists due to the File::Exists check above.
    //return PlainFileReader::Create(filename);

    return nullptr;
}

}  // namespace FileSys
