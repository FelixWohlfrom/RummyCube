# Project name and version
project(RummyCube)
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_BETA TRUE)

# The subdirectories to build
set(SUBDIRS src tests)

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
		include_directories(src/gui/resources/msw)
		set(RSRC src/gui/resources/msw/RummyCube.rc)

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
    
	# Build subdirs and include for build
	foreach(SUB ${SUBDIRS})
		add_subdirectory(${SUB})
	endforeach(SUB)
	
	# Generate the main program
	source_group("Header Files" FILES src/Main.h src/Version.h)
	add_executable(RummyCube ${GUI_TYPE} ${RSRC} src/Main.cpp src/Main.h)
	target_link_libraries(RummyCube Gui)

	# Configure tests
	add_subdirectory(libs/gtest-1.6.0)
	enable_testing ()
	add_test(NAME RummyCubeTest COMMAND RummyCubeTest)

# No wxWidgets found, inform user
else(wxWidgets_FOUND)
	message("wxWidgets not found!")
endif(wxWidgets_FOUND)
