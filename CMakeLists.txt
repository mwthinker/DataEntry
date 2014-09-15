cmake_minimum_required(VERSION 2.8)
project(DataEntry)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
if (MSVC)
	# Exception handler model.
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /EHsc")
	
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DWIN32)
else (MSVC)
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif (MSVC)

option(DataEntryTest "LoadXmlTest project is added" OFF)

# Source files.
set(SOURCES
	src/dataentry.cpp
	src/dataentry.h
)
set(SOURCES_TEST
	srcTest/main.cpp
)
# End of source files.

add_subdirectory(TinyXML2)

include_directories(
	TinyXML2/src
)

add_library(DataEntry ${SOURCES})

if (DataEntryTest)
	include_directories(src)
	add_executable(DataEntryTest ${SOURCES_TEST})	
	
	target_link_libraries(DataEntryTest DataEntry TinyXML2)
	
	file(COPY dataTest/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
endif (DataEntryTest)
