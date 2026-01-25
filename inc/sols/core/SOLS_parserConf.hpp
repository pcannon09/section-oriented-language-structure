#ifndef INCLUDE_CORE_SOLS_PARSERCONF_H_
#define INCLUDE_CORE_SOLS_PARSERCONF_H_

#include <string>
#include <vector>
#include <functional>
#include <map>

namespace sols
{
	class Function;

	namespace predefined
	{
		// I{varName}; 
		// I stands for `Internal`
		inline constexpr std::string varName = "I_Name";
	}

	enum class TokType
	{
		OpenSection, 			// <
		CloseSection, 			// </
		EndSection, 			// >
		EndCloseSection, 		// />
		Identifier, 			// Tag or attr name
		String, 				// "value"
		Equals, 				// =
		OpenComment,
		CloseComment,
		EOFTok
	};
	
	enum class ParserMessageType
	{
		Error,
		Warning,
		Message,
		Note
	};

	enum class SOLS_Bool
	{
		False,
		True,
		None
	};

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

		Node node;
		Function *function = nullptr;
	} RegisterCommand;

	typedef struct ParseMessage
	{
		ParserMessageType type;

		std::pair<unsigned int, unsigned int> lineRange;

		int code;
		int line;
		int col;
		int command;

		std::string message;
		std::string file;

		std::pair<
			std::string,
			std::function<ParseMessage(const RegisterCommand &command, const std::vector<std::string>&)>
			> call;
	} ParseMessage;

	template <typename T>
	struct ReturnData
	{
		ParseMessage messageType;

		T ret;
	};

	std::string getTokTypeName(const TokType &type);
}

#endif  // INCLUDE_CORE_SOLS_PARSERCONF_H_
