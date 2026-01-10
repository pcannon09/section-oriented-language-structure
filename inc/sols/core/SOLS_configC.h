#ifndef INCLUDE_CORE_SOLS_CONFIGC_H_
#define INCLUDE_CORE_SOLS_CONFIGC_H_

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

#endif  // INCLUDE_CORE_SOLS_CONFIGC_H_

