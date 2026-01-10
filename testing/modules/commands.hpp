#ifndef INCLUDE_MODULES_COMMANDS_HPP_
#define INCLUDE_MODULES_COMMANDS_HPP_

#include "../../inc/sols/core/SOLS_parserConf.hpp"

#include <string>
#include <vector>

namespace sols::commands
{
	sols::ParseMessage solsVersion(std::vector<std::string>);
}

#endif  // INCLUDE_MODULES_COMMANDS_HPP_
