#ifndef INCLUDE_LIB_SOLSREADER_HPP_
#define INCLUDE_LIB_SOLSREADER_HPP_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "../core/SOLS_parserConf.hpp"

#define SOLS_EXECCOMMAND_RETACTION_UNKNOWNERR 			-1
#define SOLS_EXECCOMMAND_RETACTION_SUCCESS 				0
#define SOLS_EXECCOMMAND_RETACTION_REPLACELINES 		1
#define SOLS_EXECCOMMAND_RETACTION_NOCALL 				2

#define SOLS_ADD_COMMAND(func) \
	[](const sols::RegisterCommand &cmd, \
		const std::vector<std::string> &args) -> sols::ParseMessage \
	{ return func(cmd, args); }; \

namespace sols
{
	typedef struct Node 
	{
		std::string name; // Command name <{command}>
		std::string text;
		std::string content; // Content inside;
							 // <command>{CONTENT_HERE}</command>

		std::map<std::string, std::string> attrs; // Variable set;
												  // var = "something"
		std::vector<Node> children; // Sub-nodes
	} Node;

	typedef struct RegisterCommand
	{
		int posStart;

		SOLS_Bool isOpened = SOLS_Bool::None;

		std::string file;
		std::string commandName;
	} RegisterCommand;

	typedef struct RegisteredName
	{
		std::string id;
		std::string syntax;

		std::function<ParseMessage(const RegisterCommand &command, const std::vector<std::string>&)> call;
	} RegisteredName;

	typedef struct ParserConfig
	{
		bool automaticLines = true; // ie:
									// Adds EOF at the end of the file and stops parsing from there
	} ParserConfig;

	typedef std::pair<bool, std::string> InitInfoError;

	class Parser 
	{
	private:
		std::string id;
		std::string input;
		std::string parsedStr;

		size_t pos = 0;
		size_t line = 1;

		std::vector<RegisteredName> regNames;

		ParserConfig config;

	protected:
		InitInfoError initErrorMsg;
		InitInfoError endErrorMsg;

		virtual InitInfoError __init();
		virtual InitInfoError __end();

	public:
		explicit Parser(const std::string &id, const std::string &input, const ParserConfig &conf);
		~Parser();

		virtual void execCommand(ParseMessage commandRet);

		std::vector<Node> parse();

		void skipWhitespace();
		bool expect(char c);

		char peek() const;
		char get();

		Node parseElem();

		RegisteredName getNameID(const std::string &id);
		RegisteredName getNameBySyntax(const std::string &syntax);

		bool nameExists(const std::string &syntax);
		bool registerName(const RegisteredName &name);

		void exception(const unsigned int line, const unsigned int &pos, const std::string &text, const std::string &message);

		std::string parseName();
		std::string parseStr();

		std::string getID() const;
		std::string getInput() const;
		std::vector<RegisteredName> getNames() const;
	};
}

#endif  // INCLUDE_LIB_SOLSREADER_HPP_
