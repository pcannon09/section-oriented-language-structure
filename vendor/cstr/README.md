# CSTR
## 1.1.0

`CSTR` is a lightweight C string utility library that provides a safer and more flexible alternative to raw C-style strings. It wraps dynamic allocation, formatting, substring manipulation, and replacement in a consistent API without pulling in heavy dependencies.

---

## Requirements
### Supported Compilers

**GCC** >= 7.0

**Clang** >= 6.0

All compilers **must support C11** or higher.

## Features

* **Dynamic String Management**
  Create, copy, resize, and destroy strings with automatic memory handling.

* **Safe Set and Append Operations**
  `cstr_set()` and `cstr_add()` handle allocation internally with optional capacity limiting.

* **Substring and In-Place Editing**
  Includes `cstr_substr()`, `cstr_erase()`, `cstr_insert()`, and `cstr_shrink()`.

* **Replacement Utilities**
  Replace single or all occurrences using `cstr_replace()` and `cstr_replaceAll()`.

* **Case and Formatting Helpers**
  Convert to upper/lower, reverse, clear, or generate strings from numeric values.

* **Search and Compare Support**
  `cstr_find()`, `cstr_findFrom()`, `cstr_startsWith()`, `cstr_endsWith()`, and `cstr_comp()`.

* **Optional Capacity Locking**
  Enabling `forceCap` prevents automatic reallocation and returns `CSTR_FORCECAP_LIMIT` on overflow attempts.

---

## Basic Usage
This code will set the `s` variable to `HELLO, WORLD!`

```c
#include "cstr.h"

int main(void)
{
	CSTR s = cstr_init(); // Create an empty string
	cstr_set(&s, "Hello"); // Set the string to `Hello`
	cstr_add(&s, ", World!"); // Add `, World!`
	cstr_upper(&s); // Set the full string variable to upper case
	cstr_destroy(&s); // Destroy the string and free the allocated memory
}
```

---

## Error Handling

Most functions return:

* `CSTR_SUCCESS` on success
* `CSTR_FAIL` on invalid usage or memory errors
* `CSTR_FORCECAP_LIMIT` when `forceCap` is enabled and the buffer can't grow

`cstr_bool()` is included as a helper to stringify boolean values, if failed, it will still return `false`

---

## Internal Helpers

The library includes internal functions such as `__cstr_setFormat()` for variadic formatted writes. These are not part of the public API* and should not be used directly
