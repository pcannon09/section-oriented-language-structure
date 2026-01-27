#ifndef INCLUDE_PREDEFINES_SOLSFUNCTION_HPP_
#define INCLUDE_PREDEFINES_SOLSFUNCTION_HPP_

#include <string>
#include <vector>

#include "../SOLSParser.hpp"

namespace sols
{
	typedef struct FunctionProps
	{
		std::string name;
		std::string content;
		std::string innerContent;
	} FunctionProps;

	class Function
	{
	private:
		std::string id;
		std::string fileContent;

		std::vector<FunctionProps> props;

		Parser *parser;
		bool attachedParser = false;

	public:
		Function(const std::string &id);
		~Function();

		void setFileInput(const std::string &input);
		bool attachParser(Parser *parser);
		bool declare(const FunctionProps &prop);

		bool call(RegisterCommand command, sols::ParseMessage retMsg, const std::string &name);

		std::pair<std::string, std::string> get(const std::string &name);

		std::string getFull(const std::string &name);
	};
}

#endif  // INCLUDE_PREDEFINES_SOLSFUNCTION_HPP_
