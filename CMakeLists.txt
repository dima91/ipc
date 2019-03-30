cmake_minimum_required (VERSION 3.8.0)
project (ipc)
cmake_policy (SET CMP0015 NEW)



## Macro to add all subdirectory of "curdir" directory
MACRO (SUBDIRLIST result curdir)
	file (GLOB children ${curdir}/*)
	set (dirlist "")
	foreach (child ${children})
		if (IS_DIRECTORY ${child})
			if (EXISTS ${child}/CMakeLists.txt)
				list (APPEND dirlist ${child})
			endif ()
		endif ()
	endforeach ()
	set (${result} ${dirlist})
ENDMACRO ()



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


### EXAMPLES
subdirlist (EXAMPLES ./examples)
foreach (EXAMPLE ${EXAMPLES})
	message ("-----  Including this example: " ${EXAMPLE})
	add_subdirectory (${EXAMPLE})
endforeach ()


### TESTS
include_directories (tests)
add_executable (Tester tests/main.cpp)
target_link_libraries (Tester ipc pthread)