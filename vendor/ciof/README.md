# CIOF - Custom Input Output Format

`CIOF` is a lightweight C++17+ input/output framework designed to simplify formatted printing, logging, and user interaction.
It provides a set of high-level utilities to handle standard output, error logging, ANSI styling, and user input consistently across platforms**

---

## Features

- **Unified Output Handling**
  Output to `std::cout`, `std::cerr`, or `std::clog` with a simple API.

- **Configurable Output**
  Control spacing, delimiters, and formatting globally via `ciof::outputConf`.

- **Print & Echo Helpers**
  - `print()` for output with newline
  - `echo()` for output without newline
  - Variants for standard log and error streams

- **Input Utilities**
  Read values interactively with `input()` functions, optionally with a prompt.

- **ANSI Styling Support**
  Easily apply bold, italic, underline, strikethrough, or custom RGB colors.

- **Cross-Platform (with limitations)**
  Works on Linux, macOS, and Windows 10+.
  Versions lower than Windows 10 have no support for ANSI

---

## Configuration

Output behavior can be controlled globally using `ciof::CIOFOutputConfig`.
Options include automatic spacing, custom delimiters, ignoring formatting keywords, and enabling/disabling processing.

---

## Supported Compilers

- GCC >= 7.0
- Clang >= 6.0
- MSVC >= 19.14 (Visual Studio 2017 15.7)

All compilers must support C++17 or higher.

---

## Platform Support

- Linux - Fully supported
- macOS - Fully supported
- Windows 10 or later only - Versions lower than Windows 10 have no support for ANSI, but still functional

## MINIMAL CMAKE EXAMPLE

```cmake
cmake_minimum_required(VERSION 3.24)
project(ciofTestingProject VERSION 1.0 LANGUAGES CXX)

# Set C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find the system-installed CIOF package
find_package(ciof REQUIRED)

# Add the test executable
add_executable(ciofTesting ciofTesting.cpp)

# Include CIOF headers and link its library
target_link_libraries(ciofTesting PRIVATE Ciof::ciof_static) # or `ciof_shared`
```

## MINIMAL CODE EXAMPLE
```cpp
#include <ciof/ciof.hpp>

int main() { ciof::print("CIOF Works."); }
```


