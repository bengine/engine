add_library(base64 include/base64.h base64_private.h base64.c)

if(BUILD_SHARED_LIBS)
	target_compile_definitions(base64
		PUBLIC BASE64_SHARED
		PRIVATE BASE64_EXPORT)
endif()

target_include_directories(base64
	PUBLIC include
	PRIVATE .)
