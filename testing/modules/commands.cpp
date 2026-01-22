#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../inc/sols/SOLSpredefines.h"

#include "../../inc/sols/core/SOLS_callSystemCmd.h"

#include "commands.hpp"

#include "ciof/ciof.hpp"

extern "C"
{
# 	include "cstr/cstr.h"
}

namespace fs = std::filesystem;

#define SOLS_COMMAND_ERRORCODE_INFOFAIL    	 	-2
#define SOLS_COMMAND_ERRORCODE_NEEDPROVIDE 		-3
#define SOLS_COMMAND_ERRORCODE_UNKNONWPATH 		-4

namespace sols::commands
{
	sols::ParseMessage solsCmd(const RegisterCommand &command, const std::vector<std::string> &args)
	{
		sols::ParseMessage pmsg{};
		pmsg.code = 0;

		if (command.isOpened != SOLS_Bool::False)
			return pmsg;

		std::string cmdArgs;

		for (const auto &a : args)
		{ cmdArgs += a + " "; }

		SOLS_PairExecCommand result = sols_core_execCommand(cmdArgs.c_str(), true);

		std::string stdresult = result.second; // stdout, stderr

		SOLS_C_FREE(result.second);

		pmsg.code = result.first;
		pmsg.message = stdresult;

		return pmsg;
	}

	sols::ParseMessage solsPython(const RegisterCommand &command, const std::vector<std::string> &args)
	{
		sols::ParseMessage pmsg{};
		pmsg.code = 0;

		if (command.isOpened != SOLS_Bool::False)
			return pmsg;

		sols::ParseMessage incData = solsInclude(command, args);

		std::vector<std::string> totalArgs;

		totalArgs.emplace_back(DEFAULT_PYTHON_PATH);

		for (const auto &arg : args)
		{ totalArgs.emplace_back(arg); }

		// Execute python
		// For example, with this style:
		// python /path/to/file.py
		pmsg = solsCmd(command, totalArgs);

		return pmsg;
	}

	sols::ParseMessage solsFunction(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

		if (command.isOpened == SOLS_Bool::None) // Closed
			return pmsg;



    	return pmsg;
	}

	sols::ParseMessage solsCall(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

		if (command.isOpened == SOLS_Bool::None) // Closed
			return pmsg;



    	return pmsg;
	}

	sols::ParseMessage solsInclude(const RegisterCommand &command, const std::vector<std::string> &args)
	{
		sols::ParseMessage pmsg{};
		pmsg.code = 0;

		if (command.isOpened != SOLS_Bool::False)
			return pmsg;

		if (args.empty())
		{
			pmsg.message = "Provide a path to include";
			pmsg.code = SOLS_COMMAND_ERRORCODE_NEEDPROVIDE;

			return pmsg;
		}

		if (!fs::exists(args[0]))
		{
			pmsg.message = ciof::format("Path `%1` does not exist", args[0]);
			pmsg.code = SOLS_COMMAND_ERRORCODE_UNKNONWPATH;

			return pmsg;
		}

    	const std::string openTag  = ciof::format("<%1", command.commandName);
    	const size_t &start = command.file.find(openTag);
    	const std::string closeTag = ciof::format("</%1%%2", command.commandName, ">");

    	if (start == std::string::npos)
        	return pmsg;

    	// Find closing tag after opening
    	size_t end = command.file.find(closeTag, start);

    	if (end != std::string::npos) end += closeTag.size(); // include: </comment>
    	else end = command.file.size(); // Remove until EOF

    	pmsg.command = SOLS_EXECCOMMAND_RETACTION_REPLACELINES;
    	pmsg.lineRange = { start, end };

		std::ifstream fileContent(args[0]);

		std::stringstream ss;
		ss << fileContent.rdbuf();
		pmsg.message = ss.str();

		fileContent.close();

		return pmsg;
	}

	sols::ParseMessage solsPrint(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

		if (command.isOpened == SOLS_Bool::None) // Closed
			return pmsg;

		std::string totalPrint;

		for (const auto &a : args)
		{ totalPrint += a; }

		// Replace everything from the string such as:
		// \n
		// \t
		// \r
		// etc
		const std::map<std::string, std::string> replaceChs = {
			{ "\\n", "\n" },
			{ "\\t", "\t" },
			{ "\\r", "\r" }
		};

		CSTR replacement = cstr_init();

		cstr_set(&replacement, totalPrint.c_str());

		for (const auto &replace : replaceChs)
			cstr_replaceAll(&replacement, replace.first.c_str(), replace.second.c_str());

		totalPrint = replacement.data;

		ciof::echo(totalPrint);

		cstr_destroy(&replacement);

    	return pmsg;
	}

	sols::ParseMessage solsComment(const RegisterCommand &command, const std::vector<std::string> &args)
	{
    	sols::ParseMessage pmsg{};
    	pmsg.code = 0;

    	if (command.isOpened != SOLS_Bool::True)
        	return pmsg;

    	const std::string openTag  = ciof::format("<%1", command.commandName);
    	const std::string closeTag = ciof::format("</%1%%2", command.commandName, ">");

    	// Find opening tag
    	const size_t &start = command.file.find(openTag);

    	if (start == std::string::npos)
        	return pmsg;

    	// Find closing tag after opening
    	size_t end = command.file.find(closeTag, start);

    	if (end != std::string::npos) end += closeTag.size(); // include: </comment>
    	else end = command.file.size(); // Remove until EOF

    	pmsg.command = SOLS_EXECCOMMAND_RETACTION_REPLACELINES;
    	pmsg.lineRange = { start, end };

    	return pmsg;
	}
}

