#include "../../../inc/sols/lib/predefines/SOLSFunction.hpp"

namespace sols
{
	Function::Function(const std::string &id)
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

	std::string Function::get(const std::string &name)
	{
		for (const auto &p : this->props)
		{
			// Found with matching name
			if (p.name == name)
				return p.content;
		}

		return "";
	}

	bool Function::call(const std::string &name)
	{
		const std::string &functionContent = this->get(name);

		if (functionContent.empty())
			return false;

		this->parser->setInput(functionContent);
		this->parser->parseName();
		this->parser->restoreInput();
		
		return true;
	}

	std::string Function::getFull(const std::string &name)
	{
		return "";
	}
}

