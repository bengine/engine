set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})

macro(lib_export _LIB)
	string(TOUPPER ${_LIB} LIB)
	configure_file(${CURRENT_DIR}/LibExport.h.in export.h)
	target_include_directories(${_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
endmacro()
