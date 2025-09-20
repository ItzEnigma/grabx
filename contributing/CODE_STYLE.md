# C++ Code Style Guidelines

This document outlines the code styling standards for this project, based on the official C++ Standard guidelines. All contributors must adhere to these rules to ensure code consistency, readability, and maintainability.

## Important Guidelines

### 1. Styling
- Everything is **camel case** except constants
- constants are uppercase and don't have a prefix
- enums are pascal case and don't have a prefix. And recommended to explicitly set the values of enumerators
- global variables prefixed with `g_`
- static variables prefixed with `s_`
- private and protected class attributes prefixed with `m_`
- public class attributes are not prefixed
- class attributes and methods are lower camelcase
- class names are pascal case
- namespaces are snake_case. Always use and place relevant code inside a namespace
- ❗ **DO NOT use** `using namespace ...` in header files, and avoid it in source files as well (you can use `using util = project::util;` to create an alias)

### 2. Formatting

#### 2.1. Files
- headers use the `.h` extension
- implementation files use the `.cpp` extension
- included files use the `.inc` extension
- class files bear the name of the class they implement
- no spaces in file names
- file names must be treated as case insensitive, i.e. it is not allowed to have several files with the same name but a different case
- `#include` must use fully qualified names `#include "path/to/header.h"`
- Sort the includes alphabetically with,
    1. Project includes
    2. Third-party includes
    3. Standard library includes
- all public include files must reside under the **inc/** folder
- all source files must reside under the **src/** folder
- tests reside under the **test/** folder

#### 2.2. Braces
- Use **K&R style** for braces:
    ```cpp
    if (condition) {
        // code
    }
    ```
- Always use braces for control statements, even for single-line blocks.
- Always add braces for each logical expression `if ((a && b) || (c && d))`

#### 2.3. Comments
- Use `//` for single-line comments and `/* ... */` for multi-line.
- Document all public classes, functions, and complex logic.
    - Use Doxygen-style comments for API documentation.
    - Make the methods comments on the header file, not the cpp file.
Example:
    ```cpp
    /**
     * @brief Brief description of the function.
     * 
     * Detailed description of the function.
     * 
     * @param param1 Description of parameter 1.
     * @return Description of return value.
     */
    int myFunction(int param1);
    ```

#### 2.4. Indentation
- Use **4 spaces** per indentation level.

### 3. Code Safety
- Prefer `nullptr` over `NULL` or `0` for pointers.
- Avoid using raw pointers; use smart pointers (`std::unique_ptr`, `std::shared_ptr`) where possible.
- Always check for null pointers before dereferencing (if raw pointers are used).

### 4. Standard Library
- Prefer standard library containers and algorithms over custom implementations.

### 5. Consistency
- Follow the project's existing style and conventions.
- Do not mix different coding styles.

### 6. Error Handling
- Use exceptions for error handling.
- Do not excessively use exceptions for control flow.
- Ensure proper resource management (RAII).

### 7. Line Length
- Limit lines to a maximum of **80 characters** for better readability.

### 8. Avoid Global Variables
- Use class members or function parameters instead of global variables.

### 9. Avoid Magic Numbers
- Use named constants or enumerations instead of hard-coded numbers.
- Minimize the use of `#define` for constants; prefer `const`, `constexpr`, or `enum class`.

### 10. Use `auto` Judiciously
- Use `auto` when the type is obvious from the right-hand side of the assignment. `auto myVar = someFunction();`
- Avoid `auto` when it reduces code clarity.

### 11. Use `override` and `final`
- Always use the `override` keyword when overriding virtual functions to ensure correctness.

## References

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

---

**All code submissions must comply with these guidelines. Non-conforming code may be rejected or require revision.**