# grabx

[![Build and Test](https://github.com/ItzEnigma/grabx/actions/workflows/build-test.yml/badge.svg?branch=master)](https://github.com/ItzEnigma/grabx/actions/workflows/build-test.yml)

GrabX Download Manager

---

## 🚀 Quick Start

### Prerequisites

- C++17 compatible compiler (GCC 7+ or Clang 5+)
- CMake 3.13 or higher
- GTest (automatically downloaded)
- SQLite3 (Download and add to environment PATH)

- **Optional:**
  - `TestMate` VScode extension for running tests
  - `doxygen` and `graphviz` for generating documentation
  - **Linux only:** `gcovr` for generating coverage reports

---

### Environment Setup

You can set up the development environment on Windows and Linux or even WSL by following the instructions below.

> ❗ Coverage reports generation is only supported on **Linux**.

#### Windows

1. **Install Visual Studio 2022 or later** with cmake support (you can also use **Ninja**)

2. **Clone the repository**

3. **Build the project**

   ```powershell
   mkdir build
   cmake -B build -G "Visual Studio 17 2022"
   # For Ninja, use:
   # cmake -B build -G "Ninja"
   cmake --build build --config Release # or Debug
   ```

> Download `doxygen` and `graphviz` from their official websites and add them to your system PATH to generate documentation using `cmake -DGENERATE_DOCS=ON ..` during the build step.

#### Linux (Ubuntu/Debian)

This also can be followed for wsl _(Windows Subsystem for Linux)_.

1. **Install required packages**

   ```bash
   sudo apt update
   sudo apt install -y build-essential cmake git libboost-json-dev clang-tidy ccache doxygen graphviz gcovr ninja-build sqlite3
   ```

2. **Clone the repository**

3. **Build the project**

   ```bash
   mkdir build
   cmake -B build -G "Ninja" .. # -DENABLE_COVERAGE=ON for coverage reports
   cmake --build build --config Release # or Debug
   ```

4. **Generate Coverage Reports** _(if enabled during build)_

   ```bash
   cd build
   cmake --build . --target coverage
   ```

---

## Contributing

Please read [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines on how to contribute to this project.
