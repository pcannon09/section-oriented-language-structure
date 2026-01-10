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
		Message,
		Note
	};

	typedef struct ParseMessage
	{
		ParserMessageType type;

		int code;
		int line;
		int col;

		std::string message;
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
