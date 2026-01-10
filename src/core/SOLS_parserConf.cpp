#include "../../inc/sols/core/SOLS_parserConf.hpp"

namespace sols
{
	std::string getTokTypeName(const TokType &type)
	{
		switch (type)
		{
			case TokType::OpenSection: 			return "<";
			case TokType::CloseSection: 		return "</";
			case TokType::EndSection: 			return ">";
			case TokType::EndCloseSection: 		return "/>";
			case TokType::Identifier: 			return "_id_";
			case TokType::String: 				return "\"";
			case TokType::Equals: 				return "=";
			case TokType::OpenComment: 			return "<<";
			case TokType::CloseComment: 		return ">>";
			case TokType::EOFTok: 				return "_eof_";
			default: return " "; // Leave a whitespace so the program does not crash unexpectly
		}
	}
}

