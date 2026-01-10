/**
 * EXAMPLEpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 * NOTICE: 
 * - RENAMED TO CIOFpredefines.hpp
 * - MADE FOR The CIOF Project Utils
 */

#pragma once

// Project setup
#define CIOF_DEFAULT_CPP_STD			201703L

#if defined(_MSC_VER)
#   define __CIOF_CPLUSPLUS     _MSVC_LANG
#else
#   define __CIOF_CPLUSPLUS     __cplusplus
#endif

// Versioning
#define CIOF_VERSION_MAJOR            1
#define CIOF_VERSION_MINOR            1
#define CIOF_VERSION_PATCH            0

#define CIOF_VERSION_STD              20251013

// Version states:
// * dev
// * alpha
// * beta
// * build
#define CIOF_VERSION_STATE          "build"

#define CIOF_VERSION                ((CIOF_VERSION_MAJOR<<16)|(CIOF_VERSION_MINOR<<8)|(CIOF_VERSION_PATCH)|(CIOF_VERSION_STATE << 24))

#define CIOF_VERSION_CHECK(CIOF_VERSION_MAJOR, CIOF_VERSION_MINOR, CIOF_VERSION_PATCH, CIOF_VERSION_STATE) \
    (((CIOF_VERSION_MAJOR)<<16)|((CIOF_VERSION_MINOR)<<8)|(CIOF_VERSION_PATCH)|((CIOF_VERSION_STATE) << 24))

// Macro utils
#define CIOF_STRINGIFY(x) #x
#define CIOF_TOSTRING(x) CIOF_STRINGIFY(x)

#ifndef CIOF_DEV
#   define CIOF_DEV true
#endif // !defined(CIOF_DEV)

// OS Checks
#ifdef _WIN32
#	define CIOF_OS_WIN32
#   if !defined(WIN32_ANSI_SUPPORT)
#       define WIN32_ANSI_SUPPORT       1
#   endif
#   if WIN32_ANSI_SUPPORT == 1
#       warning "ANSI Support for Windows is supported from Windows 10 or newer"
#   endif
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define CIOF_OS_MACOS
#elif defined(__linux__) || defined(__unix) || defined(__unix__)
#	define CIOF_OS_UNIX_LINUX
#elif defined(__FreeBSD__)
#	define CIOF_OS_FREEBSD
#else
#	warning "Current platform might not supported"
#endif // defined(WIN32) // Platform check

