cmake_minimum_required(VERSION 3.4)
project(zsync)

add_executable(zsync zsync.cpp)
target_link_libraries(zsync PRIVATE zsync3) 
