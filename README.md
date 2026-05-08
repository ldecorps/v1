# Hello C Project

Minimal C project using CMake with a small library, a runnable `hello_world` executable, and a unit test.

Prerequisites

- CMake 3.10 or newer
- A C compiler (AppleClang or Homebrew GCC on macOS). This project will prefer Homebrew GCC if installed (gcc-13, gcc-12, ...).

Build (out-of-source)

```bash
cmake -S . -B build
cmake --build build
```

Build specific targets

```bash
cmake --build build --target hello_world
cmake --build build --target hello_test
```

Run the programs

```bash
./build/bin/hello_world
./build/bin/hello_test
```

Run tests (CTest)

```bash
cmake -S . -B build
cmake --build build --target hello_test
ctest --test-dir build --verbose
```

VS Code / Debugging (F5)

- This repository includes `.vscode/tasks.json` and `.vscode/launch.json` to wire F5 to build and debug `build/bin/hello_world`.
- Press F5 (or Run → Start Debugging) to build the `hello_world` target and launch it under the debugger.

Quick single-file compile (useful for quick edits)

```bash
gcc -I include tests/test_hello.c src/hello.c -g -o tests/test_hello
./tests/test_hello
```

Forcing a compiler when configuring

```bash
cmake -S . -B build -DCMAKE_C_COMPILER=/usr/local/bin/gcc-13
```

Notes

- CMake is configured to export `build/compile_commands.json` and place binaries in `build/bin` and libraries in `build/lib`.
- If you prefer Clang or a specific compiler, pass `-DCMAKE_C_COMPILER` when configuring.

Enjoy!
