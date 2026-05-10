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

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Export compilation database for editor integrations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Place runtime and archive outputs into predictable folders under the build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(hello STATIC src/hello.c)
target_include_directories(hello PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Examen 2026 LU1IN002 — module especes (tableaux + listes chainees)
# REF: Examen_2026_UL1IN002_Prediction.pdf
add_library(especes STATIC src/especes.c)
target_include_directories(especes PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(hello_world src/main.c)
target_link_libraries(hello_world PRIVATE hello)
target_include_directories(hello_world PRIVATE ${PROJECT_SOURCE_DIR}/include)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
  enable_testing()

  # Cours UL1IN002 — rappels de cours avec tau (header-only, cross-platform)
  # tau est vendored dans third_party/tau/tau.h — aucune installation requise.
  set(COURS_TEST_EXE cours_test)
  add_executable(${COURS_TEST_EXE} tests/test_cours_tau.c)
  target_include_directories(${COURS_TEST_EXE} PRIVATE
    ${PROJECT_SOURCE_DIR}/third_party
  )
  # tau est header-only : aucune lib a lier.
  # Sur certains compilateurs, les maths flottantes de tau necessitent -lm.
  if(UNIX)
    target_link_libraries(${COURS_TEST_EXE} PRIVATE m)
  endif()
  add_test(NAME ${COURS_TEST_EXE} COMMAND ${COURS_TEST_EXE})

  # Examen 2026 LU1IN002 — tests unitaires des fonctions especes
  # REF: Examen_2026_UL1IN002_Prediction.pdf
  set(ESPECES_TEST_EXE especes_test)
  add_executable(${ESPECES_TEST_EXE} tests/test_especes_tau.c)
  target_include_directories(${ESPECES_TEST_EXE} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party
  )
  target_link_libraries(${ESPECES_TEST_EXE} PRIVATE especes)
  if(UNIX)
    target_link_libraries(${ESPECES_TEST_EXE} PRIVATE m)
  endif()
  add_test(NAME ${ESPECES_TEST_EXE} COMMAND ${ESPECES_TEST_EXE})
endif()

# Installation rules
install(TARGETS hello hello_world
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(FILES ${PROJECT_SOURCE_DIR}/include/hello.h
  DESTINATION include
)
