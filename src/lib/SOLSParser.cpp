#include "ciof/ciof.hpp"

#include "../../inc/sols/lib/SOLSParser.hpp"
#include "../../inc/sols/lib/predefines/SOLSFunction.hpp"

#include "../../inc/sols/core/wesi/WESI.h"
#include "../../inc/sols/SOLSpredefines.h"

#include <algorithm>
#include <cctype>

#define __SOLS_PARSER_COMMAND_CALL(name) name.call(commandSend, { node.content })
#define __SOLS_RETURN_DEBUG(statement, ret) \
	statement \
	return ret

namespace sols
{
	// PROTECTED //
	InitInfoError Parser::__init()
	{
		this->function = new Function(id + "_function");
		this->function->setFileInput(this->input);
		this->function->attachParser(this);

		return { true, "" };
	}

	InitInfoError Parser::__end()
	{
		if (!this->function)
			return { false, "Could not free `function`; Variable is NULL" };

		else SOLS_FREE(this->function);

		return { true, "" };
	}

	// PUBLIC //
	Parser::Parser(const std::string &id, const std::string &input, const ParserConfig &conf)
		: id(id), input(input), config(conf)
	{ this->initErrorMsg = this->__init(); }

	Parser::~Parser()
	{ this->endErrorMsg = this->__end(); }

	std::vector<Node> Parser::parse()
	{
		std::vector<Node> nodes;

		this->skipWhitespace();

		while (this->pos < this->input.size())
		{
			if (this->peek() == '<')
				nodes.emplace_back(this->parseElem());

			this->skipWhitespace();
		}

		return nodes;
	}

	char Parser::peek() const
	{
		return this->pos < this->input.size() ?
			this->input[this->pos] : '\0';
	}

	char Parser::get()
	{
		return pos < input.size()
			? input[pos++] : '\0';
	}

	bool Parser::expect(char c)
	{
		if (this->get() != c)
		{
			std::string chstr(1, c);

			wesi_throw(WESIType_Error, ciof::format("Expected token `%1`", chstr).c_str(), true);

			return false;
		}

		return true;
	}

	void Parser::skipWhitespace()
	{
		while (std::isspace(this->peek()))
			this->get();
	}

	std::string Parser::parseName()
	{
		std::string str;

		while (std::isalnum(this->peek()) ||
				this->peek() == '_' || this->peek() == '-')
			str += this->get();

		return str;
	}

	std::string Parser::parseStr()
	{
		// Open str
		this->expect('"');

		std::string str;

		// Iterate inside str
		while (this->peek() != '"')
			str += this->get();

		// Close str once iterated
		this->expect('"');

		return str;
	}

	void Parser::exception(const unsigned int line, const unsigned int &pos, const std::string &text, const std::string &message)
	{
		std::string exceptMsg;
		std::string sep = std::to_string(line) + "| ";

		exceptMsg += message + "\n" + sep + text;
		exceptMsg += '\n';

		for (size_t i = 0 ; i < sep.size() + pos ; i++)
			exceptMsg += "~";

		exceptMsg += "^";

		ciof::print(exceptMsg);
		wesi_throw(WESIType_Fatal, exceptMsg.c_str(), false);
	}

	void Parser::execCommand(ParseMessage commandRet)
	{
		// Make errors first
		if (commandRet.code == SOLS_EXECCOMMAND_RETACTION_NOCALL)
			return;

		// If no errors, do the following:
		if (commandRet.command == SOLS_EXECCOMMAND_RETACTION_SUCCESS)
			return; // Don't do anything if no error encountered;
					// Action is already executed

		else if (commandRet.command == SOLS_EXECCOMMAND_RETACTION_REPLACELINES)
		{
			this->input.replace(
				commandRet.lineRange.first,
				commandRet.lineRange.second - commandRet.lineRange.first,
				commandRet.message
			);

			// Continue parsing after the removed block
			this->pos = commandRet.lineRange.first;
			this->parseElem();
		}

		else if (commandRet.command == SOLS_EXECCOMMAND_RETACTION_DECLFUNCTION)
		{
			if (this->function)
			{
				const std::string content = this->input.substr(
					commandRet.lineRange.first,
					commandRet.lineRange.second - commandRet.lineRange.first
				);

				sols::FunctionProps prop;
				prop.name = commandRet.message; // Function name
				prop.content = content; // Inner function comment
				prop.innerContent = commandRet.message;

				this->function->declare(prop);

				// Erase the needed //

				this->input.erase(
					commandRet.lineRange.first,
					commandRet.lineRange.second - commandRet.lineRange.first
				);

				this->pos = commandRet.lineRange.first;
				this->parseElem();
			}
		}

		else if (commandRet.command == SOLS_EXECCOMMAND_RETACTION_UNKNOWNERR)
		{
 			WESI_Type setType = WESIType_Fatal;

			if (commandRet.type == ParserMessageType::Error)
				setType = WESIType_Error;

			else if (commandRet.type == ParserMessageType::Warning)
				setType = WESIType_Warning;

			else if (commandRet.type == ParserMessageType::Message)
				setType = WESIType_Log;

			wesi_throw(setType, commandRet.message.c_str(), true);
		}
	}

	void Parser::setInput(const std::string &inp)
	{
		this->inputSave = this->input;
		this->input = inp;
	}

	void Parser::restoreInput()
	{
		this->input = this->inputSave;
		this->inputSave = "";
	}

	Node Parser::parseElem()
	{
		RegisterCommand commandSend;

		this->skipWhitespace();

		const size_t &sectionStart = this->pos;

		if (this->expect('<'))
			commandSend.isOpened = SOLS_Bool::True;

		Node node;
		node.name = this->parseName();
		commandSend.commandName = node.name;

		const int &posSep = 1 + node.name.size();

		const size_t &lineStart = this->pos - posSep;
		const size_t &lineEnd = this->input.find('\n', lineStart);

		// If name is not registered, make the following action
		// Throw an exception
		if (!std::count_if(node.name.begin(), node.name.end(), [](unsigned int c) { return std::isspace(c); } ) &&
				!this->nameExists(node.name))
		{
			this->exception(
					this->line,
					1,
					this->input.substr(lineStart, lineEnd - lineStart),
					ciof::format("Command name `%1` is not registered", node.name)
			);

			__SOLS_RETURN_DEBUG(, node);
		}

		commandSend.file = this->input;
		commandSend.posStart = sectionStart;
		commandSend.node = node;
		commandSend.function = this->function;
		commandSend.file = this->input;

		const RegisteredName &regName = this->getNameBySyntax(node.name);
		ParseMessage commandCall = __SOLS_PARSER_COMMAND_CALL(regName);

		if (commandCall.code == -1)
		{ __SOLS_RETURN_DEBUG(, node); }
		else this->execCommand(commandCall);

		this->skipWhitespace();

		// Get attributes;
		// var = "something"
		while (std::isalnum(this->peek()))
		{
			const std::string &key = this->parseName();

			this->skipWhitespace();
			this->expect('=');
			this->skipWhitespace();

			const std::string &parsedKeyVal = this->parseStr();
			node.attrs[key] = parsedKeyVal;

			this->skipWhitespace();
		}

		// Self closing
		if (this->peek() == '/')
		{
			this->get();
			this->expect('>');

			__SOLS_RETURN_DEBUG(, node);
		}

		// Content inside the node
		while (1)
		{
			if (this->pos >= this->input.size())
 				__SOLS_RETURN_DEBUG(, node);

			// Get new line char to add a new line to `Parser::line`
			if (node.text.ends_with('\n'))
				this->line++;

			if (this->peek() == '<' &&
					this->input[pos + 1] == '/') // </
				break;

			if (this->peek() == '<')
				node.children.emplace_back(this->parseElem());

			else
			{
				node.text += this->get();
				node.content += this->peek();
			}
		}

		// Remove the last remained char that shouldn't be there for the closing
		if (!node.content.empty()) node.content.pop_back();

		// Closing tag
		// </{name}>
		this->expect('<'); this->expect('/');
		this->parseName();
		if (this->expect('>'))
			commandSend.isOpened = SOLS_Bool::False;

		commandSend.node = node;
		commandSend.function = this->function;
		commandSend.file = this->input;

		// Run again if the command call needs to be executed again;
		// 	* Developer can disable it when developing the software with SOLS
		commandCall = __SOLS_PARSER_COMMAND_CALL(regName);

		if (commandCall.code == -1)
		{ __SOLS_RETURN_DEBUG(, node); }
		else this->execCommand(commandCall);

		// Get the last line
		if (node.text.ends_with('\n'))
			this->line++;

		__SOLS_RETURN_DEBUG(, node);
	}

	RegisteredName Parser::getNameID(const std::string &id)
	{
		for (const auto &x : this->regNames)
		{ if (x.id == id) return x; }

		return {};
	}

	RegisteredName Parser::getNameBySyntax(const std::string &syntax)
	{
		for (const auto &x : this->regNames)
		{
			if (x.syntax == syntax)
				return x;
		}

		return {};
	}

	bool Parser::nameExists(const std::string &syntax)
	{
		for (const auto &x : this->regNames)
		{
			if (x.syntax == syntax)
				return true;
		}

		return false;
	}

	bool Parser::registerName(const RegisteredName &name)
	{
		if (name.id.empty() ||
				name.id == this->getNameID(name.id).id) return false;

		this->regNames.emplace_back(name);

		return true;
	}

	std::string Parser::getID() const
	{ return this->id; }

	std::vector<RegisteredName> Parser::getNames() const
	{ return this->regNames; }
}

#ifdef __SOLS_PARSER_COMMAND_CALL
# 	undef __SOLS_PARSER_COMMAND_CALL
#endif // defined(__SOLS_PARSER_COMMAND_CALL)

