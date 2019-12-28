cmake_minimum_required(VERSION 3.0.0)

project(ap VERSION 0.1)

include_directories(./include)
if(MSVC)
    add_definitions(/std:c++17 /permissive)
else()
	add_definitions(-std=c++17)
	SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
	SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
    
endif()

add_executable( ${PROJECT_NAME} main.cpp )