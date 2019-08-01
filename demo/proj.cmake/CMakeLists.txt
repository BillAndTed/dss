cmake_minimum_required (VERSION 2.6)
project (demo)

set(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

add_subdirectory(../../oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

add_executable(demo 
	../src/example.cpp
	../src/main.cpp
	../src/Demo.h
	../src/Demo.cpp
	../src/example.h
	../src/EditorialData.h
	../src/EditorialData.cpp
	../src/EditorialDisplay.h
	../src/EditorialDisplay.cpp
	../src/EditorialMenu.h
	../src/EditorialMenu.cpp)
target_link_libraries(demo ${OXYGINE_CORE_LIBS})



if (WIN32) #disable console mode for VC++
	set_target_properties(demo PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)


if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --emrun \
		-s NO_EXIT_RUNTIME=1 \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s WARN_ON_UNDEFINED_SYMBOLS=1 \
		-s FORCE_FILESYSTEM=1 \
		-s ERROR_ON_MISSING_LIBRARIES=0 \
		--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../data/@/ \
		-s 'EXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]'")
	em_link_pre_js( demo  ${OXYGINE_JS_LIBRARIES} )
endif(EMSCRIPTEN)