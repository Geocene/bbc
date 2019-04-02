//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

/// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
///
#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s

/// https://stackoverflow.com/questions/154136/why-use-apparently-meaningless-do-while-and-if-else-statements-in-macros
///
#define BBC_MACRO_BLOCK(x) do {x} while (false)

#if DEBUG
#ifndef BBC_DEBUG
#define BBC_DEBUG
#endif
#endif
