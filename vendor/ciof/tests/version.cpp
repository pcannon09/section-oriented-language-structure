#include "../inc/ciof/ciof.hpp"

int main()
{
	ciof::print("# CIOF VERSION CHECK");
	ciof::print("VERSION: %1.%2.%3", CIOF_VERSION_MAJOR, CIOF_VERSION_MINOR, CIOF_VERSION_PATCH);
	ciof::print("VERSION STATE: %1", CIOF_VERSION_STATE);
	ciof::print("VERSION STANDARD: %1", CIOF_VERSION_STD);

	return 0;
}

