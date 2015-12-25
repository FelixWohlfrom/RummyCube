# Project name and version
project(RummyCube)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_BETA TRUE)

# The subdirectories to build
set(SUBDIRS main)

# Minimum cmake version
cmake_minimum_required(VERSION 3.0.2)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Set compiler flags on g++
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    # Here the -D_DEBUG is needed because it's not set under cmake in ubuntu
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -ggdb -Wall -Wextra -O0")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Set compiler flags on visual studio
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(MSVC)

if(CMAKE_CXX_FLAGS_RELEASE)
    add_definitions(-DUSED_CXXFLAGS="${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
endif(CMAKE_CXX_FLAGS_RELEASE)
if (CMAKE_CXX_FLAGS_DEBUG)
    add_definitions(-DUSED_CXXFLAGS_DEBUG="${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
endif(CMAKE_CXX_FLAGS_DEBUG)
        
# Set some Win Specific Settings
if(WIN32)
    set(GUI_TYPE WIN32) # Disables console
#	include_directories(main/gui/resources/msw)
#	set(RSRC main/gui/resources/msw/RummyCube.rc)

	if (MSVC)
		add_definitions(-D_VARIADIC_MAX=10)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_CRT_SECURE_NO_WARNINGS")
		include_directories("C:/Program Files (x86)/Visual Leak Detector/include")
		if (CMAKE_CL_64)
			link_directories("C:/Program Files (x86)/Visual Leak Detector/lib/Win64")
		else(CMAKE_CL_64)
			link_directories("C:/Program Files (x86)/Visual Leak Detector/lib/Win32")
		endif(CMAKE_CL_64)
	endif(MSVC)
else(WIN32)
    set(GUI_TYPE )
    set(RSRC )
endif(WIN32)

    # Set some Apple MacOS Specific settings
    if(APPLE)
        set(GUI_TYPE MACOSX_BUNDLE)
    endif(APPLE)
    
# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
	include_directories(${SUB})
	link_directories(${PROJECT_BINARY_DIR}/${SUB})
	add_subdirectory(${SUB})
endforeach(SUB)
