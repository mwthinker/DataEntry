cmake_minimum_required(VERSION 3.14)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Use VCPKG CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(DataEntry
	DESCRIPTION
		"A wrapper of TinyXml2"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	# Exception handler model.
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /EHsc")
	
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DWIN32)
endif ()

find_package(tinyxml2 CONFIG REQUIRED)
    
add_library(DataEntry 
	src/xml/dataentry.cpp
	src/xml/dataentry.h
)

target_include_directories(DataEntry
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

target_link_libraries(DataEntry
	PUBLIC
		tinyxml2::tinyxml2
)

message(STATUS "DataEntry_Test is available to add: -DDataEntry_Test=1")
option(DataEntry_Test "Add DataEntry_Test project." OFF)
if (DataEntry_Test)
	add_subdirectory(DataEntry_Test)
endif (DataEntry_Test)
