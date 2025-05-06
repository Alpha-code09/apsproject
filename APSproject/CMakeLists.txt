cmake_minimum_required(VERSION 3.10)
project(SearchEngine VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create main executable
add_executable(search_engine ${SOURCES})

# Testing configuration
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add test executable
file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests gtest_main)

# Register tests
include(GoogleTest)
gtest_discover_tests(unit_tests)

# Installation rules
install(TARGETS search_engine
        RUNTIME DESTINATION bin) 