// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#ifdef _WIN32

#include "common/atomic_win32.h"

#else

// GCC-compatible compiler assumed!
#include "common/atomic_gcc.h"

#endif
