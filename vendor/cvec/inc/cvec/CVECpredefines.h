/**
 * CVECpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 */

#pragma once

// Project setup
#define CVEC_DEFAULT_C_STD			201112L

// Versioning
#define CVEC_VERSION_MAJOR              1
#define CVEC_VERSION_MINOR              0
#define CVEC_VERSION_PATCH              0

#define CVEC_VERSION_STD                20251201

// Version states:
// * dev
// * beta
// * build
#define CVEC_VERSION_STATE          "build"

#define CVEC_VERSION                ((CVEC_VERSION_MAJOR<<16)|(CVEC_VERSION_MINOR<<8)|(CVEC_VERSION_PATCH)|(CVEC_VERSION_STATE << 24))

#define CVEC_VERSION_CHECK(CVEC_VERSION_MAJOR, CVEC_VERSION_MINOR, CVEC_VERSION_PATCH, CVEC_VERSION_STATE) \
    (((CVEC_VERSION_MAJOR)<<16)|((CVEC_VERSION_MINOR)<<8)|(CVEC_VERSION_PATCH)|((CVEC_VERSION_STATE) << 24))

// Macro utils
#define CVEC_STRINGIFY(x) #x
#define CVEC_TOSTRING(x) CVEC_STRINGIFY(x)

#ifndef CVEC_DEV
#   define CVEC_DEV true
#endif

#if _MSC_VER
#	warning "Currently not available for MSVC compilers, still proceeding"
#endif

#ifdef WIN32
#	define CVEC_OS_WIN32
#	warning "No full support for Windows"
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define CVEC_OS_MACOS
#elif defined(__linux__) || defined(__unix) || defined(__unix__)
#	define CVEC_OS_UNIX_LINUX
#elif defined(__FreeBSD__)
#	define CVEC_OS_FREEBSD
#else
#	warning "Current platform might not be supported"
#endif // defined(WIN32) // Platform check

