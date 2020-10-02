cmake_minimum_required(VERSION 3.4)

set(source)
list(APPEND source
   client_p.cpp
   client.cpp
   rollingchecksum.cpp)

if(BUILD_SHARED_LIB)
	add_library(zsync3 SHARED ${source})
endif()

if(NOT BUILD_SHARED_LIB)
	add_library(zsync3 ${source})
endif()

target_link_libraries(zsync3 crypto pthread)
