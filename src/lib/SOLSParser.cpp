#include "ciof/ciof.hpp"

#include "../../inc/sols/lib/SOLSParser.hpp"

extern "C"
{
#	include "../../inc/sols/core/wesi/WESI.h"
}

#include <algorithm>
#include <cctype>

namespace sols
{
	// PROTECTED //
	InitInfoError Parser::__init()
	{ return { true, "" }; }

	InitInfoError Parser::__end()
	{ return { true, "" }; }

	// PUBLIC //
	Parser::Parser(const std::string &id, const std::string &input)
		: id(id), input(input)
	{ this->initErrorMsg = this->__init(); }

	Parser::~Parser()
	{ this->endErrorMsg = this->__end(); }

	Node Parser::parse()
	{
		this->skipWhitespace();

		return this->parseElem();
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

	void Parser::expect(char c)
	{
		if (this->get() != c)
		{
			std::string chstr(1, c);

			wesi_throw(WESIType_Error, ciof::format("Expected token `%1`", chstr).c_str(), true);
		}
	}

	void Parser::skipWhitespace()
	{
		while (std::isspace(this->peek()))
			this->get();
	}

	std::string Parser::parseName()
	{
		std::string str;

		while (std::isalnum(this->peek()) || this->peek() == '_' || this->peek() == '-')
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

	void Parser::exception(const unsigned int line, const unsigned int &pos, const std::string &text)
	{
		std::string exceptMsg;
		std::string sep = std::to_string(line) + "| ";

		exceptMsg += sep + text;
		exceptMsg += '\n';

		for (size_t i = 0 ; i < sep.size() + pos ; i++)
			exceptMsg += "~";

		exceptMsg += "^";

		ciof::print(exceptMsg);
		wesi_throw(WESIType_Fatal, exceptMsg.c_str(), false);
	}

	Node Parser::parseElem()
	{
		this->expect('<');

		Node node;
		node.name = this->parseName();

		// If name is not registered, make the following action
		// Throw an exception
		if (!std::count_if(node.name.begin(), node.name.end(), [](unsigned int c) { return std::isspace(c); } ) &&
				!this->nameExists(node.name))
		{
			const int &posSep = 1 + node.name.size();

			const size_t &lineStart = this->pos - posSep;
			const size_t &lineEnd = this->input.find('\n', lineStart);

			this->exception(
					this->line,
					1,
					this->input.substr(lineStart, lineEnd - lineStart)
			);

			return node;
		}

		this->skipWhitespace();

		// Get attributes
		while (std::isalnum(this->peek()))
		{
			const std::string &key = parseName();

			this->skipWhitespace();
			this->expect('=');
			this->skipWhitespace();

			node.attrs[key] = this->parseStr();

			this->skipWhitespace();
		}

		// Self closing
		if (this->peek() == '/')
		{
			this->get();
			this->expect('>');

			return node;
		}

		// Content inside the node
		while (1)
		{
			// Get new line char to add a new line to `Parser::line`
			if (node.text.ends_with('\n'))
				this->line++;

			if (this->peek() == '<' &&
					this->input[pos + 1] == '/') // </
				break;

			if (this->peek() == '<')
				node.children.emplace_back(this->parseElem());

			else node.text += this->get();
		}

		// Closing tag
		this->expect('<'); this->expect('/');
		this->parseName();
		this->expect('>');

		// Get the last line
		if (node.text.ends_with('\n'))
			this->line++;

		return node;
	}

	RegisteredName Parser::getNameID(const std::string &id)
	{
		for (const auto &x : this->regNames)
		{ if (x.id == id) return x; }

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

