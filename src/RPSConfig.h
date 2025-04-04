/***************************************************************************
 *   Copyright (c) 2004 Jürgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the LabRPS CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/
/** \file RPSConfig.h
 *  \brief Include all needed defines and macros
 *  Here all defines and macro switching is done for LabRPS.
 *  Every used library has its own section to define the configuration.
 *  This file keeps the makefiles and project files cleaner.
 */


#ifndef RPS_CONFIG_H
#define RPS_CONFIG_H



//**************************************************************************
// switching the operating systems

// First check for *WIN64* since the *WIN32* are also set on 64-bit platforms
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#   ifndef RPS_OS_WIN32
#   define RPS_OS_WIN32
#   endif
#   ifndef RPS_OS_WIN64
#   define RPS_OS_WIN64
#   endif
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   ifndef RPS_OS_WIN32
#   define RPS_OS_WIN32
#   endif
#   if defined(__MINGW32__)
#   if HAVE_CONFIG_H
#   include <config.h>
#   endif // HAVE_CONFIG_H
//# define HAVE_INT8_T
//# define HAVE_UINT8_T
//# define HAVE_INT16_T
//# define HAVE_UINT16_T
//# define HAVE_INT32_T
//# define HAVE_UINT32_T
//# define HAVE_INT64_T
//# define HAVE_UINT64_T
//# define HAVE_INTPTR_T
//# define HAVE_UINTPTR_T
#   endif
#elif defined(__MWERKS__) && defined(__INTEL__)
#   ifndef RPS_OS_WIN32
#   define RPS_OS_WIN32
#   endif
#elif defined(__APPLE__)
#   ifndef RPS_OS_MACOSX
#   define RPS_OS_MACOSX
#   endif
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__GLIBC__)
#   ifndef RPS_OS_LINUX
#   define RPS_OS_LINUX
#   endif
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#   ifndef RPS_OS_BSD
#   define RPS_OS_BSD
#   endif
#elif defined(__CYGWIN__)
#   ifndef RPS_OS_CYGWIN
#   define RPS_OS_CYGWIN
#endif

#else
#   error "LabRPS is not ported to this OS yet. For help see www.labrps.com"
#endif

#ifdef RPS_OS_WIN32
#   define PATHSEP '\\'
#else
#   define PATHSEP '/'
#endif

//**************************************************************************
// Standard types for Windows

#if defined(__MINGW32__)
// nothing specific here
#elif defined (RPS_OS_WIN64) || defined (RPS_OS_WIN32)

#ifndef HAVE_INT8_T
#define HAVE_INT8_T
typedef signed char         int8_t;
#endif

#ifndef HAVE_UINT8_T
#define HAVE_UINT8_T
typedef unsigned char       uint8_t;
#endif

#ifndef HAVE_INT16_T
#define HAVE_INT16_T
typedef short               int16_t;
#endif

#ifndef HAVE_UINT16_T
#define HAVE_UINT16_T
typedef unsigned short      uint16_t;
#endif

#ifndef HAVE_INT32_T
#define HAVE_INT32_T
typedef int                 int32_t;
#endif

#ifndef HAVE_UINT32_T
#define HAVE_UINT32_T
typedef unsigned int        uint32_t;
#endif

#ifndef HAVE_INT64_T
#define HAVE_INT64_T
typedef __int64             int64_t;
#endif

#ifndef HAVE_UINT64_T
#define HAVE_UINT64_T
typedef unsigned __int64    uint64_t;
#endif

/* The <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1
/* The <stdint.h> header file. */
#define HAVE_STDINT_H 1
/* The <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1
/* The <stddef.h> header file. */
#define HAVE_STDDEF_H 1

#endif

//**************************************************************************
// Qt

// Make sure to explicitly use the correct conversion
#define QT_NO_CAST_FROM_ASCII
#undef  QT3_SUPPORT


#if defined (RPS_OS_WIN32) || defined(RPS_OS_CYGWIN)
# ifndef QT_DLL
#  define QT_DLL
# endif
#endif

#ifndef QT_NO_KEYWORDS
# define QT_NO_KEYWORDS
#endif

#ifndef QT_THREAD_SUPPORT
# define QT_THREAD_SUPPORT
#endif

#ifndef QT_ALTERNATE_QTSMANIP
# define QT_ALTERNATE_QTSMANIP
#endif

// stops inclusion of the QT 3 header through the SoQT header...
//#define __Qt3All__

//**************************************************************************
// Boost
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif

//**************************************************************************
// Exception handling

// Don't catch C++ exceptions in DEBUG!
#ifdef RPS_DEBUG
# define DONT_CATCH_CXX_EXCEPTIONS 1
# define DBG_TRY
# define DBG_CATCH(X)
#else
/// used to switch a catch with the debug state
# define DBG_TRY try {
/// see docu DBGTRY
# define DBG_CATCH(X) } catch (...) { X }
#endif


//**************************************************************************
// Windows import export DLL defines
#include <RPSGlobal.h>

//**************************************************************************
// here get the warnings of too long specifiers disabled (needed for VC6)
#ifdef _MSC_VER
#   pragma warning( disable : 4251 )
//#   pragma warning( disable : 4503 )
//#   pragma warning( disable : 4786 )  // specifier longer then 255 chars
//#   pragma warning( disable : 4290 )  // not implemented throw specification
#   pragma warning( disable : 4996 )  // suppress deprecated warning for e.g. open()
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#   pragma warning( disable : 4244 )
#   pragma warning( disable : 4267 )
#endif
//#	define _PreComp_                  // use precompiled header
#endif

#endif //RPS_CONFIG_H
