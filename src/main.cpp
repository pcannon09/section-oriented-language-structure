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
<root a = "he">
	<child>Heyy world</child>
</root>)";

	sols::Parser parser("main-parser", file);

	{
		sols::RegisteredName name;
		name.id = "root";
		name.syntax = "root";
		name.call = sols::commands::solsComment;

		parser.registerName(name);
	}

	sols::Node root = parser.parse();

	wesi_destroy();

	return 0;
}
