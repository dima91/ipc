cmake_minimum_required (VERSION 3.8.0)
project (ipc)
cmake_policy (SET CMP0015 NEW)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -pedantic -O0 -g3 -std=c++11 -fPIC")
set (LIB_NAME ipc)
set (EXECUTABLE_OUTPUT_PATH bin)
set (HEADERS_DIRS include)
set (LIBS_DIRS )


include_directories (${HEADERS_DIRS})
link_directories	(${LIBS_DIRS})


### Library source files
file (GLOB SRC_FILES src/*.cpp)

add_library (${LIB_NAME} SHARED ${SRC_FILES})