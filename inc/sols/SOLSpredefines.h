/**
 * SOLSpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 */

#ifndef INCLUDE_SOLS_SOLSPREDEFINES_HPP_
#define INCLUDE_SOLS_SOLSPREDEFINES_HPP_

// Project setup
#define SOLS_DEFAULT_CPP_STD			201703L
#define SOLS_DEFAULT_C_STD			201112L

// Versioning
#define SOLS_VERSION_MAJOR            0
#define SOLS_VERSION_MINOR            0
#define SOLS_VERSION_PATCH            1

#define SOLS_VERSION_STD              0

// Version states:
// * dev
// * beta
// * build
#define SOLS_VERSION_STATE          "dev"

#define SOLS_VERSION                ((SOLS_VERSION_MAJOR<<16)|(SOLS_VERSION_MINOR<<8)|(SOLS_VERSION_PATCH)|(SOLS_VERSION_STATE << 24))

#define SOLS_VERSION_CHECK(SOLS_VERSION_MAJOR, SOLS_VERSION_MINOR, SOLS_VERSION_PATCH, SOLS_VERSION_STATE) \
    (((SOLS_VERSION_MAJOR)<<16)|((SOLS_VERSION_MINOR)<<8)|(SOLS_VERSION_PATCH)|((SOLS_VERSION_STATE) << 24))

// Macro utils
#define SOLS_STRINGIFY(x) #x
#define SOLS_TOSTRING(x) SOLS_STRINGIFY(x)

#ifndef SOLS_DEV
#   define SOLS_DEV true
#endif

#ifdef _WIN32
#	define SOLS_OS_WIN32
#	define popen 	_popen
#	define pclose 	_pclose
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define SOLS_OS_MACOS
#elif defined(__linux__)
#	define SOLS_OS_LINUX
#elif defined(__unix) || defined(__unix__)
#	define SOLS_OS_UNIX
#elif defined(__FreeBSD__)
#	define SOLS_OS_FREEBSD
#else
#	warning "Current platform might not be supported"
#endif // defined(WIN32) // Platform check

#define SOLS_UNKNOWN_ERROR           1

#define SOLS_FREE(x) \
	delete x; x = nullptr;

#define SOLS_C_FREE(x) \
	free(x); x = NULL;

#endif  // INCLUDE_SOLS_SOLSPREDEFINES_HPP_

