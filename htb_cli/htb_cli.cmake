# Gather all htb_cli files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

file(GLOB_RECURSE TINY_XML_SOURCES ${CMAKE_SOURCE_DIR}/external/tinyxml/*.cpp)

# htb_cli executable
add_executable(htb_cli ${HEADERS} ${SOURCES} ${TINY_XML_SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# C++ standard
set_target_properties(htb_cli PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(htb_cli PUBLIC 
	${CMAKE_SOURCE_DIR}/htb/htb_lib/src
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_CURRENT_LIST_DIR}/src
)


# Link libraries
target_link_libraries(htb_cli PRIVATE htb_lib)