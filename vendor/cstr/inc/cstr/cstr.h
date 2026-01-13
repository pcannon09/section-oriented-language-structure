// cstr.h

#pragma once

#include "CSTRpredefines.h"

#ifdef __CSTR_OK
# 	undef __CSTR_OK
#endif // defined(__CSTR_OK)

#if __STDC_VERSION__ >= CSTR_DEFAULT_C_STD || defined(__cplusplus)

#ifdef CSTR_sys_strdup
# 	undef CSTR_sys_strdup
#endif

#if defined(CSTR_OS_WIN32)
# 	include <string.h>
# 	define CSTR_sys_strdup 		_strdup
#else
# 	ifdef _POSIX_C_SOURCE 
# 		undef _POSIX_C_SOURCE 
# 	endif
# 	define _POSIX_C_SOURCE 200809L
# 	include <string.h>
# 	define CSTR_sys_strdup 		strdup
#endif // defined(CSTR_OS_WIN32)

#include <stdbool.h>
#include <stdlib.h>

#define CSTR_NPOS 					-1
#define CSTR_SUCCESS				0
#define CSTR_FAIL					1
#define CSTR_FORCECAP_LIMIT			2

// NOTE: Dear developer, check all the available macros here
#ifdef CSTR_RECOMMENDED
# 	define CSTR_DYNAMIC_CAPACITY
# 	define CSTR_ENABLE_GET_RETURN
#   define CSTR_ENABLE_GET_CONST_RETURN
#endif // CSTR_RECOMMENDED

// NOTE:
// Other macros that are not recommended but available for flexibility are:
// CSTR_DEFINE_INTERNAL     :       Define the internal files inside of the header file (cstr.h)
//                                  Check the definitions below the definition of the CSTR struct
// CSTR_RECOMMENDED         :       Enable recommended macros, such as CSTR_DYNAMIC_CAPACITY, CSTR_ENABLE_GET_RETURN, ...

/**
 * @brief Custom dynamic string type
 *
 * The fields should typically be managed only through the cstr_* API
 * Directly modifying them can lead to undefined behavior unless explicitly intended
 */
typedef struct
{
    /**
     * @brief Tracks whether this CSTR has been initialized
     * @note Should only be set by internal or initialization functions
     */
    bool initialized;

    /**
     * @brief Pointer to the character buffer
     * Always NULL terminated if properly managed through the API
     */
    char *data;

    /**
     * @brief If true, capacity is fixed and operations that exceed
     * it return an error instead of reallocating the size
     */
    bool forceCap;

    /**
     * @brief Current string length (not counting the NULL terminator)
     */
    size_t len;

    /**
     * @brief Total allocated capacity (including NULL terminator)
     */
    size_t cap;
} CSTR;

#ifdef CSTR_DEFINE_INTERNAL
/**
 * @brief Update the capacity automatically
 * IMPORTANT:
 *      This function won't do anything if macro `CSTR_DYNAMIC_CAPACITY` is NOT defined
 *
 * @return Return if fail or success
 * @note Typically used in the other defined functions for CSTR inner API
 */
int __cstr_updateCap(CSTR *_str);

/**
 * @brief Internal helper to format and set the string content.
 *
 * Uses a printf-style format string and variadic arguments to
 * overwrite the content of a CSTR. The function computes the
 * required buffer size, reallocates if needed (unless forceCap
 * is active), formats the string, and updates length/capacity.
 *
 * @param _str Pointer to the CSTR to modify.
 * @param _fmt printf-style format string.
 * @param ...  Variadic arguments corresponding to the format.
 *
 * @return
 *  - CSTR_SUCCESS on success
 *  - CSTR_FAIL if parameters are invalid, allocation fails,
 *    or vsnprintf reports an error
 *  - CSTR_FORCECAP_LIMIT if forceCap is enabled and the
 *    required size exceeds current capacity
 *
 * @note Typically used in the other defined functions for CSTR inner API
 */
static int __cstr_setFormat(CSTR *_str, const char *_fmt, ...);
#endif // defined(CSTR_DEFINE_INTERNAL)

/**
 * @brief Initialize a new empty CSTR
 *
 * @return A new CSTR instance. Check `.initialized` or if CSTR is not NULL to confirm success
 */
CSTR cstr_init(void);

/**
 * @brief Initialize a CSTR with a copy of an original C-string
 *
 * @param _dest Pointer to the destination CSTR
 * @param _src NULL terminated string to copy from
 * @return CSTR_SUCCESS on success, otherwise CSTR_FAIL
 */
int cstr_initCopy(CSTR *_dest, const char *_src);

/**
 * @brief Destroy and clean up a CSTR and its allocated objects
 *
 * @param _str Pointer to the CSTR to destroy
 * @return CSTR_SUCCESS on success, otherwise CSTR_FAIL
 */
int cstr_destroy(CSTR *_str);

/**
 * @brief Set the content of a CSTR to a new C-string in `.data`
 *
 * @param _str Pointer to the CSTR to modify
 * @param _data Null terminated string to set
 * @return CSTR_SUCCESS, CSTR_FAIL, or CSTR_FORCECAP_LIMIT
 */
int cstr_set(CSTR *_str, const char *_data);

/**
 * @brief Append a suffix to the end of a CSTR
 *
 * @param _str Pointer to the CSTR to modify
 * @param _suffix NULL terminated string to append
 * @return CSTR_SUCCESS, CSTR_FAIL, or CSTR_FORCECAP_LIMIT
 */
int cstr_add(CSTR *_str, const char *_suffix);

/**
 * @brief Extract a substring in place from a CSTR
 *
 * @param _str Pointer to the CSTR to modify
 * @param _start Starting index of the substring
 * @param _len Length of the substring
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_substr(CSTR *_str, size_t _start, size_t _len);

/**
 * @brief Clear a CSTR to an empty string without freeing memory
 *
 * @param _str Pointer to the CSTR to clear
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_clear(CSTR *_str);

/**
 * @brief Convert all characters in a CSTR to uppercase
 *
 * @param _str Pointer to the CSTR to modify
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_upper(CSTR *_str);

/**
 * @brief Convert all characters in a CSTR to lowercase
 *
 * @param _str Pointer to the CSTR to modify
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_lower(CSTR *_str);

/**
 * @brief Insert a string at a specific position in a CSTR
 *
 * @param _str Pointer to the CSTR to modify
 * @param _data NULL terminated string to insert
 * @param _pos Position at which to insert, 0-based
 * @return CSTR_SUCCESS, CSTR_FAIL, or CSTR_FORCECAP_LIMIT
 */
int cstr_insert(CSTR *_str, const char *_data, size_t _pos);

/**
 * @brief Reverse the content of a CSTR in place
 *
 * @param _str Pointer to the CSTR to modify
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_reverse(CSTR *_str);

/**
 * @brief Clear the content of a CSTR and optionally shrink capacity
 *
 * @param _str Pointer to the CSTR to shrink
 * @param _len New length (characters to keep)
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_shrink(CSTR *_str, const size_t _len);

/**
 * @brief Create a numeric string from an integer
 */
int cstr_fromInt(CSTR *_str, int _val);

/**
 * @brief Create a numeric string from a long
 */
int cstr_fromLong(CSTR *_str, long _val);

/**
 * @brief Create a numeric string from a float
 */
int cstr_fromFloat(CSTR *_str, float _val);

/**
 * @brief Create a numeric string from a double
 */
int cstr_fromDouble(CSTR *_str, double _val);

/**
 * @brief Create a hexadecimal string from an unsigned integer
 */
int cstr_fromHex(CSTR *_str, unsigned int _val);

/**
 * @brief Replace only the first occurrence of a substring
 *
 * @param _str Pointer to the CSTR
 * @param _old Substring to locate
 * @param _new Replacement string
 * @return CSTR_SUCCESS or an error code (CSTR_FAIL, ...)
 */
int cstr_replace(CSTR *_str, const char *_old, const char *_new);

/**
 * @brief Replace all occurrences of a substring
 *
 * @param _str Pointer to the CSTR
 * @param _old Substring to locate
 * @param _new Replacement string
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_replaceAll(CSTR *_str, const char *_old, const char *_new);

/**
 * @brief Erase a portion of the string
 *
 * @param _str Pointer to the CSTR
 * @param _pos Starting position
 * @param _len Number of characters to remove
 * @return CSTR_SUCCESS or CSTR_FAIL
 */
int cstr_erase(CSTR *_str, int _pos, size_t _len);

/**
 * @brief Find the first occurrence of a substring
 *
 * @param _str Pointer to the CSTR to search
 * @param _find Substring to locate
 * @return Index of first match, or CSTR_NPOS if not found
 */
size_t cstr_find(const CSTR *_str, const char *_find);

/**
 * @brief Find a substring starting from a specific index
 *
 * @param _str Pointer to the CSTR to search
 * @param _find Substring to locate
 * @param pos Starting index
 * @return Index of match, or CSTR_NPOS if not found
 */
size_t cstr_findFrom(const CSTR *_str, const char *_find, size_t pos);

/**
 * @brief Count occurrences of a single character within a CSTR
 *
 * Iterates through the string and counts how many times the specified
 * character appears in the `.data` buffer.
 *
 * @param _str Pointer to the CSTR to inspect
 * @param ch Character to count
 *
 * @return Number of occurrences of `ch` in the string
 *         Returns 0 if `_str` is NULL or not initialized
 *
 * @note This function performs a linear scan across all characters.
 */
size_t cstr_countChar(const CSTR *_str, const char ch);

/**
 * @brief Count occurrences of a substring within a CSTR
 *
 * Searches for non-overlapping occurrences of a null-terminated substring
 * inside the CSTR’s `.data` buffer using strstr. Each found instance advances
 * the scan position by the length of the substring, preventing overlapping matches.
 *
 * @param _str Pointer to the CSTR to inspect
 * @param ch   Null-terminated substring to count
 *
 * @return Number of non-overlapping occurrences found
 *         Returns 0 if parameters are invalid (`_str`, `.data`, `ch`, or `*ch`)
 */
size_t cstr_count(const CSTR *_str, const char *ch);

/**
 * @brief Check if a CSTR begins with the given prefix
 *
 * @param _str The CSTR to check by value
 * @param _prefix The prefix string
 * @return true if it starts with prefix, else return false
 */
bool cstr_startsWith(CSTR _str, const char *_prefix);

/**
 * @brief Check if a CSTR ends with the given suffix
 *
 * @param _str The CSTR to check (by value)
 * @param _suffix The suffix string
 * @return true if it ends with suffix, false otherwise
 */
bool cstr_endsWith(const CSTR _str, const char *_suffix);

/**
 * @brief Compare two CSTR instances for equality
 *
 * @param _s1 First CSTR (by value)
 * @param _s2 Second CSTR (by value)
 * @return true if equal, false otherwise
 */
bool cstr_comp(const CSTR _s1, const CSTR _s2);

/**
 * @brief Return "true" or "false" based on a boolean value
 *
 * @param _bool Boolean value
 * @return "true" if _bool is true, otherwise "false".
 */
const char *cstr_bool(const bool _bool);

/**
 * @brief Get if the string is empty 
 * Return true if:
 * `.len` smaller or equal to 0
 * `.data` is empty ("")
 * `.initialized` is false
 * `_str` is NULL
 * If none of the requirements are met, means that it's not empty, hence, return false
 *
 * @return Is the string empty? Yes (true), No (false)
 */
bool cstr_empty(const CSTR *_str);

#define __CSTR_OK

#ifdef __CSTR_OK
# 	ifdef CSTR_ENABLE_GET_RETURN
# 		define CSTR_GET_RETURN(_cstr, _call, ...) ((_call(&_cstr,  __VA_ARGS__)), _cstr)
# 	else
# 		define CSTR_GET_RETURN(_cstr, _call, ...)
# 	endif // defined(CSTR_ENABLE_GET_RETURN)

# 	ifdef CSTR_CONST_RETURN_USE_PTR_METHOD
# 		define CSTR_CONST_RETURN_PTR_METHOD ->
# 	else
# 		define CSTR_CONST_RETURN_PTR_METHOD .
# 	endif

# 	ifdef CSTR_ENABLE_GET_CONST_RETURN
#       define CSTR_GET_CONST_RETURN(_str, _call, ...) \
        ({ \
            CSTR tmpS; \
            cstr_initCopy(&tmpS, _str CSTR_CONST_RETURN_PTR_METHOD data); \
            _call(&tmpS, __VA_ARGS__); \
            tmpS; \
        })
# 	else
# 		define CSTR_GET_CONST_RETURN(_str, _call, ...)
# 	endif // defined(CSTR_ENABLE_GET_CONST_RETURN)
#endif // __CSTR_OK

#else
# 	error "Must use C11 as the minimum standard"
#endif // __STDC_VERSION__

