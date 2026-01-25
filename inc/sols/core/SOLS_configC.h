#ifndef INCLUDE_CORE_SOLS_CONFIGC_H_
#define INCLUDE_CORE_SOLS_CONFIGC_H_

#ifdef __cplusplus
# 	define __CONFIGC_EXTERNC_OPEN 		extern "C" {
# 	define __CONFIGC_EXTERNC_CLOSE 		}
#else
# 	define __CONFIGC_EXTERNC_OPEN
# 	define __CONFIGC_EXTERNC_CLOSE
#endif

__CONFIGC_EXTERNC_OPEN

#include "../SOLSpredefines.h"

#define SOLS_Declare_Pair(type, type2, id) \
	typedef struct \
	{ \
		type first; \
		type2 second; \
	} SOLS_Pair##id

SOLS_Declare_Pair(int, char*, ExecCommand);

#ifdef SOLS_OS_WIN32
#	if defined(SOLS_BUILD_DLL)
# 		define SOLS_API __declspec(dllexport)
#	else
# 		define SOLS_API __declspec(dllimport)
# 	endif
#else
# 	define SOLS_API
#endif // SOLS_OS_WIN32

__CONFIGC_EXTERNC_CLOSE

#endif  // INCLUDE_CORE_SOLS_CONFIGC_H_

