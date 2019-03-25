cmake_minimum_required(VERSION 3.0)


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


subdirlist (examples .)
foreach (example ${examples})
	message ("Including this example: " ${example})
	add_subdirectory (${example})
endforeach ()