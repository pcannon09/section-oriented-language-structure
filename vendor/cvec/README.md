# CVEC

## 1.0.0

### _"Functional C++ vector-like manipulation for C"_

`CVEC` is a minimal and efficient C dynamic array library that offers type-agnostic vector behavior with optional capacity control and safe dynamic resizing. It provides an interface similar to `std::vector` in C++ but implemented in pure C with focus on simplicity, safety, and flexibility.

---

### Supported Compilers

**GCC** >= 5.0
**Clang** >= 3.5

All compilers **must support C99** or higher.

---

## Features

* **Dynamic Capacity Management**
  Initialize, resize, and grow vectors automatically, or lock capacity with `forceCap` for predictable memory limits.

* **Safe Element Operations**
  Supports indexed push, insert, and removal without exposing raw pointer arithmetic.

* **Front, Back, and Indexed Insertion**
  Add elements to the end, front, or any specific position via `__cvec_push()`, `__cvec_pushFront()`, or `__cvec_pushIndex()`.

* **Element Access and Queries**
  Retrieve or validate indices safely with `cvec_get()`, `cvec_at()`, and `cvec_atCap()`.

* **Merging and Copying**
  Combine vectors or duplicate existing ones using `cvec_merge()` and `cvec_initCopy()`.

* **Deletion Utilities**
  Delete by index, by element, or remove multiple matching elements through `__cvec_delIndex()`, `__cvec_del()`, and `cvec_remove()`.

* **Reversal and Shrinking**
  Reverse vector order or reduce capacity using `cvec_reverse()` and `cvec_shrink()`.

* **Optional Dynamic Capacity**
  When `dynamicCap` is disabled, all operations must fit within the current capacity.
  This is useful for embedded or low-level systems.

---

## Basic Usage

Example: creating, pushing, and accessing integer elements.

```c
#include "cvec.h"
#include <stdio.h>

int main(void)
{
	CVEC v = cvec_init(4, sizeof(int)); // Create a vector for ints with capacity of 4

	for (int i = 0; i < 10; i++)
		__cvec_push(&v, &i); // Push elements, auto-resizing as needed

	for (int i = 0; i < v.size; i++)
		printf("%d ", *(int*)cvec_get(&v, i)); // Print each element

	cvec_destroy(&v); // Free allocated memory
}
```

---

## Error Handling

Most functions return:

* `CVEC_SUCCESS` - operation succeeded
* `CVEC_FAIL` - invalid input or operation
* `CVEC_FAIL_ALLOCATION` — allocation or reallocation failed
* `CVEC_FORCECAP_FAIL` — capacity exceeded while `forceCap` is enabled
* `CVEC_OUT_BOUNDS` — invalid index access
* `CVEC_ELEM_NOT_FOUND` — target element not found
* `CVEC_NPOS` — element not found (for `__cvec_find()`)

---

## Internal Behavior

Internal (private) functions use the `__cvec_` prefix, for example:

* `__cvec_push()` — push element to the end
* `__cvec_pushFront()` — push element to the beginning
* `__cvec_pushIndex()` — insert at a specific index
* `__cvec_delIndex()` — delete element at or after a specific index

These functions are used internally by the main API but may also be called directly if low-level control is desired.

---

## Notes on Capacity

* If `forceCap` is set to `true`, exceeding the capacity will **not** reallocate memory.
  The function instead returns `CVEC_FORCECAP_FAIL`.

* If `dynamicCap` is `false`, the capacity remains fixed even if `forceCap` is disabled.

* Default allocation growth is defined by `__CVEC_CAP_ADDITION`.

---

## Example: Merging Two Vectors

```c
#include "cvec.h"
#include <stdio.h>

int main(void)
{
	CVEC a = cvec_init(2, sizeof(int));
	CVEC b = cvec_init(2, sizeof(int));

	for (int i = 0; i < 3; i++) __cvec_push(&a, &i);
	for (int i = 3; i < 6; i++) __cvec_push(&b, &i);

	cvec_merge(&a, &b); // Merge `b` into `a`

	for (int i = 0; i < a.size; i++)
		printf("%d ", *(int*)cvec_get(&a, i));

	cvec_destroy(&a);
	cvec_destroy(&b);
}
```

---

## Internal Helpers

Helper functions such as `__cvec_hasEnoughCap()` and macros like `__CVEC_SET_NULL()` are used internally for safety and consistency.
These are not part of the **public API** and should not be called directly in user code.

---

## Summary

`CVEC` simplifies handling generic dynamic arrays in C while remaining lightweight and dependency free.
It’s ideal for embedded systems, lightweight applications, or any project needing a simple vector-like structure without C++ overhead.

