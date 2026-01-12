#ifndef INCLUDE_CORE_SOLS_PARSERCONF_H_
#define INCLUDE_CORE_SOLS_PARSERCONF_H_

#include <string>

namespace sols
{
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
