#include "commands.hpp"

#include "ciof/ciof.hpp"

#define SOLS_COMMAND_ERRORCODE_INFOFAIL 	-2

namespace sols::commands
{
	sols::ParseMessage solsComment(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

    	// Only act on open section
    	if (command.isOpened != SOLS_Bool::True)
        	return pmsg;

    	size_t start = command.posStart;

    	// Find closing section
    	const std::string &toFind = ciof::format("</%1>", command.commandName);
    	size_t end = command.file.find(toFind, start);

    	if (end == std::string::npos) return pmsg;

    	end += std::string(toFind).size();

    	pmsg.command = SOLS_EXECCOMMAND_RETACTION_REPLACELINES;
    	pmsg.lineRange = { start, end };

    	return pmsg;
	}
}

