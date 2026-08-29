cmake_minimum_required(VERSION 3.15)
project(OopAgentCS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
message(STATUS "Found CURL")

include_directories(src)

file(GLOB SOURCES 
    "src/*.cpp"
    "src/*/*.cpp"
    "src/*/*/*.cpp"
)

# Main executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

if(CURL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /std:c++latest)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Benchmark runner
add_executable(run_eval benchmark/run_eval.cpp ${SOURCES})
if(CURL_FOUND)
    target_link_libraries(run_eval PRIVATE CURL::libcurl)
endif()
if(MSVC)
    target_compile_options(run_eval PRIVATE /W4 /std:c++latest)
else()
    target_compile_options(run_eval PRIVATE -Wall -Wextra -Wpedantic)
endif()