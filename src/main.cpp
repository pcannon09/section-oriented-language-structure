#include "../inc/sols/SOLSpredefines.h"
#include "../inc/sols/lib/SOLSParser.hpp"

#include "ciof/ciof.hpp"

extern "C"
{
#	include "../inc/sols/core/wesi/WESI.h"
}

#include "../testing/modules/commands.hpp"

int main()
{
	wesi_init();
	char *newRgb = wesi_rgbSet(255, 255, 20);

	SOLS_C_FREE(newRgb);

	std::string file = R"(
<comment>
	<co>Heyy world</co>
</comment>
)";

	const sols::ParserConfig &parserConfig = {
		.automaticLines = true
	};

	sols::Parser parser("main-parser", file, parserConfig);

	{
		sols::RegisteredName name;

		name.id = "comment";
		name.syntax = "comment";
		name.call = [](const sols::RegisterCommand &cmd,
               		const std::vector<std::string> &args) -> sols::ParseMessage
		{ return sols::commands::solsComment(cmd, args); };

		parser.registerName(name);
	}

	sols::Node root = parser.parse();

	wesi_destroy();

	return 0;
}
