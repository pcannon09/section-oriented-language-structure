#ifndef INCLUDE_MODULES_COMMANDS_HPP_
#define INCLUDE_MODULES_COMMANDS_HPP_

#include "../../inc/sols/core/SOLS_parserConf.hpp"

#include "../../inc/sols/lib/SOLSParser.hpp"

#include <string>
#include <vector>

namespace sols::commands
{
	sols::ParseMessage solsPrint(const RegisterCommand &command, const std::vector<std::string> &args);
	sols::ParseMessage solsComment(const RegisterCommand &command, const std::vector<std::string> &args);
}

#endif  // INCLUDE_MODULES_COMMANDS_HPP_
