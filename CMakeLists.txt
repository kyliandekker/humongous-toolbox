cmake_minimum_required(VERSION 3.20)

project(humongous_toolbox)

if (MSVC)
    add_compile_options(/MP)
endif()

# Configurations
set(RELEASE "Release")
set(DEBUG "Debug")
set(CMAKE_CONFIGURATION_TYPES "${RELEASE};${DEBUG}" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Linker flags (MSVC only)
if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "Debug linker flags" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF /OPT:ICF" CACHE STRING "Release linker flags" FORCE)
endif()

# Global settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMakePredefinedTargets)

add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)

# Third-party libraries

# Include modules
include(htb/htb_lib/htb_lib.cmake)
include(htb/htb_lib_gui/htb_lib_gui.cmake)

# Libraries
set_property(TARGET htb_lib PROPERTY FOLDER "htb_libs")
set_property(TARGET htb_lib_gui PROPERTY FOLDER "htb_libs")

# Platform-specific modules (Win32 only)
if(WIN32)
	# IMGUI
	file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp)

	add_library(imgui STATIC ${IMGUI_SOURCES})
	target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/external ${CMAKE_SOURCE_DIR}/external/imgui)
	set_target_properties(imgui PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON FOLDER "ThirdParty")
	source_group(TREE ${CMAKE_SOURCE_DIR}/external/imgui PREFIX "" FILES ${IMGUI_SOURCES})

	include(htb/htb_lib_win32/htb_lib_win32.cmake)
	set_property(TARGET htb_lib_win32 PROPERTY FOLDER "htb_libs")

	include(htb_patcher/htb_patcher.cmake)
	set_property(TARGET htb_patcher PROPERTY FOLDER "htb_exe")

	include(htb_app/htb_app.cmake)
	set_property(TARGET htb_app PROPERTY FOLDER "htb_exe")
endif()

include(htb_cli/htb_cli.cmake)
set_property(TARGET htb_cli PROPERTY FOLDER "htb_exe")