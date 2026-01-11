#include "../../../inc/sols/core/wesi/WESI.h"
#include "../../../inc/sols/SOLSpredefines.h"

#include "cstr/cstr.h"

#include <time.h>

bool wesi_hasInit = false;

CVEC wesi_warnings;
CVEC wesi_errors;
CVEC wesi_logs;
CVEC wesi_total;

int wesi_init(void)
{
	wesi_errors = cvec_init(-1, sizeof(const char*));
	wesi_warnings = cvec_init(-1, sizeof(const char*));
	wesi_logs = cvec_init(-1, sizeof(const char*));

	wesi_hasInit = true;

	return 0;
}

int wesi_destroy(void)
{
	cvec_destroy(&wesi_errors);
	cvec_destroy(&wesi_warnings);
	cvec_destroy(&wesi_total);

	wesi_hasInit = false;

	return 0;
}

char *wesi_resetStyle()
{ return "\033[0m"; }

char *wesi_colorSet(const int color)
{
	CSTR str = cstr_init();
	CSTR intStr = cstr_init();

	cstr_add(&str, "\033[");
	cstr_fromInt(&intStr, color); cstr_add(&str, intStr.data);
	cstr_add(&str, "m");

	char *dupped = CVEC_sys_strdup(str.data);

	cstr_destroy(&str);
	cstr_destroy(&intStr);

	return dupped;
}

char *wesi_rgbSet(const unsigned int r, const unsigned int g, const unsigned int b)
{
	CSTR str = cstr_init();
	CSTR intStr = cstr_init();

	cstr_add(&str, "\033[38;2;");
	cstr_fromInt(&intStr, r); cstr_add(&str, intStr.data); cstr_add(&str, ";");
	cstr_fromInt(&intStr, g); cstr_add(&str, intStr.data); cstr_add(&str, ";");
	cstr_fromInt(&intStr, b); cstr_add(&str, intStr.data); cstr_add(&str, "m");

	char *dupped = CVEC_sys_strdup(str.data);

	cstr_destroy(&str);
	cstr_destroy(&intStr);

	return dupped;
}

int wesi_throw(enum WESI_Type type, const char *message, const bool show)
{
	CSTR totalMsg = cstr_init();

	cstr_add(&totalMsg, "[ ");

	switch (type)
	{
		case WESIType_Error:
		{
			cstr_add(&totalMsg, "ERROR");

			if (show)
			{
				char *setRGB = wesi_rgbSet(230, 25, 25);
				printf("%s", setRGB);
				SOLS_C_FREE(setRGB);
			}

			break;
		}

		case WESIType_Fatal:
		{
			cstr_add(&totalMsg, "FATAL");

			if (show)
			{
				char *setRGB = wesi_rgbSet(255, 0, 0);
				printf("%s", setRGB);
				SOLS_C_FREE(setRGB);
			}

			break;
		}

		case WESIType_Warning:
		{
			cstr_add(&totalMsg, "WARNING");

			if (show)
			{
				char *setRGB = wesi_rgbSet(212, 212, 0);
				printf("%s", setRGB);
				SOLS_C_FREE(setRGB);
			}

			break;
		}

		case WESIType_Log:
		{
			cstr_add(&totalMsg, "LOG");

			if (show)
			{
				char *setRGB = wesi_rgbSet(245, 245, 245);
				printf("%s", setRGB);
				SOLS_C_FREE(setRGB);
			}

			break;
		}

		default: return 1;
	}

	cstr_add(&totalMsg, " | ");

	time_t t = time(NULL);
	struct tm *tmInfo = localtime(&t);
	
	CSTR intTime = cstr_init();

	cstr_fromInt(&intTime, tmInfo->tm_year + 1900);
	cstr_add(&totalMsg, intTime.data);
	cstr_add(&totalMsg, "/");

	cstr_fromInt(&intTime, tmInfo->tm_mon + 1);
	cstr_add(&totalMsg, intTime.data);
	cstr_add(&totalMsg, "/");

	cstr_fromInt(&intTime, tmInfo->tm_mday);
	cstr_add(&totalMsg, intTime.data);

	cstr_add(&totalMsg, " "); // Separator

	cstr_fromInt(&intTime, tmInfo->tm_hour);
	cstr_add(&totalMsg, intTime.data);
	cstr_add(&totalMsg, ":");

	cstr_fromInt(&intTime, tmInfo->tm_min);
	cstr_add(&totalMsg, intTime.data);
	cstr_add(&totalMsg, ":");

	cstr_fromInt(&intTime, tmInfo->tm_sec);
	cstr_add(&totalMsg, intTime.data);

	cstr_add(&totalMsg, " ] ");
	cstr_add(&totalMsg, message);

	if (show) printf("%s\n", totalMsg.data);

	cstr_destroy(&intTime);
	cstr_destroy(&totalMsg);

	return 0;
}

