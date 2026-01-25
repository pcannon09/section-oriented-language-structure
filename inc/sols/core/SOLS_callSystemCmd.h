#ifndef INCLUDE_CORE_SOLS_CALLSYSTEMCMD_H_
#define INCLUDE_CORE_SOLS_CALLSYSTEMCMD_H_

#ifdef __cplusplus
# 	define __SYSCMD_EXTERNC_OPEN 		extern "C" {
# 	define __SYSCMD_EXTERNC_CLOSE 		}
#else
# 	define __SYSCMD_EXTERNC_OPEN
# 	define __SYSCMD_EXTERNC_CLOSE
#endif

__SYSCMD_EXTERNC_OPEN

#include "SOLS_configC.h"
#include <stdbool.h>

SOLS_PairExecCommand sols_core_execCommand(const char *command, const bool directOut);

__SYSCMD_EXTERNC_CLOSE

#endif  // INCLUDE_CORE_SOLS_CALLSYSTEMCMD_H_
