#pragma once

#include "ciof.hpp"

#ifndef __CIOF_OK
# 	error "CIOF Is not OK, make sure to not have any errors at compile time"
#endif // NOT : __CIOF_OK

#include <tuple>
#include <string>
#include <sstream>
#include <type_traits>
#include <iostream>
#include <cctype>
#include <array>

namespace ciof
{
	namespace impl
	{
    	// Convert arbitrary types to string
    	template <typename T>
    	std::string toString(const T& _val)
    	{
        	if constexpr (std::is_convertible_v<T, std::string>)
            	return _val;

        	else if constexpr (std::is_arithmetic_v<T>)
            	return std::to_string(_val);

        	else
        	{
            	std::ostringstream oss;
            	oss << _val;

            	return oss.str();
        	}
    	}

    	template <typename T, typename ... Args>
    	std::string parse(T _fmt, Args ... _args)
    	{
        	std::string totalParsed;
        	std::string paramDigits;

        	const std::string fmt = impl::toString(_fmt);

        	auto tup = std::forward_as_tuple(std::forward<Args>(_args) ...);

        	for (size_t i = 0; i < fmt.size(); ++i)
        	{
            	const char c = fmt[i];

				// Get param number
				if (c == '%' && !ciof::outputConf.ignoreAllFormating)
				{
					// If `%%`, ignore
    				if (i + 1 < fmt.size() && fmt[i + 1] == '%')
    				{
        				totalParsed += '%';
        				++i;

        				continue;
    				}

    				size_t j = i + 1;

    				paramDigits.clear();

					// Add digits to string if it is a digit
					// Later, convert the string to a whole digit
    				while (j < fmt.size() && std::isdigit(fmt[j]))
    				{
        				paramDigits.push_back(fmt[j]);
        				++j;
    				}

    				if (!paramDigits.empty())
    				{
						// 1-based index
        				int idx = std::stoi(paramDigits) - 1;

        				bool hasClosingPercent = (j < fmt.size() && fmt[j] == '%');

        				if (hasClosingPercent)
            				++j; // skip the trailing %

        				if (idx >= 0 && idx < (int)sizeof...(Args))
        				{
            				std::apply([&](auto&& ... elems) {
                					std::array<std::string, sizeof...(Args)> arr{ toString(elems)... };
                					totalParsed += arr[idx];
            						}, tup);
        				}

        				i = j - 1; // move loop to the right spot
    				}
				}

				else totalParsed.push_back(c);
        	}

        	return totalParsed;
    	}

    	template <typename T>
    	void __out(const OutputType &_outType, T _t)
    	{
    		switch (_outType)
    		{
				case ciof::OutputType::Err: std::cerr << _t; break;
				case ciof::OutputType::Log: std::clog << _t; break;
				case ciof::OutputType::Out: std::cout << _t; break;
				default: std::cout << _t; break;
    		}
    	}

    	template <typename T, typename ... Args>
		void __out(const OutputType &_outType, T _t, Args ... _args)
		{
    		if (ciof::outputConf.processing)
    		{
        		std::string parsed = impl::parse(_t, _args ...);

        		// If parse returned only the original string and there were extra args, concatenate them
        		if constexpr (sizeof...(Args) > 0)
        		{
            		if (parsed == impl::toString(_t))
                		(parsed.append(impl::toString(_args)), ...);
        		}

        		switch (_outType)
        		{
            		case ciof::OutputType::Err: std::cerr << parsed; break;
            		case ciof::OutputType::Log: std::clog << parsed; break;
            		case ciof::OutputType::Out: std::cout << parsed; break;
            		default: std::cout << parsed; break;
        		}
    		}

 			// Do not throw away args
    		else (ciof::impl::__out(_outType, _t), ..., ciof::impl::__out(_outType, _args));
		}
	}

	// NOTE: OUTPUT
	
	// PRINT
	template <typename T>
	void print(T _t)
	{
		ciof::impl::__out(OutputType::Out, _t);
		std::cout << std::endl;
	}

	template <typename T, typename ... Args>
	void print(T _t, Args ... _args)
	{
		ciof::impl::__out(OutputType::Out, _t, _args ...);
		std::cout << std::endl;
	}

	template <typename T>
	void printLog(T _t)
	{
		ciof::impl::__out(OutputType::Log, _t);
		std::cout << std::endl;
	}

	template <typename T, typename ... Args>
	void printLog(T _t, Args ... _args)
	{
		ciof::impl::__out(OutputType::Log, _t, _args ...);
		std::cout << std::endl;
	}

	template <typename T>
	void printError(T _t)
	{
		ciof::impl::__out(OutputType::Err, _t);
		std::cout << std::endl;
	}

	template <typename T, typename ... Args>
	void printError(T _t, Args ... _args)
	{
		ciof::impl::__out(OutputType::Err, _t, _args ...);
		std::cout << std::endl;
	}

	// ECHO
    template <typename T>
    void echo(T _t)
    {
		ciof::impl::__out(OutputType::Out, _t);
    }

    template <typename T, typename ... Args>
    void echo(T _t, Args ... _args)
    {
		ciof::impl::__out(OutputType::Out, _t, _args ...);
    }

    template <typename T>
    void echoLog(T _t)
    {
		ciof::impl::__out(OutputType::Log, _t);
    }

    template <typename T, typename ... Args>
    void echoError(T _t, Args ... _args)
    {
		ciof::impl::__out(OutputType::Err, _t, _args ...);
    }

    template <typename T>
    void echoError(T _t)
    {
		ciof::impl::__out(OutputType::Err, _t);
    }

    template <typename T, typename ... Args>
    void echoLog(T _t, Args ... _args)
    {
		ciof::impl::__out(OutputType::Log, _t, _args ...);
    }

    // NOTE: UTILS
	template <typename T>
	void input(const std::string &_prompt, T *_var)
	{
		ciof::echo(_prompt);

		std::getline(std::cin, *_var);
	}

	template <typename T>
	void input(T *_var)
	{ std::getline(std::cin, *_var); }

    // NOTE: UTILS

    template <typename T, typename ... Args>
	std::string format(T _t, Args ... _args)
	{
		std::string parsed = ciof::impl::parse(_t, _args ...);

		return parsed;
	}
}

