/*
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

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
