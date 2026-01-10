/**
 * CSTRpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 */

#pragma once

// Project setup
#define CSTR_DEFAULT_C_STD			201112L

// Versioning
#define CSTR_VERSION_MAJOR            1
#define CSTR_VERSION_MINOR            1
#define CSTR_VERSION_PATCH            0

#define CSTR_VERSION_STD              20251202

// Version states:
// * dev
// * beta
// * build
#define CSTR_VERSION_STATE          "build"

#define CSTR_VERSION                ((CSTR_VERSION_MAJOR<<16)|(CSTR_VERSION_MINOR<<8)|(CSTR_VERSION_PATCH)|(CSTR_VERSION_STATE << 24))

#define CSTR_VERSION_CHECK(CSTR_VERSION_MAJOR, CSTR_VERSION_MINOR, CSTR_VERSION_PATCH, CSTR_VERSION_STATE) \
    (((CSTR_VERSION_MAJOR)<<16)|((CSTR_VERSION_MINOR)<<8)|(CSTR_VERSION_PATCH)|((CSTR_VERSION_STATE) << 24))

// Macro utils
#define CSTR_STRINGIFY(x) #x
#define CSTR_TOSTRING(x) CSTR_STRINGIFY(x)

#ifndef CSTR_DEV
#   define CSTR_DEV 1
#endif

// #if _MSC_VER
// #	error "Currently not available for MSVC compilers"
// #endif

#ifdef WIN32
#	define CSTR_OS_WIN32
#	warning "No full support for Windows"
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define CSTR_OS_MACOS
#elif defined(__linux__) || defined(__unix) || defined(__unix__)
#	define CSTR_OS_UNIX_LINUX
#elif defined(__FreeBSD__)
#	define CSTR_OS_FREEBSD
#else
#	warning "Current platform might not be supported"
#endif // defined(WIN32) // Platform check

