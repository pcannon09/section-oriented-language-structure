#include "commands.hpp"

#include "ciof/ciof.hpp"

#define SOLS_COMMAND_ERRORCODE_INFOFAIL 	-2

namespace sols::commands
{
	sols::ParseMessage solsComment(const RegisterCommand &command,
            const std::vector<std::string> &args)
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

