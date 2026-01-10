#ifndef INCLUDE_LIB_SOLSREADER_HPP_
#define INCLUDE_LIB_SOLSREADER_HPP_

#include <string>
#include <vector>
#include <map>

namespace sols
{
	typedef struct Node 
	{
		std::string name;
		std::string text;

		std::map<std::string, std::string> attrs;
		std::vector<Node> children;
	} Node;

	typedef std::pair<bool, std::string> InitInfoError;

	class Parser 
	{
	private:
		std::string id;
		std::string input;

		size_t pos;

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

		std::string parseName();
		std::string parseStr();

		std::string getID() const;
	};
}

#endif  // INCLUDE_LIB_SOLSREADER_HPP_
