cmake_minimum_required(VERSION 3.14)
project(DataEntry_Test
	DESCRIPTION
		"Tests DataEntry library"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DataEntry_Test
	src/main.cpp
)
	
target_link_libraries(DataEntry_Test
	PRIVATE
		DataEntry
)

file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
