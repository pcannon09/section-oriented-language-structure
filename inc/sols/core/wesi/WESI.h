#ifndef INCLUDE_WESI_WESI_H_
#define INCLUDE_WESI_WESI_H_

#include "cvec/cvec.h"

#ifdef __cplusplus
# 	define __WESI_EXTERNC_OPEN 		extern "C" {
# 	define __WESI_EXTERNC_CLOSE 	}
#else
# 	define __WESI_EXTERNC_OPEN
# 	define __WESI_EXTERNC_CLOSE
#endif

__WESI_EXTERNC_OPEN
extern bool wesi_hasInit;
extern CVEC wesi_warnings;
extern CVEC wesi_errors;
extern CVEC wesi_logs;
extern CVEC wesi_total;
__WESI_EXTERNC_CLOSE;

enum WESI_Type
{
	WESIType_Error,
	WESIType_Fatal,
	WESIType_Warning,
	WESIType_Log
};

int wesi_init(void);
int wesi_destroy(void);

int wesi_throw(enum WESI_Type type, const char *message, const bool show);

char *wesi_resetStyle();
char *wesi_colorSet(const int color);
char *wesi_rgbSet(const unsigned int r, const unsigned int g, const unsigned int b);

#endif  // INCLUDE_WESI_WESI_H_
