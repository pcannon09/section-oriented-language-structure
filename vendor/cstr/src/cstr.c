// cstr.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>

#include "../inc/cstr/cstr.h"

/* INNER FUNCTIONS */

int __cstr_updateCap(CSTR *_str)
{
	if (!_str) return CSTR_FAIL;

	if (_str->forceCap)
		return CSTR_FAIL;

	// Set the dynamic capacity in CSTR type
#ifdef CSTR_DYNAMIC_CAPACITY
	_str->cap = _str->len + 1;
#endif // defined(CSTR_DYNAMIC_CAPACITY)

	return CSTR_SUCCESS;
}

static int __cstr_setFormat(CSTR *_str, const char *_fmt, ...)
{
	if (!_str || !_fmt) return CSTR_FAIL;

	va_list args;
	va_start(args, _fmt);

	int needed = vsnprintf(NULL, 0, _fmt, args);

	if (needed < 0) return CSTR_FAIL;

	// Make room for forceCap if enabled
	if (_str->forceCap && (size_t)(needed + 1) > _str->cap)
		return CSTR_FORCECAP_LIMIT;

	// forceCap is `false` when here
	char *newData = realloc(_str->data, needed + 1);
	
	if (!newData)
	{
		va_end(args);

		return CSTR_FAIL;
	}

	_str->data = newData;
	_str->cap = needed + 1;

	va_end(args);

	// Format
	va_start(args, _fmt);
	vsnprintf(_str->data, _str->cap, _fmt, args);
	va_end(args);

	_str->len = needed;

	return CSTR_SUCCESS;
}

/* OTHER FUNCTIONS */

CSTR cstr_init(void)
{
    const char *initData = "";
    const unsigned int initialLen = strlen(initData); // 0

    CSTR str;

    str.data = malloc(initialLen + 1);  // allocate space for null terminator

    if (!str.data)
    {
        str.initialized = false;

        return str;
    }

    // Initialize the string data with an empty string
    str.data[0] = '\0';

    str.len = initialLen;         // Should be: 0
    str.cap = initialLen + 1;     // Should be: 1
    str.forceCap = false;
    str.initialized = true;

    return str;
}

int cstr_destroy(CSTR *_str)
{
	// Fail directly if the `_str` pointer is NULL
    if (!_str || !_str->data)
    	return CSTR_FAIL;

    free(_str->data);

    _str->data = NULL;
    _str->len = 0;
    _str->cap = 0;
    _str->initialized = false;
    _str->forceCap = false;

    return CSTR_SUCCESS;
}

int cstr_set(CSTR *_str, const char *_data)
{
    if (!_str || !_data) return CSTR_FAIL;

    size_t newLen = strlen(_data);

    // Prevent overflow if forceCap is true
    if (_str->forceCap && newLen >= _str->cap)
        return CSTR_FORCECAP_LIMIT;

    char *newData;

    if (_str->forceCap) newData = _str->data; // Use existing buffer
    else
    {
        newData = realloc(_str->data, newLen + 1); // Reallocate to fit new string

        if (!newData) return CSTR_FAIL;

        _str->data = newData;

    	__cstr_updateCap(_str);
    }

    strcpy(_str->data, _data);

    _str->len = newLen;

    __cstr_updateCap(_str);

    return CSTR_SUCCESS;
}

int cstr_add(CSTR *_str, const char *_suffix)
{
    if (!_str || !_suffix) return CSTR_FAIL;

    size_t suffixLen = strlen(_suffix);
    size_t newLen = _str->len + suffixLen;

    // Prevent overflow if forceCap is true
    if (_str->forceCap && newLen >= _str->cap) 
        return CSTR_FORCECAP_LIMIT;

    if (!_str->forceCap) 
    {
        char *newData = realloc(_str->data, newLen + 1); // + 1 for NULL terminator
        if (!newData) return CSTR_FAIL;

        _str->data = newData;

    	__cstr_updateCap(_str);
    }

    // Safe concatenation including null terminator
    memcpy(_str->data + _str->len, _suffix, suffixLen + 1); // +1 to copy null terminator

    _str->len = newLen;

    __cstr_updateCap(_str);

    return CSTR_SUCCESS;
}

int cstr_clear(CSTR *_str)
{
	if (!_str || !_str->data)
		return CSTR_FAIL;

	_str->data[0] = '\0';
	_str->len = strlen(_str->data);

	return CSTR_SUCCESS;
}

int cstr_upper(CSTR *_str)
{
	if (!_str || !_str->data)
		return CSTR_FAIL;

	for (size_t i = 0 ; i < _str->len ; i++)
		_str->data[i] = toupper((unsigned char)_str->data[i]);

	return CSTR_SUCCESS;
}

int cstr_lower(CSTR *_str)
{
	if (!_str || !_str->data)
		return CSTR_FAIL;

	for (size_t i = 0 ; i < _str->len ; i++)
		_str->data[i] = tolower((unsigned char)_str->data[i]);

	return CSTR_SUCCESS;
}

int cstr_insert(CSTR *_str, const char *_data, size_t _pos)
{
	if (!_str || !_data) return CSTR_FAIL;
	if (_pos > _str->len) return CSTR_FAIL;

	size_t addLen = strlen(_data);
	size_t newLen = _str->len + addLen;

	if (_str->forceCap && newLen >= _str->cap) return CSTR_FORCECAP_LIMIT;

	// If not `forceCap`, allow to add the `_pos` index to the new index
	if (!_str->forceCap)
	{
		char *newData = realloc(_str->data, newLen + 1);

		if (!newData) return CSTR_FAIL;

		_str->data = newData;
	}

	memmove(_str->data + _pos + addLen, _str->data + _pos, _str->len - _pos + 1); // + 1 for NULL terminator
	memcpy(_str->data + _pos, _data, addLen);

	_str->len = newLen;

    __cstr_updateCap(_str);

	return CSTR_SUCCESS;
}

int cstr_reverse(CSTR *_str)
{
	if (!_str || !_str->data)
		return CSTR_FAIL;

	int l = 0; // left
	int r = strlen(_str->data) - 1; // right
									// Also remove the NULL terminator from there to get the correct size of the string

	char t;

	while (l < r)
	{
		t = _str->data[l];  				// Set single char to the data in `l` index
		_str->data[l] = _str->data[r]; 		// data in index `l` will be set to the data from `r` index
		_str->data[r] = t; 					// Set the data from `r` index to the previously set `t` char

		// Move index values towards each other
		l++;
		r--;
	}

	return CSTR_SUCCESS;
}

int cstr_substr(CSTR *_str, size_t _start, size_t _len)
{
	if (!_str || _start >= _str->len)
		return CSTR_FAIL;

	size_t actualLen = (_start + _len > _str->len) ? _str->len - _start : _len;

	char *buff = malloc(actualLen + 1); // Add + 1 for the NULL terminator

	if (!buff) return CSTR_FAIL;

	memcpy(buff, _str->data + _start, actualLen);
	buff[actualLen] = '\0';

	free(_str->data); _str->data = NULL;

	_str->data = buff;
	_str->len = actualLen;
	_str->cap = actualLen + 1;

	return CSTR_SUCCESS;
}

int cstr_initCopy(CSTR *_dest, const char *_src)
{
	if (!_dest || !_src) return CSTR_FAIL;

	size_t len = strlen(_src);
	_dest->data = malloc(len + 1);

	if (!_dest->data) return CSTR_FAIL;

	memcpy(_dest->data, _src, len + 1);

	_dest->len = len;
	_dest->cap = len + 1;

	return CSTR_SUCCESS;
}

int cstr_replace(CSTR *_str, const char *_old, const char *_new)
{
    if (!_str) return CSTR_FAIL;

    size_t oldPosFound = cstr_find(_str, _old);

    if (oldPosFound != CSTR_SUCCESS) return oldPosFound;

	// MAIN
    CSTR totalStr; 	cstr_initCopy(&totalStr, _str->data);
    CSTR newStr; 	cstr_initCopy(&newStr, _str->data);

    unsigned int oldRet = cstr_substr(&totalStr, 0, oldPosFound);

    if (oldRet) { printf("cstr_replace() 1: Error calling cstr_substr(), code: %u\n", oldRet); exit(oldRet); }

    // Only compute the `rest` if there is a `rest`
    size_t restStart = oldPosFound + strlen(_old);

	// If the start of `rest` is smaller than the length of newStr
    if (restStart < newStr.len)
    {
        unsigned int newRet = cstr_substr(&newStr, restStart, newStr.len);

        if (newRet) { printf("cstr_replace() 2: Error calling cstr_substr(), code: %u\n", newRet); exit(newRet); }
    }

	// Just clear `newStr` var
    else cstr_set(&newStr, "");

    cstr_add(&totalStr, _new);          // Add replacement
    cstr_add(&totalStr, newStr.data);   // Append rest (may also be empty)

    cstr_destroy(&totalStr);
    cstr_destroy(&newStr);

    return CSTR_SUCCESS;
}

int cstr_replaceAll(CSTR *_str, const char *_old, const char *_new)
{
    if (!_str || !_old || !_new) return CSTR_FAIL;
    if (_old[0] == '\0') return CSTR_FAIL; // avoid infinite loop

    size_t oldLen = strlen(_old);

    CSTR original;
    cstr_initCopy(&original, _str->data);

    CSTR result = cstr_init();
    size_t pos = 0;

    while (1)
    {
        size_t found = cstr_findFrom(&original, _old, pos);

        if (found == CSTR_NPOS) break;

        // Copy everything to `found`
        char saved = original.data[found];
        original.data[found] = '\0';

        cstr_add(&result, original.data + pos);

        original.data[found] = saved;

        // Add replacement
        cstr_add(&result, _new);

        pos = found + oldLen;
    }

    // Add the rest of the original string
    if (pos < original.len)
        cstr_add(&result, original.data + pos);

    // Replace original CSTR with result
    cstr_destroy(_str);
    *_str = result;

    cstr_destroy(&original);

    return CSTR_SUCCESS;
}

int cstr_erase(CSTR *_str, int _pos, size_t _len)
{
	if (!_str) return CSTR_FAIL;

	CSTR foundInPos; cstr_initCopy(&foundInPos, _str->data);

	// Erase between
	cstr_substr(&foundInPos, _pos, _len - _pos + 1); // Remove the remaining space char with the (+ 1) operation
	cstr_replaceAll(_str, foundInPos.data, "");
	cstr_destroy(&foundInPos);

	return CSTR_SUCCESS;
}

int cstr_shrink(CSTR *_str, const size_t _len)
{
	if (!_str) return CSTR_FAIL;

	cstr_substr(_str, 0, _len);

	return CSTR_SUCCESS;
}

int cstr_fromInt(CSTR *_str, int _val)
{ return __cstr_setFormat(_str, "%d", _val); }

int cstr_fromLong(CSTR *_str, long _val)
{ return __cstr_setFormat(_str, "%ld", _val); }

int cstr_fromFloat(CSTR *_str, float _val)
{ return __cstr_setFormat(_str, "%f", _val); }

int cstr_fromDouble(CSTR *_str, double _val)
{ return __cstr_setFormat(_str, "%lf", _val); }

int cstr_fromHex(CSTR *_str, unsigned int _val)
{ return __cstr_setFormat(_str, "%x", _val); }

size_t cstr_find(const CSTR *_str, const char *_find)
{
	if (!_str || !_find || !_str->data) return CSTR_NPOS;

	// Find `_find` (needle) in the haystack (`_str->data`)
	// strstr() function is to find a char inside another char
	char *pos = strstr(_str->data, _find);

	if (!pos) return CSTR_NPOS;

	return pos - _str->data;
}

size_t cstr_countChar(const CSTR *_str, const char ch)
{
	if (!_str || !_str->initialized)
		return 0;

	size_t foundChs = 0;

	for (size_t i = 0 ; i < _str->len ; i++)
	{
		if (_str->data[i] == ch) foundChs++;
	}

	return foundChs;
}

size_t cstr_count(const CSTR *_str, const char *ch)
{
    if (!_str || !_str->data || !ch || !*ch)
        return 0;

    size_t count = 0;
    const char *p = _str->data;
    size_t len = strlen(ch);

    while ((p = strstr(p, ch)) != NULL)
    {
        count++;
        p += len;
    }

    return count;
}

size_t cstr_findFrom(const CSTR *_str, const char *_find, size_t pos)
{
	if (!_str || !_find) return CSTR_NPOS;

	size_t strLen = _str->len;
	size_t subLen = strlen(_find);

	if (subLen == 0 || pos >= strLen) return CSTR_NPOS;

	for (size_t i = pos ; i <= strLen - subLen ; i++)
	{
		size_t j;

		for (j = 0 ; j < subLen ; j++)
		{
			if (_str->data[i + j] != _find[j]) break;
		} // for() loop 2

		if (j == subLen) return i; // Found match
	} // for() loop 1

	// FAIL
	return CSTR_NPOS;
}

bool cstr_startsWith(CSTR str, const char *prefix)
{
    if (!str.data || !prefix) return false;

    size_t plen = strlen(prefix);

    if (plen > str.len) return false;

    return strncmp(str.data, prefix, plen) == 0;
}

bool cstr_endsWith(CSTR str, const char *suffix)
{
    if (!str.data || !suffix) return false;

    size_t slen = strlen(suffix);

    if (slen > str.len) return false;

    return strncmp(str.data + (str.len - slen), suffix, slen) == 0;
}

bool cstr_comp(const CSTR _s1, const CSTR _s2)
{
	if (strcmp(_s1.data, _s2.data) == 0) return true;
	
	return false;
}

bool cstr_empty(const CSTR *_str)
{
	if (!_str->initialized) 				return true;
	if (!_str)  							return true;
	if (!_str->data) 						return true;
	if (_str->len <= 0) 					return true;

	return false;
}

const char *cstr_bool(const bool _bool)
{ return (_bool) ? "true" : "false"; }

