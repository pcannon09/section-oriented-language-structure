#ifndef INCLUDE_LIB_SOLSREADER_HPP_
#define INCLUDE_LIB_SOLSREADER_HPP_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "../core/SOLS_parserConf.hpp"

namespace sols
{
	typedef struct Node 
	{
		std::string name;
		std::string text;

		std::map<std::string, std::string> attrs;
		std::vector<Node> children;
	} Node;

	typedef struct RegisteredName
	{
		std::string id;
		std::string syntax;

		std::function<ParseMessage(std::vector<std::string>)> call;
	} RegisteredName;

	typedef std::pair<bool, std::string> InitInfoError;

	class Parser 
	{
	private:
		std::string id;
		std::string input;

		size_t pos = 0;
		size_t line = 1;

		std::vector<RegisteredName> regNames;

	protected:
		InitInfoError initErrorMsg;
		InitInfoError endErrorMsg;

		virtual InitInfoError __init();
		virtual InitInfoError __end();

	public:
		explicit Parser(const std::string &id, const std::string &input);
		~Parser();

		Node parse();
		void skipWhitespace();
		void expect(char c);

		char peek() const;
		char get();

		Node parseElem();

		RegisteredName getNameID(const std::string &id);

		bool nameExists(const std::string &syntax);
		bool registerName(const RegisteredName &name);

		void exception(const unsigned int line, const unsigned int &pos, const std::string &text);

		std::string parseName();
		std::string parseStr();

		std::string getID() const;
		std::vector<RegisteredName> getNames() const;
	};
}

#endif  // INCLUDE_LIB_SOLSREADER_HPP_
