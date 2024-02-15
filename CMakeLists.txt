cmake_minimum_required(VERSION 3.10)
project(Dareman)

#Add all the source files in the code directory
file(GLOB SOURCE_CODE "Code/*")
source_group("Code" FILES ${SOURCE_CODE})
add_executable(Dareman WIN32 ${SOURCE_CODE})

set_target_properties(Dareman PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Binaries)
set_property(TARGET Dareman PROPERTY CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Werror -fsanitize=undefined -fsanitize=address"
  )
  target_link_options(Dareman
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	set_target_properties(Dareman PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
	target_include_directories(Dareman PRIVATE Libs/SDL2-2.0.7/include/)
	target_include_directories(Dareman PRIVATE Libs/SDL2_ttf-2.0.14/include/)
	target_include_directories(Dareman PRIVATE Libs/SDL2_image-2.0.2/include/)

	# Support both 32 and 64 bit builds
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
		set(SDL2_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2-2.0.7/lib/x64/")
		set(SDL2_IMAGE_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_image-2.0.2/lib/x64/")
		set(SDL2_TTF_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_ttf-2.0.14/lib/x64/")
	else ()
		set(SDL2_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2-2.0.7/lib/x86/")
		set(SDL2_IMAGE_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_image-2.0.2/lib/x86/")
		set(SDL2_TTF_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_ttf-2.0.14/lib/x86/")
	endif ()


	find_library(SDL2_LIB SDL2 PATH ${SDL2_LIBPATH})
	find_library(SDL2_MAIN_LIB SDL2main PATH ${SDL2_LIBPATH})
	find_library(SDL2_IMAGE_LIB SDL2_image PATH ${SDL2_IMAGE_LIBPATH})
	find_library(SDL2_TTF_LIB SDL2_ttf PATH ${SDL2_TTF_LIBPATH})

	target_link_libraries(Dareman ${SDL2_MAIN_LIB})
	target_link_libraries(Dareman ${SDL2_LIB})
	target_link_libraries(Dareman ${SDL2_IMAGE_LIB})
	target_link_libraries(Dareman ${SDL2_TTF_LIB})

	file(GLOB_RECURSE DLLS "${SDL2_LIBPATH}/*.dll" "${SDL2_IMAGE_LIBPATH}/*.dll" "${SDL2_TTF_LIBPATH}/*.dll")

	add_custom_command(TARGET Dareman POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${DLLS} $<TARGET_FILE_DIR:Dareman>/
	)
else()
	include(FindPkgConfig)
	pkg_search_module(SDL2 REQUIRED sdl2 >= 2.0.5)
	pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

	target_include_directories(Dareman PRIVATE ${SDL2_INCLUDE_DIRS}	${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
	target_link_libraries(Dareman ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} )
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
