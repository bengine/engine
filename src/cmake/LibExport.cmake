set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})

macro(lib_export _TARGET)
	target_include_directories(${_TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})	
	string(TOUPPER ${_TARGET} _LIB)
	string(TOLOWER ${_TARGET} _LLIB)
	configure_file(${CURRENT_DIR}/LibExport.h.in ${_LLIB}_export.h)
endmacro()
