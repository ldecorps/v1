# Hello C Project

Minimal C project using CMake and a simple unit test run via CTest.

Build and run tests:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

Run the program directly:

```bash
./hello_world
```
