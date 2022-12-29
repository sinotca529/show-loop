cmake_minimum_required(VERSION 3.13)
project(show-loop)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
## LLVM
find_package(LLVM
    REQUIRED
    CONFIG
    HINTS "$ENV{LLVM13}/lib/cmake"
    NO_DEFAULT_PATH
)

message(STATUS "Using LLVM ${LLVM_PACKAGE_VERSION}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_llvm_pass_plugin(ShowLoop ShowLoop.cpp)
