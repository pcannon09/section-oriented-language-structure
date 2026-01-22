#include "../../inc/sols/core/SOLS_callSystemCmd.h"

#include "../../vendor/cstr/inc/cstr/cstr.h"

#include <stdio.h>

#define __SOLS_CORE_BUFFSIZE 	4096

SOLS_PairExecCommand sols_core_execCommand(const char *command, const bool directOut)
{
	SOLS_PairExecCommand ret = {0};

	FILE *file = popen(command, "r");

	if (!file)
	{
		ret.first = 1;

		return ret;
	}

	CSTR total = cstr_init();

	char buff[__SOLS_CORE_BUFFSIZE];

	while (fgets(buff, __SOLS_CORE_BUFFSIZE, file))
	{
		cstr_add(&total, buff);
		
		if (directOut)
		{
			printf("%s", buff);
			fflush(stdout);
		}
	}

	char *totalCpy = strdup(total.data);

	ret.first = 0;
	ret.second = totalCpy;

	cstr_destroy(&total);

	pclose(file);

	return ret;
}

