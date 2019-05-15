//  Copyright (c) 2019 Bob Brown. All rights reserved.
//

#pragma once

/**
 * Commonly used macros should be defined in this file.
 */

/// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
///
#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s

// Example Use Cases for STRINGIFY and XSTRINGIFY
//
#if 0
#define foo 4
STRINGIFY (foo)
    results "foo"
XSTRINGIFY (foo)
    results XSTRINGIFY (4)
    results  STRINGIFY (4)
    results  "4"
#endif

/// https://stackoverflow.com/questions/154136/why-use-apparently-meaningless-do-while-and-if-else-statements-in-macros
///
#define BBC_MACRO_BLOCK(x) do {x} while (false)

//Example Use Cases for BBC_MACRO_BLOCK
//
#if 0
{
 
 #define myMacroSafe BBC_MACRO_BLOCK(int x = 1; int y = 1;)
 
 if (true)
 myMacroSafe;
 else
 int i = 1;
 
 #define myMacrounSafe int x = 1; int y = 1;
 /*
 if (true)
 myMacrounSafe;
 else
 int i = 1;
 */

#endif
     
#if DEBUG
#ifndef BBC_DEBUG
#define BBC_DEBUG
#endif
#endif
