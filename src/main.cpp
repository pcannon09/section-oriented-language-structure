#include "../inc/sols/SOLSpredefines.h"
#include "../inc/sols/lib/SOLSParser.hpp"

#include "ciof/ciof.hpp"

#include "../inc/sols/core/wesi/WESI.h"

#include "../testing/modules/commands.hpp"

int main()
{
	wesi_init();
	char *newRgb = wesi_rgbSet(255, 255, 20);

	SOLS_C_FREE(newRgb);

	std::string file = R"(
<include>./testing/files/testing.sols</include>
<print>Hello world\n</print>
<print>Hello world22\n</print>
)";

	const sols::ParserConfig &parserConfig = {
		.automaticLines = true
	};

	sols::Parser parser("main-parser", file, parserConfig);

	{
		sols::RegisteredName name;

		name.id = "comment";
		name.syntax = "comment";
		name.call = SOLS_ADD_COMMAND(sols::commands::solsComment)

		parser.registerName(name);
	}

	{
		sols::RegisteredName name;

		name.id = "print";
		name.syntax = "print";
		name.call = SOLS_ADD_COMMAND(sols::commands::solsPrint)

		parser.registerName(name);
	}

	{
		sols::RegisteredName name;

		name.id = "include";
		name.syntax = "include";
		name.call = SOLS_ADD_COMMAND(sols::commands::solsInclude)

		parser.registerName(name);
	}

	auto root = parser.parse();

	wesi_destroy();

	return 0;
}
