cmake_minimum_required(VERSION 3.4)
project(zsync3)

set(V_MAJOR 0)
set(V_MINOR 1)
set(V_PATCH 0)
set(V_SUFFIX "")

set(zsync3_VERSION ${V_MAJOR}.${V_MINOR}.${V_PATCH}${V_SUFFIX})
set(zsync3_SOVERSION ${V_MAJOR}.${V_MINOR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(.)
add_subdirectory(src)
add_subdirectory(tools)
