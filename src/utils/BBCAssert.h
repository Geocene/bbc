//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

#include <cassert>

#include "BBCMacros.h"

#ifdef BBC_DEBUG
#define BBC_ASSERT(x) BBC_MACRO_BLOCK( assert(x); )
#else
#define BBC_ASSERT(...)
#endif

#define BBC_ASSERT_R(x) assert(x)

