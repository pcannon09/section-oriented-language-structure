#include "../inc/cvec/cvec.h"

CVEC cvec_init(int _cap, size_t _elemSize)
{
	CVEC v;

	v.forceCap = false;
 	v.dynamicCap = true;
	v.__usedSplit = false;

	v.size = 0;
	v.elemLen = _elemSize;
	v.cap = (_cap < 0 ? v.size + __CVEC_CAP_ADDITION : _cap);
	v.data = malloc(v.elemLen * v.cap);

	// Failed to malloc
	if (!v.data)
	{
		v.initialized = false;

		return v;
	}

	v.initialized = true;

	return v;
}

int cvec_destroy(CVEC *_vec)
{
	if (!_vec || !_vec->data) return CVEC_FAIL;

	if (_vec->__usedSplit)
	{
		int result;

		if ((result = __cvec_destroySplit(_vec)) != CVEC_SUCCESS)
			return result;
	}

	free(_vec->data); _vec->data = NULL;

	__CVEC_SET_NULL(_vec, ->);

	return CVEC_SUCCESS;
}

int __cvec_destroySplit(CVEC *_vec)
{
	if (!_vec || !_vec->initialized || !_vec->data) return CVEC_FAIL;

	for (size_t i = 0; i < _vec->size; i++)
    {
    	free(*(char**)cvec_get(_vec, i));
		*(char**)cvec_get(_vec, i) = NULL;
	}

	return CVEC_SUCCESS;
}

int __cvec_push(CVEC *_vec, void *_elem)
{
	if (!_vec || !_elem) return CVEC_FAIL;

	if (_vec->size >= _vec->cap)
	{
		if (_vec->forceCap)
			return CVEC_FORCECAP_FAIL;

		if (!_vec->forceCap)
			_vec->cap += __CVEC_CAP_ADDITION;

		void *newData = realloc(_vec->data, _vec->cap * _vec->elemLen);

		if (!newData)
			return CVEC_FAIL_ALLOCATION;

		_vec->data = newData;
	}

	memcpy((char*)_vec->data + _vec->size * _vec->elemLen, _elem, _vec->elemLen);

	_vec->size++;

	return CVEC_SUCCESS;
}

int __cvec_pushFront(CVEC *_vec, void *_elem)
{
	if (!_vec || !_elem) return CVEC_FAIL;

	// Expand capacity if needed
	if (_vec->size >= _vec->cap)
	{
		if (_vec->forceCap)
			return CVEC_FORCECAP_FAIL;

		if (!_vec->forceCap && _vec->dynamicCap) _vec->cap += __CVEC_CAP_ADDITION;

		void *newData = realloc(_vec->data, _vec->cap * _vec->elemLen);

		if (!newData)
			return CVEC_FAIL_ALLOCATION;

		_vec->data = newData;
	}

	// Shift elements one slot to the right
	if (_vec->size > 0)
	{
		memmove(
			(char*)_vec->data + _vec->elemLen,    // destination start (index 1)
			_vec->data,                           // source start (index 0)
			_vec->size * _vec->elemLen            // number of bytes to move
		);
	}

	// Copy new element to index 0
	memcpy(_vec->data, _elem, _vec->elemLen);

	_vec->size++;

	return CVEC_SUCCESS;
}

int __cvec_pushIndex(CVEC *_vec, const size_t _index, void *_elem)
{
	if (!_vec || !_elem) return CVEC_FAIL;

	// Bounds: inserting at size is allowed (append),
	// anything larger is out of range.
	if (_index > _vec->size)
		return CVEC_FAIL; // or define a specific error code

	// Expand capacity if needed
	if (_vec->size >= _vec->cap)
	{
		if (_vec->forceCap)
			return CVEC_FORCECAP_FAIL;

		if (!_vec->forceCap && _vec->dynamicCap) _vec->cap += __CVEC_CAP_ADDITION;

		void *newData = realloc(_vec->data, _vec->cap * _vec->elemLen);

		if (!newData)
			return CVEC_FAIL_ALLOCATION;

		_vec->data = newData;
	}

	// If not inserting at the end, shift elements to the right
	if (_index < _vec->size)
	{
 		memmove(
    			(char*)_vec->data + (_index + __CVEC_CAP_ADDITION) * _vec->elemLen,
    			(char*)_vec->data + _index * _vec->elemLen,
    			(_vec->size - _index) * _vec->elemLen
			   );
	}

	// Copy new element into the index
	memcpy(
		(char*)_vec->data + _index * _vec->elemLen,
		_elem,
		_vec->elemLen
	);

	_vec->size++;
	return CVEC_SUCCESS;
}

bool cvec_at(const CVEC *_vec, const size_t _index)
{ return _vec && _index < _vec->size; }

bool cvec_atCap(const CVEC *_vec, const size_t _index)
{ return _vec && _index < _vec->cap; }

void *cvec_get(const CVEC *_vec, const size_t _index)
{
	// Error: Out of bounds
	if (!_vec || !_vec->data ||
			_index >= _vec->size)
		return NULL;

	return (char*)_vec->data + _index * _vec->elemLen;
}

int cvec_merge(CVEC *_toMerge, const CVEC *_input)
{
	if (!_toMerge || !_input || !_input->data)
		return CVEC_FAIL;

	for (unsigned int i = 0; i < _input->size; i++)
	{
		void *elem = cvec_get(_input, i);

		if (!elem)
			continue;

		__cvec_push(_toMerge, elem);
	}

	return CVEC_SUCCESS;
}

int __cvec_set(CVEC *_vec, const size_t _index, void *_set)
{
	if (!_vec || !_vec->data) return CVEC_FAIL;

	if (_index >= _vec->cap) return CVEC_OUT_BOUNDS;

	CVEC old = *_vec;
	CVEC replacement = cvec_init(old.cap, old.elemLen);

	bool isSet = false;

	for (unsigned int i = 0 ; i < old.size ; i++)
	{
		void *foundElem = cvec_get(&old, i);

		if (!foundElem) continue;

		if (!isSet && i == _index)
		{
			__cvec_push(&replacement, _set);

			isSet = true;

			continue;
		}

		// Leave it as is
		__cvec_push(&replacement, foundElem);
	}

	// Fill the rest

	*_vec = replacement;

	cvec_destroy(&old);

	return CVEC_SUCCESS;
}

CVEC cvec_initCopy(const CVEC *_src)
{
	CVEC dest;

	// Set error, return an empty copy without initializement
	if (!_src || !_src->data || !_src->initialized)
	{
		__CVEC_SET_NULL(dest, .);

		return dest;
	}

	dest = cvec_init(_src->cap, _src->elemLen);

	if (!dest.initialized)
		return dest; // Return the `dest` as is

	memcpy(dest.data, _src->data, _src->size * _src->elemLen);

	dest.size = _src->size;
	dest.forceCap = _src->forceCap;

	return dest;
}

// del() functions
int __cvec_delIndex(CVEC *_vec, void *_elem, const size_t start)
{
    if (!_vec || !_elem) return CVEC_FAIL;

    CVEC old = *_vec;  // Shallow Copy
    CVEC retVec = cvec_init(old.cap, old.elemLen);

    bool removed = false;

    for (unsigned int i = 0; i < old.size; i++)
    {
        void *val = cvec_get(&old, i);

        // Once reached the start index and have not removed anything yet, do the following if condition
        if (!removed && i >= start)
        {
            if (memcmp(val, _elem, old.elemLen) == 0)
            { removed = true; continue; }
        }

        // Copy all others as is
        __cvec_push(&retVec, val);
    }

    if (!removed)
    {
        cvec_destroy(&retVec);

        return CVEC_ELEM_NOT_FOUND;
    }

    *_vec = retVec;

    // Free `old` vector data
    cvec_destroy(&old);

    return CVEC_SUCCESS;
}

int __cvec_del(CVEC *_vec, void *_elem)
{
	if (!_vec || !_elem) return CVEC_FAIL;

	// Start from index 0
	// Delete element
	// Repeat until reached the size limit
	for (unsigned int i = 0 ; i < _vec->size ; i++)
	{ __cvec_delIndex(_vec, _elem, 0); }

	return CVEC_SUCCESS;
}

int __cvec_find(const CVEC *_vec, void *_find)
{
	if (!_vec || !_find)
		return CVEC_FAIL;

	for (unsigned int i = 0 ; i < _vec->size ; i++)
	{
        void *val = cvec_get(_vec, i);

        if (memcmp(val, _find, _vec->elemLen) == 0)
        	return i;
	}

	return CVEC_NPOS;
}

int cvec_remove(CVEC *_vec, const size_t _index)
{
	if (!_vec) return CVEC_FAIL;

	CVEC old = *_vec;
	CVEC retVec = cvec_init(old.cap, old.elemLen);

	for (unsigned int i = 0 ; i < old.size ; i++)
	{
		void *foundElem = cvec_get(&old, i);

		// Element to delete will be ignored
		if (_index == i)
			continue;

		__cvec_push(&retVec, foundElem);
	}

	*_vec = retVec;

	cvec_destroy(&old);

	return CVEC_SUCCESS;
}

int cvec_popBack(CVEC *_vec)
{
	if (!_vec)
		return CVEC_FAIL;

	cvec_remove(_vec, _vec->size - 1);

	return CVEC_SUCCESS;
}

int cvec_popFront(CVEC *_vec)
{
	if (!_vec)
		return CVEC_FAIL;

	cvec_remove(_vec, 0);

	return CVEC_SUCCESS;
}

int cvec_reverse(CVEC *_vec)
{
    if (!_vec || !_vec->data)
        return CVEC_FAIL;

    CVEC old = *_vec;  // Shallow Copy
    CVEC tmpVec = cvec_init(old.cap, old.elemLen);

    // Reverse the elements by iterating backward through `_vec`
    for (int i = old.size - 1; i >= 0; i--)
    {
        void *result = cvec_get(&old, i);

        if (!result)
        {
        	cvec_destroy(&tmpVec);

            return CVEC_GET_NULL;
        }

        __cvec_push(&tmpVec, result);
    }

    *_vec = tmpVec;

    cvec_destroy(&old);

    return CVEC_SUCCESS;
}

int cvec_shrink(CVEC *_vec)
{
    if (!_vec || !_vec->data) return CVEC_FAIL;
    if (_vec->cap == _vec->size) return CVEC_SUCCESS;

    void *newData = realloc(_vec->data, _vec->size * _vec->elemLen);

    if (!newData) return CVEC_FAIL_ALLOCATION;

    _vec->data = newData;
    _vec->cap = _vec->size;

    return CVEC_SUCCESS;
}

void cvec_swap(CVEC *_a, CVEC *_b)
{
	CVEC aTmp = *_a;

	*_a = *_b;
	*_b = aTmp;
}

bool __cvec_hasEnoughCap(const CVEC *_vec, const size_t _additions)
{
    if (!_vec) return false;

    if (_vec->dynamicCap)
    	return true;

    return _vec->size  <= _vec->cap - _additions;
}

void *cvec_begin(const CVEC *_vec)
{ return _vec ? _vec->data : NULL; }

void *cvec_end(const CVEC *_vec)
{ return _vec ? (char*)_vec->data + _vec->size * _vec->elemLen : NULL; }

int cvec_emptyAll(CVEC *_vec)
{
	_vec->size = 0;

	return CVEC_SUCCESS;
}

void cvec_clear(CVEC *_vec)
{
	cvec_emptyAll(_vec);
}

int cvec_split(CVEC *_vec, char *_str, const char *_del)
{
	if (!_vec || !_vec->initialized)
		return CVEC_FAIL;

    if (_vec->__usedSplit)
    	__cvec_destroySplit(_vec);

    cvec_clear(_vec);

    _vec->__usedSplit = true;

    char chs[strlen(_str) + 1];

    memcpy(chs, _str, strlen(_str) + 1);

    char *tok = strtok(chs, _del);

	while (tok != NULL)
	{
		char *copy = malloc(strlen(tok) + 1);

    	strcpy(copy, tok);

        __cvec_push(_vec, &copy);

        tok = strtok(NULL, _del);
	}

    return CVEC_SUCCESS;
}

