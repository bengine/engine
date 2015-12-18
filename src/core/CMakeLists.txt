add_library(core include/core.h include/yaml.h yaml_private.h yaml.c yaml_parser.c yaml_utils.c)

include(LibExport RESULT_VARIABLE INCLUDE_RESULT)
if(INCLUDE_RESULT)
	lib_export(core)
else()
	message(FATAL_ERROR "\"LibExport.cmake\" module is not found.")
endif()

target_include_directories(core
	PUBLIC include)

add_executable(yaml_loader yaml/tests/yaml_loader.c)
target_link_libraries(yaml_loader
	PRIVATE core)
