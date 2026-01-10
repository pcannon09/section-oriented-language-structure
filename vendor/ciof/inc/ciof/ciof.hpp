#pragma once

#include "CIOFpredefines.hpp"

#if __CIOF_CPLUSPLUS >= CIOF_DEFAULT_CPP_STD

#define __CIOF_OK

#include <string>

// This must be after all 
#ifdef CIOF_OS_WIN32
# 	ifdef __out
# 		undef __out
# 	endif
#endif

namespace ciof
{
	/**
	 * @brief Set output configuration for ciof::impl::__out() function and its wrappers
	 */
	typedef struct CIOFOutputConfig
	{
		// Add a space between parameter automaticalliy
		bool spacing = false;
		
		// Ignore custom and predefined formatting keywords
		bool ignoreAllFormating = false;

		// Process config
		bool processing = true;

		// Set delimiter for spacing
		std::string spacingDelimeter = " ";
	} CIOFOutputConfig;

	/**
	 * @brief Set output type for ciof::impl::__out() function and its helper functions
	 * Out : To Standard Output
	 * Err : To Standard Error
	 * Log : To Standard Log (Error, unbuffered)
	 */
	enum class OutputType
	{
		Out, // std::cout
		Err, // std::cerr
		Log, // std::clog
	};

	/**
	 * @brief Set output style:
	 * Bold
	 * Italic
	 * Underline
	 * Strikethrough
	 */
	enum class OutputStyle
	{
		Bold,
		Italic,
		Underline,
		Strikethrough
	};

	/**
	 * @brief Set default output configuration
	 */
	extern CIOFOutputConfig outputConf;

	/**
	 * @brief Implementation functions
	 */
	namespace impl
	{
		/**
		 * @brief Convert to string safely
		 * @param _val any convertable type to string
		 * @return std::string total converted strig
		 */
    	template <typename T>
    	std::string toString(const T &_val);

		/**
		 * @brief Parse the params for printing and echoing
		 * @return _fmt parameter 1 to parse
		 * @return _args parameter 2 to the last one to parse
		 * @return std::string Total parsed
		 */
    	template <typename T, typename ... Args>
    	std::string parse(T _fmt, Args ... _args);

		/**
		 * @brief Output the with its corresponding type either to:
		 * - Standard output
		 * - Standard error
		 * - Standard log (AKA: Error but unbuffered)
		 * @param _outType Type for OUT, ERR or LOG
		 * @param _t Text to add
		 */
    	template <typename T>
    	void __out(const OutputType &_outType, T _t);

		/**
		 * @brief Output the with its corresponding type either to:
		 * - Standard output
		 * - Standard error
		 * - Standard log (AKA: Error but unbuffered)
		 * @param _outType Type for OUT, ERR or LOG
		 * @param _t Text to add
		 * @param _args More text to add
		 */
    	template <typename T, typename ... Args>
    	void __out(const OutputType &_outType, T _t, Args ... _args);
    }

	// NOTE: OUTPUT
	
	// PRINT
	/**
	 * @brief Print a new line
	 */
	void print();

	/**
	 * @brief Print to the standard output for `_t`
	 * @param _t Message to print
	 */
	template <typename T>
	void print(T _t);

	/**
	 * @brief Print to the standard output for `_t`
	 * @param _t Message to print
	 * @param _args More messages to add
	 */
	template <typename T, typename ... Args>
	void print(T _t, Args ... _args);

	/**
	 * @brief Print to the standard log for `_t`
	 * @param _t Message to print
	 */
	template <typename T>
	void printLog(T _t);

	/**
	 * @brief Print to the standard log for `_t`
	 * @param _t Message to print
	 * @param _args More messages to add
	 */
	template <typename T, typename ... Args>
	void printLog(T _t, Args ... _args);

	/**
	 * @brief Print to the standard error for `_t`
	 * @param _t Message to print
	 */
	template <typename T>
	void printError(T _t);

	/**
	 * @brief Print to the standard error for `_t`
	 * @param _t Message to print
	 * @param _args More messages to add
	 */
	template <typename T, typename ... Args>
	void printError(T _t, Args ... _args);

	// ECHO
	/**
	 * @brief Echo to the standard output for `_t`
	 * @param _t Message to print
	 */
	template <typename T>
	void echo(T _t);

	/**
	 * @brief Echo to the standard output for `_t`
	 * @param _t Message to print
	 * @param _args More messages to add
	 */
	template <typename T, typename ... Args>
	void echo(T _t, Args ... _args);

	/**
	 * @brief Echo to the standard log for `_t`
	 * @param _t Message to print
	 */
    template <typename T>
    void echoLog(T _t);

	/**
	 * @brief Echo to the standard log for `_t`
	 * @param _t Message to print
	 * @param _args More messages to add
	 */
    template <typename T, typename ... Args>
    void echoLog(T _t, Args ... _args);

	/**
	 * @brief Echo to the standard error for `_t`
	 * @param _t Message to print
	 */
    template <typename T>
    void echoError(T _t);

	/**
	 * @brief Echo to the standard log for `_t`
	 * @param _t Message to print
	 * @param _args More messages to add
	 */
    template <typename T, typename ... Args>
    void echoError(T _t, Args ... _args);

	// NOTE: INPUT
	/**
	 * @brief Get full input from the user with a prompt
	 * @param _prompt Prompt to user
	 * @param _var Variable to store the value
	 */
	template <typename T>
	void input(const std::string &_prompt, T *_var);

	/**
	 * @brief Get full input from the user
	 * @param _prompt Prompt to user
	 * @param _var Variable to store the value
	 */
	template <typename T>
	void input(T *_var);

	// NOTE: UTILS
	/**
	 * @brief Get everything as a string format
	 * @param _t First param of the string
	 * @param _args Rest of the params of the string
	 * @return std::string Return the formatted string
	 */
    template <typename T, typename ... Args>
	std::string format(T _t, Args ... _args);

	/**
	 * @brief Set the styles according to the `OutputStyle` type
	 * @param _style Set the style (Bold, Italics, Underline, ...)
	 * @return std::string Escape code according to the style
	 */
	std::string styleSet(const OutputStyle &_style);

	/**
	 * @brief Reset all the styles and colors in the terminal
	 * @return std::string Escape code to ereset colors and styles from terminal
	 */
	std::string styleReset();

	/**
	 * @brief Set the colors from the default color palette from the terminal
	 * @param _color Integer for color
	 * @return std::string Escape code to set the code
	 */
	std::string colorSet(int _color);

	/**
	 * @brief Set the RGB color in the terminal
	 * @param r Red color
	 * @param g Green color
	 * @param b Blue color
	 * @return std::string Color escape sequence for RGB colors
	 */
	std::string rgbSet(unsigned int r, unsigned int g, unsigned int b);

	/**
	 * @brief Initialize ANSI text option in Windows 10 or later
	 * NOTE: ONLY WORKS IN WINDOWS 10 OR LATER
	 */
	void initANSI();

	/**
	 * @brief Get the set cursor position using _row and _col
	 * @param _row Row to set (X)
	 * @param _col Column to set (Y)
	 * @return std::string Return the string to set the position using ANSI
	 */
	std::string getCursorPos(int _row, int _col);

	/**
	 * @brief Get the set cursor position using _row and _col
	 * Essentially calls the `getCursorPos()` function and then sets the row and col according to the `getCursorPos()` function
	 * @param _row Row to set (X)
	 * @param _col Column to set (Y)
	 */
	void cursorPos(int _row, int _col);
}

#include "ciof.ipp"

#else
# 	error "Cannot compile with the current C++ standard"
# 	pragma message("Use C++ " CIOF_TOSTRING(CIOF_DEFAULT_CPP_STD) " or higer instead of " CIOF_TOSTRING(__cplusplus))
#endif // __cplusplus

