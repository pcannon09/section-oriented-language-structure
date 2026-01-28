#include "../../../inc/sols/lib/predefines/SOLSFunction.hpp"

#include "ciof/ciof.hpp"

namespace sols
{
	Function::Function(const std::string &id)
		: id(id)
	{
	}

	Function::~Function()
	{ }

	void Function::setFileInput(const std::string &input)
	{ this->fileContent = input; }

	bool Function::attachParser(Parser *parser)
	{
		// Checks
		if (!parser || this->attachedParser)
			return false;

		if (!this->attachedParser)
			this->attachedParser = true;

		// Finally, attach the parser
		this->parser = parser;

		return true;
	}

	bool Function::declare(const FunctionProps &prop)
	{
		if (prop.name.empty())
			return false;

		this->props.emplace_back(prop);

		return true;
	}

	std::pair<std::string, std::string> Function::get(const std::string &name)
	{
		for (const auto &p : this->props)
		{
			// Found with matching name
			if (p.name == name)
				return { p.content, p.innerContent };
		}

		return { "", "" };
	}

	bool Function::call(RegisterCommand command, sols::ParseMessage retMsg, const std::string &name)
	{
		const std::string &functionContent = this->get(name).second;

		if (functionContent.empty())
			return false;

		this->parser->input.replace(
				retMsg.lineRange.first,
				retMsg.lineRange.second,
				functionContent
				);

		return true;
	}

	std::string Function::getFull(const std::string &name)
	{
		return "";
	}
}

