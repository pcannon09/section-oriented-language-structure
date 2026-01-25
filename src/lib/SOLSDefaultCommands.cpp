#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../inc/sols/SOLSpredefines.h"

#include "../../inc/sols/core/SOLS_callSystemCmd.h"

#include "../../inc/sols/lib/SOLSDefaultCommands.hpp"
#include "../../inc/sols/lib/predefines/SOLSFunction.hpp"

#include "ciof/ciof.hpp"

extern "C"
{
# 	include "cstr/cstr.h"
}

namespace fs = std::filesystem;

#define SOLS_COMMAND_ERRORCODE_INFOFAIL		 	-2
#define SOLS_COMMAND_ERRORCODE_NEEDPROVIDE 		-3
#define SOLS_COMMAND_ERRORCODE_UNKNONWPATH 		-4

namespace sols::defcommands
{
	namespace _utils
	{
		std::pair<int, int> getStartEnd(const RegisterCommand &command)
		{
			const std::string openTag  = ciof::format("<%1", command.commandName);
			const std::string closeTag = ciof::format("</%1%%2", command.commandName, ">");

			// Find opening tag
			const size_t &start = command.file.find(openTag);

			if (start == std::string::npos)
				return { -1, -1 };

			// Find closing tag after opening
			size_t end = command.file.find(closeTag, start);

			if (end != std::string::npos) end += closeTag.size(); // include: </comment>
			else end = command.file.size(); // Remove until EOF

			return { start, end };
		}

		std::string parseNonRawString(
				const std::string &str,
				const std::map<std::string, std::string> &replacements)
		{
			std::string result;
			std::string var;

			bool opened = false;

			for (size_t i = 0; i < str.size(); ++i)
			{
				char c = str[i];

				if (c == '%')
				{
					if (opened)
					{
						// closing `%`
						const auto &it = replacements.find(var);

						if (it != replacements.end())
							result += it->second;

						// Unknown variable; keep original text
						else result += "%" + var + "%";

						var.clear();
						opened = false;
					}

					else
					{
						// opening `%`
						opened = true;
						var.clear();
					}

					continue;
				}

				if (opened) var += c;
				else result += c;
			}

			// Unmatched opening `%`
			if (opened)
				result += "%" + var;

			return result;
		}
	}

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

	sols::ParseMessage solsCall(const RegisterCommand &command, const std::vector<std::string> &args)
	{
		sols::ParseMessage pmsg{};
		pmsg.code = 0;

		if (command.isOpened == SOLS_Bool::True) // Closed
			return pmsg;

		RegisterCommand commandNonConst = command;

		std::string totalPrint;

		for (const auto &a : args)
		{ totalPrint += a; }

		std::string idAccess; // Function name

		// Access with ID for function name in the params
		for (const auto &a : commandNonConst.node.attrs)
		{
			if (a.first == sols::predefined::varName)
			{
				idAccess = a.second;

				break;
			}
		}

		std::pair<int, int> range = _utils::getStartEnd(command);

		if (command.function)
		{
			pmsg.lineRange = range;
			pmsg.col = command.posStart;

			commandNonConst.function->call(command, pmsg, idAccess);
		}

		else
		{
			// TODO:
			// DO ERROR
		}

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

		std::pair<int, int> range = _utils::getStartEnd(command);

		pmsg.command = SOLS_EXECCOMMAND_RETACTION_REPLACELINES;
		pmsg.lineRange = { range.first, range.second};

		std::ifstream fileContent(args[0]);

		std::stringstream ss;
		ss << fileContent.rdbuf();
		pmsg.message = ss.str();

		fileContent.close();

		return pmsg;
	}

	sols::ParseMessage solsFunction(const RegisterCommand &command, const std::vector<std::string> &args)
	{
		sols::ParseMessage pmsg{};
		pmsg.code = 0;

		if (command.isOpened == SOLS_Bool::True)
			return pmsg;

		RegisterCommand commandNonConst = command;
		
		std::string totalPrint;

		for (const auto &a : args)
		{ totalPrint += a; }

		std::string idAccess; // Function name

		// Access with ID for function name in the params
		for (const auto &a : commandNonConst.node.attrs)
		{
			if (a.first == sols::predefined::varName)
			{
				idAccess = a.second;

				break;
			}
		}

		const std::string newTotalPrint = _utils::parseNonRawString(
				totalPrint,
				commandNonConst.node.attrs
				);

		sols::RegisteredName name;

		std::pair<int, int> range = _utils::getStartEnd(command);

		pmsg.lineRange = { range.first, range.second };
		pmsg.command = SOLS_EXECCOMMAND_RETACTION_DECLFUNCTION;
		pmsg.message = idAccess;

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

		// Replace with the `replaceChs` map
		for (const auto &replace : replaceChs)
			cstr_replaceAll(&replacement, replace.first.c_str(), replace.second.c_str());

		RegisterCommand commandNonConst = command;

		totalPrint = _utils::parseNonRawString(
				replacement.data,
				commandNonConst.node.attrs
				);

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

		std::pair<int, int> range = _utils::getStartEnd(command);

		pmsg.command = SOLS_EXECCOMMAND_RETACTION_REPLACELINES;
		pmsg.lineRange = { range.first, range.second };
		pmsg.message = "";

		return pmsg;
	}
}

