cmake_minimum_required(VERSION 3.10)
project(mini_dbms)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/storage/*.cpp"
    "tests/*.cpp"
)

add_executable(mini_dbms ${SOURCES})
