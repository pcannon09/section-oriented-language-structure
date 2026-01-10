#include "ciof/ciof.hpp"

#include "../../inc/sols/lib/SOLSParser.hpp"

extern "C"
{
#	include "../../inc/sols/core/wesi/WESI.h"
}

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

			wesi_throw(WESIType_Error, ciof::format("Unexpected token `%1`", chstr).c_str(), true);
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

	Node Parser::parseElem()
	{
		expect('<');

		Node node;
		node.name = this->parseName();

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
			if (this->peek() == '<' &&
					this->input[pos + 1] == '/')
				break;

			if (this->peek() == '<')
				node.children.emplace_back(this->parseElem());
			else node.text += this->get();
		}

		// Closing tag
		this->expect('<'); this->expect('/');
		this->parseName();
		this->expect('>');

		return node;
	}

	std::string Parser::getID() const
	{ return this->id; }
}
