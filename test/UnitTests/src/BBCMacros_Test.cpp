//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s

#define BBC_MACRO_BLOCK(x) do {x} while (false)

#if DEBUG
#ifndef BBC_DEBUG
#define BBC_DEBUG
#endif

