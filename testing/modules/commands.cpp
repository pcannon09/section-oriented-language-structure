#include <algorithm>

#include "commands.hpp"

#include "ciof/ciof.hpp"

extern "C"
{
# 	include "cstr/cstr.h"
}

#define SOLS_COMMAND_ERRORCODE_INFOFAIL 	-2

namespace sols::commands
{
	sols::ParseMessage solsPrint(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

		if (command.isOpened == SOLS_Bool::None) // Closed
			return pmsg;

		std::string totalPrint;

		for (const auto &a : args)
		{ totalPrint += a; }

		// Replace everything from the string such as:
		// \n
		// \t
		// \r
		// etc
		const std::map<std::string, std::string> replaceChs = {
			{ "\\n", "\n" },
			{ "\\t", "\t" },
			{ "\\r", "\r" }
		};

		CSTR replacement = cstr_init();

		cstr_set(&replacement, totalPrint.c_str());

		for (const auto &replace : replaceChs)
			cstr_replaceAll(&replacement, replace.first.c_str(), replace.second.c_str());

		totalPrint = replacement.data;

		cstr_destroy(&replacement);

		ciof::echo(totalPrint);

    	return pmsg;
	}

	sols::ParseMessage solsComment(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

    	if (command.isOpened != SOLS_Bool::True)
        	return pmsg;

    	const std::string openTag  = ciof::format("<%1", command.commandName);
    	const std::string closeTag = ciof::format("</%1%%2", command.commandName, ">");

    	// Find opening tag
    	const size_t &start = command.file.find(openTag);

    	if (start == std::string::npos)
        	return pmsg;

    	// Find closing tag after opening
    	size_t end = command.file.find(closeTag, start);

    	if (end != std::string::npos) end += closeTag.size(); // include: </comment>
    	else end = command.file.size(); // Remove until EOF

    	pmsg.command = SOLS_EXECCOMMAND_RETACTION_REPLACELINES;
    	pmsg.lineRange = { start, end };

    	return pmsg;
	}
}

