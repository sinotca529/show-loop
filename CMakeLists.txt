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

# Handle RTTI stuff, which often leads to error
if (NOT ${LLVM_ENABLE_RTTI})
    if (   "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"
    )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
        message(STATUS "Disable RTTI")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
        message(STATUS "Disable RTTI")
    endif()
    # Do not give any flags for other less widely used
    # compilers
endif()

set(
    DUMP_IR_PASS_TEXTUAL_PIPELINE OFF CACHE BOOL
    "Use `instr-ib-check-fn` with the textual pass pipelne description"
)

if (DUMP_IR_PASS_TEXTUAL_PIPELINE)
  add_compile_definitions(ADD_CHECK_FUNC_DECLARE_PASS_USE_PIPELINE_PARSER)
endif()

set(_SOURCE_FILES ShowLoop.cpp)

add_llvm_pass_plugin(ShowLoop ${_SOURCE_FILES})
