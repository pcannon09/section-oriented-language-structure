// For this test you will need to have CIOF installed on your system

#include <ciof.hpp>

int main()
{
	ciof::outputConf.spacing = true;
	ciof::outputConf.ignoreAllFormating = false;

	std::string name;

	ciof::input(ciof::format("%1>>%2 ", ciof::rgbSet(255, 220, 9), ciof::rgbSet(90, 90, 200)), &name);

	ciof::styleReset();

	ciof::print("1. You typed %1`%2`%3, type something else", ciof::styleSet(ciof::OutputStyle::Bold) + ciof::styleSet(ciof::OutputStyle::Underline), name, ciof::styleReset() + ciof::rgbSet(90, 90, 200));
	ciof::print();
	ciof::echo("%3%2. You typed `%1`, type something else\nThis text is literaly printed inside the `ciof:echo()` function%2\n",
			name, ciof::styleReset(), ciof::styleSet(ciof::OutputStyle::Strikethrough) + ciof::styleSet(ciof::OutputStyle::Italic));

	ciof::cursorPos(15, 2);

	ciof::echoLog("-> LOG MESSAGE <-\n");
	ciof::echoError("-> ERROR MESSAGE <-");

	ciof::styleReset();

	return 0;
}

