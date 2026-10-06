# Gather all htb_lib_win32 sources
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Engine library
add_library(htb_lib_win32 STATIC ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(htb_lib_win32 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_SOURCE_DIR}/htb/htb_lib/src
    ${CMAKE_SOURCE_DIR}/htb/htb_lib_gui/src
    ${CMAKE_SOURCE_DIR}/external
)

# Threads (required for std::thread in core/Log)
find_package(Threads REQUIRED)
target_link_libraries(htb_lib_win32 PUBLIC Threads::Threads)

# C++ standard
set_target_properties(htb_lib_win32 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)