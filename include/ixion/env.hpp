/*************************************************************************
 *
 * Copyright (c) 2011 Kohei Yoshida
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 ************************************************************************/

#ifndef __IXION_ENV_HPP__
#define __IXION_ENV_HPP__

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __IXION_BUILDING_DLL
    #ifdef __GNUC__
      #define IXION_DLLPUBLIC __attribute__ ((dllexport))
    #else
      #define IXION_DLLPUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define IXION_DLLPUBLIC __attribute__ ((dllimport))
    #else
      #define IXION_DLLPUBLIC __declspec(dllimport)
    #endif
  #endif
  #define IXION_DLLLOCAL
#else
  #if __GNUC__ >= 4
    #define IXION_DLLPUBLIC __attribute__ ((visibility ("default")))
    #define IXION_DLLLOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define IXION_DLLPUBLIC
    #define IXION_DLLLOCAL
  #endif
#endif

#endif