add_library(usaa SHARED usaa.c)

include(LibExport RESULT_VARIABLE INCLUDE_RESULT)
if(INCLUDE_RESULT)
	lib_export(usaa)
	target_compile_options(usaa
		PRIVATE -DUSAA_DECL_EXPORT)
else()
	message(FATAL_ERROR "\"LibExport.cmake\" module is not found.")
endif()

target_include_directories(usaa
	PUBLIC include)
