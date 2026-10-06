# Gather all htb_patcher files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

file(GLOB_RECURSE TINY_XML_SOURCES ${CMAKE_SOURCE_DIR}/external/tinyxml/*.cpp)

# htb_patcher executable
add_executable(htb_patcher WIN32 ${HEADERS} ${SOURCES} ${TINY_XML_SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# C++ standard
set_target_properties(htb_patcher PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(htb_patcher PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/htb/htb_lib_win32/src
	${CMAKE_SOURCE_DIR}/htb/htb_lib/src
	${CMAKE_SOURCE_DIR}/external/imgui
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_CURRENT_LIST_DIR}/src
)


# Link libraries
target_link_libraries(htb_patcher PRIVATE htb_lib htb_lib_gui htb_lib_win32 imgui dxgi.lib d3dcompiler.lib d3d11.lib winmm.lib)

# Link directories
target_link_directories(htb_patcher PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")