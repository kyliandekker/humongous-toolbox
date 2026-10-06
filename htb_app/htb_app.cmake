# Gather all htb_app files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# htb_app executable
add_executable(htb_app WIN32 ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# C++ standard
set_target_properties(htb_app PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(htb_app PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/hewin32/src
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_SOURCE_DIR}/external/imgui
	${CMAKE_CURRENT_LIST_DIR}/src
)

# Link libraries
target_link_libraries(htb_app PRIVATE dxgi.lib d3dcompiler.lib d3d11.lib winmm.lib Shcore.lib htb_lib imgui htb_lib_win32 htb_lib_gui)

# Link directories
target_link_directories(htb_app PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")