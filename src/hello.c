#include "hello.h"

const char* say_hello(void) {
    return "Hello, World!";
}


/*

Pour voir et utiliser le menu "Run Test" localement sur son Mac, Jules doit avoir au minimum :

Xcode Command Line Tools (compilateur/Make) — installer avec xcode-select --install.
CMake (et éventuellement ninja) pour configurer/build le projet — brew install cmake ninja.
Extensions VS Code : CMake Tools (ms-vscode.cmake-tools) + C/C++ (ms-vscode.cpptools) et un adaptateur de tests compatible avec votre framework + Test Explorer UI (par ex. Catch2/GoogleTest adapter ou un adaptateur CTest).
Le projet configuré/compilé localement : lancer CMake: Configure puis CMake: Build (les tests sont découverts si CMakeLists.txt contient enable_testing() / add_test()).
Remarque Live Share : si Jules est invité via Live Share, les actions de test s’exécutent normalement sur la machine hôte — pour exécuter localement il faut cloner et configurer/builder sur son Mac.
Commandes utiles :


xcode-select --install
brew install cmake ninja
cmake -S . -B build
cmake --build build
ctest --test-dir build -V


*/