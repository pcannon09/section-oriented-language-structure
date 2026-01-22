#ifndef INCLUDE_MODULES_COMMANDS_HPP_
#define INCLUDE_MODULES_COMMANDS_HPP_

#include "../core/SOLS_parserConf.hpp"

#include "SOLSParser.hpp"

#include <string>
#include <vector>

namespace sols::defcommands
{
	namespace _utils
	{
		std::string parseNonRawString(const std::string &str, const std::string varName, const std::string varVal);
	}

	sols::ParseMessage solsPython(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsCmd(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsFunction(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsCall(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsInclude(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsPrint(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsComment(const RegisterCommand &command, const std::vector<std::string> &args);
}

#endif  // INCLUDE_MODULES_COMMANDS_HPP_
