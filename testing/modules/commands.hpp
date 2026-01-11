#ifndef INCLUDE_MODULES_COMMANDS_HPP_
#define INCLUDE_MODULES_COMMANDS_HPP_

#include "../../inc/sols/core/SOLS_parserConf.hpp"

#include <string>
#include <vector>

namespace sols::commands
{
	sols::ParseMessage solsComment(const std::vector<std::string> &args);
}

#endif  // INCLUDE_MODULES_COMMANDS_HPP_
