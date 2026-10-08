cmake_minimum_required(VERSION 3.16)
project(md2html CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(md2html_lib
    src/utils.cpp
    src/inline.cpp
    src/parser.cpp
    src/page.cpp
)
target_include_directories(md2html_lib PUBLIC src)

add_executable(md2html src/main.cpp)
target_link_libraries(md2html PRIVATE md2html_lib)

add_executable(run_tests tests/run_tests.cpp)
target_link_libraries(run_tests PRIVATE md2html_lib)

enable_testing()
add_test(NAME markdown_cases COMMAND run_tests ${CMAKE_SOURCE_DIR}/tests/cases)
