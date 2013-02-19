# Project name and version
project(RummyCube)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_BETA TRUE)

# The subdirectories to build
set(SUBDIRS network game gui)

# Minimum cmake version
cmake_minimum_required(VERSION 2.8)

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Find wxWidgets
find_package(wxWidgets COMPONENTS core base adv net REQUIRED)

if(wxWidgets_FOUND)
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
    
	# wxWidgets include (this will do all the magic to configure everything)
	include(${wxWidgets_USE_FILE})
    
	# Set some Win Specific Settings
	if(WIN32)
		set(GUI_TYPE WIN32) # Disables console
		include_directories(gui/resources/msw)
		set(RSRC gui/resources/msw/RummyCube.rc)

		if (MSVC)
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

	# Generate the version file
	configure_file(
	  "${PROJECT_SOURCE_DIR}/Version.h.in"
	  "${PROJECT_BINARY_DIR}/Version.h"
	)

	# Make version file readable for project
	include_directories("${PROJECT_BINARY_DIR}")
    
	# Build subdirs and include for build
	foreach(SUB ${SUBDIRS})
		include_directories(${PROJECT_SOURCE_DIR}/${SUB})
		link_directories(${PROJECT_BINARY_DIR}/${SUB})
		add_subdirectory(${SUB})
	endforeach(SUB)

	# Build the internationalization
	source_group("Header Files" FILES Internationalization.h)
	add_library(I18n Internationalization.cpp Internationalization.h)
	target_link_libraries(I18n ${wxWidgets_LIBRARIES})

	# Generate the main program
	source_group("Header Files" FILES Main.h Version.h)
	add_executable(RummyCube ${GUI_TYPE} ${RSRC} Main.cpp Main.h)
	target_link_libraries(RummyCube Gui)

# No wxWidgets found, inform user
else(wxWidgets_FOUND)
	message("wxWidgets not found!")
endif(wxWidgets_FOUND)
