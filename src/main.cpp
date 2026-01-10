#include "../inc/sols/SOLSpredefines.h"
#include "../inc/sols/lib/SOLSParser.hpp"

#include "ciof/ciof.hpp"

extern "C"
{
#	include "../inc/sols/core/wesi/WESI.h"
}

int main()
{
	wesi_init();
	char *newRgb = wesi_rgbSet(255, 255, 20);

	SOLS_C_FREE(newRgb);

	std::string file = R"(
<root a = "he">
	<child>Heyy world</child>
	<a>Heyy world</a>
</root>)";

	sols::Parser parser("main-parser", file);

	sols::Node root = parser.parse();

	wesi_destroy();

	return 0;
}
