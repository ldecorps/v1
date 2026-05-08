cmake_minimum_required(VERSION 3.10)

# Prefer Homebrew GCC on macOS when available (gcc-13, gcc-12, ...).
if(APPLE)
  find_program(HOMEBREW_GCC NAMES gcc-13 gcc-12 gcc-11 gcc-10 gcc)
  if(HOMEBREW_GCC)
    set(CMAKE_C_COMPILER ${HOMEBREW_GCC} CACHE STRING "C compiler" FORCE)
  endif()
endif()

project(hello_c_project VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compilation database for editor integrations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Place runtime and archive outputs into predictable folders under the build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(hello STATIC src/hello.c)
target_include_directories(hello PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(hello_world src/main.c)
target_link_libraries(hello_world PRIVATE hello)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
  enable_testing()
  add_executable(hello_test tests/test_hello.c)
  target_link_libraries(hello_test PRIVATE hello)
  add_test(NAME hello_test COMMAND hello_test)
endif()
