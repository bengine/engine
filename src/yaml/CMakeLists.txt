add_library(yaml include/yaml.h yaml_private.h yaml.c yaml_scanner.c)

if(BUILD_SHARED_LIBS)
	target_compile_definitions(yaml
		PUBLIC YAML_SHARED
		PRIVATE YAML_EXPORT)
endif()

target_include_directories(yaml
	PUBLIC include
	PRIVATE .)
