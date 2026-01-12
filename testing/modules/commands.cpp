#include "commands.hpp"

#include "ciof/ciof.hpp"

namespace sols::commands
{
	sols::ParseMessage solsComment(const RegisterCommand &command, const std::vector<std::string> &args)
	{
		sols::ParseMessage pmsg;

		pmsg.lineRange.first = command.posStart;
		// pmsg.lineRange.second = command.file.find();

		ciof::print(command.file.substr(pmsg.lineRange.first, pmsg.lineRange.second));

		return pmsg;
	}
}

