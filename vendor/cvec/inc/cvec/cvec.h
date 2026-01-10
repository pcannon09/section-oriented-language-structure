#pragma once

#include "CVECpredefines.h"

#ifdef CVEC_sys_strdup
# 	undef CVEC_sys_strdup
#endif

#if defined(CVEC_OS_WIN32)
# 	include <string.h>
# 	define CVEC_sys_strdup 		_strdup
#else
# 	ifdef _POSIX_C_SOURCE 
# 		undef _POSIX_C_SOURCE 
# 	endif
# 	define _POSIX_C_SOURCE 200809L
# 	include <string.h>
# 	define CVEC_sys_strdup 		strdup
#endif // defined(CVEC_OS_WIN32)

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CVEC_NPOS 						-1
#define CVEC_SUCCESS 					0
#define CVEC_FAIL 						1
#define CVEC_FAIL_ALLOCATION 			2  		// Or reallocation
#define CVEC_FORCECAP_FAIL 				3
#define CVEC_ELEM_NOT_FOUND 			4
#define CVEC_GET_NULL  					5
#define CVEC_OUT_BOUNDS  				6
#define CVEC_NOT_ENOUGH_CAP 			7

#define __CVEC_CAP_ADDITION 			1

#define __CVEC_SET_NULL(vec, ptrSym) \
	vec ptrSym size = 0;             \
	vec ptrSym cap = 0;              \
	vec ptrSym elemLen = 0;          \
	vec ptrSym forceCap = false;     \
	vec ptrSym dynamicCap = false;    \
	vec ptrSym initialized = false   \

/**
 * @brief Custom generic dynamic vector type
 *
 * CVEC is a flexible container that stores a sequence of elements of uniform size.
 * It behaves similarly to std::vector in C++, supporting dynamic resizing,
 * insertion, deletion, merging, reversing, and splitting.
 *
 * @note Direct modification of structure fields should be avoided — prefer CVEC API usage.
 */
typedef struct
{
    bool initialized;       /**< Tracks whether this CVEC has been properly initialized */
    bool __usedSplit;       /**< Internal flag used by split operations */
    void *data;             /**< Pointer to contiguous allocated memory storing array elements */
    bool forceCap;          /**< Prevents capacity expansion if true; insertion fails when full */
    bool dynamicCap;        /**< Enables automatic capacity growth when true */
    size_t size;            /**< Current number of stored items */
    size_t cap;             /**< Allocated storage capacity */
    size_t elemLen;         /**< Size in bytes of each element */
} CVEC;

// PUBLIC //

/**
 * @brief Initialize a new vector with requested capacity and element size
 *
 * @param _cap Initial capacity (number of elements, not bytes)
 * @param _elemSize Size in bytes of each element
 * @return A fully initialized CVEC instance
 */
CVEC cvec_init(int _cap, size_t _elemSize);

/**
 * @brief Create a deep copy of an existing CVEC
 *
 * Copies memory, capacity, element properties, and flags.
 *
 * @param _src Pointer to the original CVEC
 * @return A new CVEC instance with copied data
 */
CVEC cvec_initCopy(const CVEC *_src);

/**
 * @brief Clear all elements and reset size to zero without freeing memory
 *
 * @param _vec Pointer to the vector
 * @return CVEC_SUCCESS on success, else CVEC_FAIL
 */
int cvec_emptyAll(CVEC *_vec);

/**
 * @brief Destroy a CVEC, freeing allocated memory and invalidating the structure
 *
 * @param _vec Pointer to the CVEC to destroy
 * @return CVEC_SUCCESS on success, or CVEC_FAIL
 */
int cvec_destroy(CVEC *_vec);

/**
 * @brief Merge contents of `_input` vector into `_toMerge`
 *
 * Appends all items from `_input` to `_toMerge`, expanding capacity if allowed.
 *
 * @param _toMerge Destination vector to merge into
 * @param _input Vector to merge from
 * @return CVEC_SUCCESS or error code (CVEC_FAIL, CVEC_NOT_ENOUGH_CAP, ...)
 */
int cvec_merge(CVEC *_toMerge, const CVEC *_input);

/**
 * @brief Remove an element at a specific index
 *
 * Shifts trailing elements left by one.
 *
 * @param _vec Pointer to vector
 * @param _index Index to remove
 * @return CVEC_SUCCESS, CVEC_OUT_BOUNDS, or CVEC_FAIL
 */
int cvec_remove(CVEC *_vec, const size_t _index);

/**
 * @brief Remove the last element
 *
 * @param _vec Vector to modify
 * @return CVEC_SUCCESS or CVEC_FAIL
 */
int cvec_popBack(CVEC *_vec);

/**
 * @brief Remove the first element, shifting all others left
 *
 * @param _vec Vector to modify
 * @return CVEC_SUCCESS or CVEC_FAIL
 */
int cvec_popFront(CVEC *_vec);

/**
 * @brief Reverse the order of elements in place
 *
 * @param _vec Pointer to CVEC
 * @return CVEC_SUCCESS or CVEC_FAIL
 */
int cvec_reverse(CVEC *_vec);

/**
 * @brief Shrink allocated memory to match current size
 *
 * @param _vec Pointer to vector
 * @return CVEC_SUCCESS or CVEC_FAIL_ALLOCATION
 */
int cvec_shrink(CVEC *_vec);

/**
 * @brief Split a string by delimiter and store parts inside CVEC
 *
 * Internal `__usedSplit` tracks cleanup ownership.
 *
 * @param _vec Destination vector
 * @param _str Input string to tokenize
 * @param _del Delimiter string
 * @return CVEC_SUCCESS or CVEC_FAIL
 */
int cvec_split(CVEC *_vec, char *_str, const char *_del);

/**
 * @brief Check whether element exists at index
 *
 * @param _vec Vector to inspect
 * @param _index Element index
 * @return true if valid index, false otherwise
 */
bool cvec_at(const CVEC *_vec, const size_t _index);

/**
 * @brief Check whether element exists within vector capacity
 *
 * @param _vec Vector to inspect
 * @param _index Index inside original allocated capacity
 * @return true if index < capacity, else false
 */
bool cvec_atCap(const CVEC *_vec, const size_t _index);

/**
 * @brief Get a pointer to an element at specified index
 *
 * @param _vec Vector to inspect
 * @param _index Position to access
 * @return Pointer to element or NULL if invalid
 */
void *cvec_get(const CVEC *_vec, const size_t _index);

/**
 * @brief Swap two CVEC instances
 *
 * @param _a First vector
 * @param _b Second vector
 */
void cvec_swap(CVEC *_a, CVEC *_b);

/**
 * @brief Clear vector contents but keep capacity unchanged
 *
 * @param _vec Pointer to vector
 */
void cvec_clear(CVEC *_vec);

// PRIVATE //

/**
 * @brief Insert at end of vector
 *
 * @param _vec Vector
 * @param _elem Pointer to element data to store
 * @return CVEC_SUCCESS or an error code
 */
int __cvec_push(CVEC *_vec, void *_elem);

/**
 * @brief Insert new element at beginning
 *
 * @return CVEC_SUCCESS or CVEC_NOT_ENOUGH_CAP
 */
int __cvec_pushFront(CVEC *_vec, void *_elem);

/**
 * @brief Insert element at specific index, shifting elements as needed
 */
int __cvec_pushIndex(CVEC *_vec, const size_t _index, void *_elem);

/**
 * @brief Replace value at given index
 */
int __cvec_set(CVEC *_vec, const size_t _index, void *_set);

/**
 * @brief Delete matching element starting search at index
 */
int __cvec_delIndex(CVEC *_vec, void *_elem, const size_t start);

/**
 * @brief Delete first occurrence of matching value
 */
int __cvec_del(CVEC *_vec, void *_elem);

/**
 * @brief Search for an element in the vector
 *
 * @return index or CVEC_NPOS
 */
int __cvec_find(const CVEC *_vec, void *_find);

/**
 * @brief Cleanup after split operations
 */
int __cvec_destroySplit(CVEC *_vec);

/**
 * @brief Check whether capacity is sufficient for requested additions
 *
 * @return true if capacity allows, else false
 */
bool __cvec_hasEnoughCap(const CVEC *_vec, const size_t _additions);

/**/

#define __CVEC_MACRO_MOD_CALL(func, vec, type, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = CVEC_NOT_ENOUGH_CAP; \
	if (__cvec_hasEnoughCap(vec, 1)) \
		_CVEC_TMP_RETVAL_ = func(vec, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

// Capacity modifiers
#define cvec_push(vec, type, val) \
	__CVEC_MACRO_MOD_CALL(__cvec_push, vec, type, val)

#define cvec_pushFront(vec, type, val) \
	__CVEC_MACRO_MOD_CALL(__cvec_pushFront, vec, type, val)

#define cvec_pushIndex(vec, type, pos, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_; \
	if (__cvec_hasEnoughCap(vec, 1)) \
		_CVEC_TMP_RETVAL_  = __cvec_pushIndex(vec, pos, &_CVEC_TMP_X_VAR_); \
	else \
		_CVEC_TMP_RETVAL_ = CVEC_NOT_ENOUGH_CAP; \
	_CVEC_TMP_RETVAL_; \
})

////

#define cvec_set(vec, type, val, index) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_set(vec, index, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

#define cvec_delIndex(vec, type, val, index) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_delIndex(vec, &_CVEC_TMP_X_VAR_, index); \
	_CVEC_TMP_RETVAL_; \
})

#define cvec_del(vec, type, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_del(vec, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

#define cvec_find(vec, type, val) \
({ \
	type _CVEC_TMP_X_VAR_ = val; \
	int _CVEC_TMP_RETVAL_ = __cvec_find(vec, &_CVEC_TMP_X_VAR_); \
	_CVEC_TMP_RETVAL_; \
})

